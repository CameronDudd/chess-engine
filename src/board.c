/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include "board.h"

#include <log.h>
#include <stdio.h>

#include "defs.h"
#include "magics.h"

#define BORDER "+---------------+"
#define PIECE_WHITE(piece) ((PIECE_NULL < (piece)) && ((piece) <= WP))

static PositionIndex getLSB(BitBoard* bitboard) {
  PositionIndex pos = __builtin_ctzll(*bitboard);
  *bitboard &= *bitboard - 1;
  return pos;
}

static void pushMoves(BitBoard occupancy, PositionIndex src, BitBoard* moves, Move** movePtr) {
  while (*moves) {
    PositionIndex dst = getLSB(moves);
    MoveFlag flag     = (POSITION_BIT(dst) & occupancy) ? CAPTURE : QUIET;
    *(*movePtr)++     = MOVE(flag, dst, src);
  }
}

#define BORDER "+---------------+"

static const char pieceCharacterLookup[PIECE_END] = {' ', 'K', 'Q', 'R', 'B', 'N', 'P', 'k', 'q', 'r', 'b', 'n', 'p'};

static char pieceChar(const Piece piece) {
  if ((piece < 0) || (piece >= PIECE_END)) return '?';
  return pieceCharacterLookup[piece];
}

void displayBoard(const Board* board) {
  printf("  %s\r\n", BORDER);
  for (int rank = 7; rank >= 0; --rank) {
    printf("%c ", '1' + rank);
    for (int file = 0; file < 8; ++file) {
      PositionIndex i = POS_INDEX(rank, file);
      printf("|%c", pieceChar(board->squares[i]));
    }
    printf("|\r\n");
  }
  printf("  %s\r\n   ", BORDER);
  for (int file = 0; file < 8; ++file) {
    printf("%c ", 'a' + file);
  }
  printf("\r\n");
}

void displayBitBoard(const BitBoard board) {
  printf("  %s\r\n", BORDER);
  for (int rank = 7; rank >= 0; --rank) {
    printf("%c ", '1' + rank);
    for (int file = 0; file < 8; ++file) {
      PositionIndex i = POS_INDEX(rank, file);
      printf("|%c", (board & POSITION_BIT(i)) ? '*' : ' ');
    }
    printf("|\r\n");
  }
  printf("  %s\r\n   ", BORDER);
  for (int file = 0; file < 8; ++file) {
    printf("%c ", 'a' + file);
  }
  printf("\r\n");
}

bool moveCheck(const Move move) {
  return FLAG(move) == CHECK;
}

bool moveCastle(const Move move) {
  MoveFlag flag = FLAG(move);
  return flag == KING_CASTLE || flag == QUEEN_CASTLE;
}

bool moveCapture(const Move move) {
  return FLAG(move) == CAPTURE;
}

bool moveCheckmate(const Move move) {
  return FLAG(move) == CHECKMATE;
}

bool movePromotion(const Move move) {
  MoveFlag flag = FLAG(move);
  return flag == QUEEN_PROMOTION || flag == ROOK_PROMOTION || flag == BISHOP_PROMOTION || flag == KNIGHT_PROMOTION;
}

void initBoard(Board* board) {
  // Zero out the board
  for (int i = 0; i < NUM_POSITIONS; ++i) board->squares[i] = (Piece)0;
  board->turn                 = WHITE;
  board->castlingAvailability = 0;
  board->whites               = (BitBoard)0;
  board->blacks               = (BitBoard)0;
  board->enemyAttacks         = (BitBoard)0;
  for (int i = 0; i < NUM_PIECE_TYPES; ++i) board->whitePieceBoards[i] = (BitBoard)0;
  for (int i = 0; i < NUM_PIECE_TYPES; ++i) board->blackPieceBoards[i] = (BitBoard)0;
}

void boardSetPiece(Board* board, const PositionIndex position, const Piece piece) {
  if (piece == PIECE_NULL) return;

  if (PIECE_WHITE(piece)) {
    board->whites |= POSITION_BIT(position);
    board->whitePieceBoards[piece - WK] |= POSITION_BIT(position);
  } else {
    board->blacks |= POSITION_BIT(position);
    board->blackPieceBoards[piece - BK] |= POSITION_BIT(position);
  }
  board->squares[position] = piece;
}

void boardRemovePiece(Board* board, const PositionIndex position, const Piece piece) {
  if (piece == PIECE_NULL) return;

  BitBoard positionBit = POSITION_BIT(position);

  switch (piece) {
    case WK:
      board->castlingAvailability &= ~(CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN);
      break;
    case BK:
      board->castlingAvailability &= ~(CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN);
      break;
    case WR:
      if (positionBit & RANK_1 & FILE_A) {
        board->castlingAvailability &= ~CASTLE_WHITE_QUEEN;
      } else if (positionBit & RANK_1 & FILE_H) {
        board->castlingAvailability &= ~CASTLE_WHITE_KING;
      }
      break;
    case BR:
      if (positionBit & RANK_8 & FILE_A) {
        board->castlingAvailability &= ~CASTLE_BLACK_QUEEN;
      } else if (positionBit & RANK_8 & FILE_H) {
        board->castlingAvailability &= ~CASTLE_BLACK_KING;
      }
      break;
    default:
      break;
  }

  if (PIECE_WHITE(piece)) {
    board->whites &= ~positionBit;
    board->whitePieceBoards[piece - WK] &= ~POSITION_BIT(position);
  } else {
    board->blacks &= ~positionBit;
    board->blackPieceBoards[piece - BK] &= ~POSITION_BIT(position);
  }
  board->squares[position] = PIECE_NULL;
}

Piece boardGetPiece(const Board* board, PositionIndex position) {
  return board->squares[position];
}

void boardMakeMove(Board* board, Move move, UndoMove* undo) {
  PositionIndex src = SRC(move);
  PositionIndex dst = DST(move);
  MoveFlag flag     = FLAG(move);

  if (flag == KING_CASTLE || flag == QUEEN_CASTLE) {
    displayBoard(board);
  }

  Piece piece  = board->squares[src];
  Piece target = board->squares[dst];

  undo->move                 = move;
  undo->captured             = target;
  undo->castlingAvailability = board->castlingAvailability;

  if (target != PIECE_NULL) boardRemovePiece(board, dst, target);

  boardRemovePiece(board, src, piece);
  boardSetPiece(board, dst, piece);

  UndoMove phonyUndo;
  if (flag == KING_CASTLE) {
    boardMakeMove(board, MOVE(QUIET, src + 1, dst + 1), &phonyUndo);
  } else if (flag == QUEEN_CASTLE) {
    boardMakeMove(board, MOVE(QUIET, src - 1, dst - 2), &phonyUndo);
  } else {
    board->turn = (board->turn == WHITE) ? BLACK : WHITE;
  }

  if (flag == KING_CASTLE || flag == QUEEN_CASTLE) {
    displayBoard(board);
  }
}

void boardUnmakeMove(Board* board, const UndoMove* undo) {
  Move move = undo->move;

  PositionIndex src = SRC(move);
  PositionIndex dst = DST(move);
  MoveFlag flag     = FLAG(move);

  Piece piece = board->squares[dst];

  boardRemovePiece(board, dst, piece);
  boardSetPiece(board, src, piece);

  board->castlingAvailability = undo->castlingAvailability;
  if (undo->captured != PIECE_NULL) boardSetPiece(board, dst, undo->captured);

  if (flag == KING_CASTLE) {
    boardUnmakeMove(board, &(UndoMove){
                               .castlingAvailability = board->castlingAvailability,
                               .captured             = PIECE_NULL,
                               .move                 = MOVE(QUIET, src + 1, dst + 1),
                           });
  } else if (flag == QUEEN_CASTLE) {
    boardUnmakeMove(board, &(UndoMove){
                               .castlingAvailability = board->castlingAvailability,
                               .captured             = PIECE_NULL,
                               .move                 = MOVE(QUIET, src - 1, dst - 2),
                           });
  } else {
    board->turn = (board->turn == WHITE) ? BLACK : WHITE;
  }
}

void boardSetTurnColor(Board* board, Color turnColor) {
  log_debug("Setting board turn to %s", (turnColor == WHITE) ? "WHITE" : "BLACK");
  board->turn = turnColor;
}

void boardSetCastlingAvailability(Board* board, CastlingAvailability castlingAvailability) {
#if DEBUG
  switch (castlingAvailability) {
    case CASTLE_WHITE_KING:
      log_debug("Setting board white king castle available");
      break;
    case CASTLE_WHITE_QUEEN:
      log_debug("Setting board white queen castle available");
      break;
    case CASTLE_BLACK_KING:
      log_debug("Setting board black king castle available");
      break;
    case CASTLE_BLACK_QUEEN:
      log_debug("Setting board black queen castle available");
      break;
  }
#endif
  board->castlingAvailability |= castlingAvailability;
}

BitBoard kingMoves(PositionIndex pos) {
  return kingAttacks[pos];
}

BitBoard rookMoves(PositionIndex pos, BitBoard occupancy) {
  occupancy &= rookMasks[pos];
  uint64_t magic = (occupancy * RookMagics[pos]) >> (NUM_POSITIONS - rookNumBits[pos]);
  return rookAttacks[pos][magic];
}

BitBoard bishopMoves(PositionIndex pos, BitBoard occupancy) {
  occupancy &= bishopMasks[pos];
  uint64_t magic = (occupancy * BishopMagics[pos]) >> (NUM_POSITIONS - bishopNumBits[pos]);
  return bishopAttacks[pos][magic];
}

BitBoard queenMoves(PositionIndex pos, BitBoard occupancy) {
  return rookMoves(pos, occupancy) | bishopMoves(pos, occupancy);
}

BitBoard knightMoves(PositionIndex pos) {
  return knightAttacks[pos];
}

BitBoard colorAttack(Board* board, Color side) {
  BitBoard attacks = (BitBoard)0;

  BitBoard occupancy = board->whites | board->blacks;

  BitBoard* pieceBitboards = (side == WHITE) ? board->whitePieceBoards : board->blackPieceBoards;

  BitBoard kings = pieceBitboards[KING];
  while (kings) {
    PositionIndex pos = getLSB(&kings);
    attacks |= kingAttacks[pos];
  }

  BitBoard queens = pieceBitboards[QUEEN];
  while (queens) {
    PositionIndex pos = getLSB(&queens);
    attacks |= rookMoves(pos, occupancy) | bishopMoves(pos, occupancy);
  }

  BitBoard rooks = pieceBitboards[ROOK];
  while (rooks) {
    PositionIndex pos = getLSB(&rooks);
    attacks |= rookMoves(pos, occupancy);
  }

  BitBoard bishops = pieceBitboards[BISHOP];
  while (bishops) {
    PositionIndex pos = getLSB(&bishops);
    attacks |= bishopMoves(pos, occupancy);
  }

  BitBoard knights = pieceBitboards[KNIGHT];
  while (knights) {
    PositionIndex pos = getLSB(&knights);
    attacks |= knightAttacks[pos];
  }

  BitBoard pawns = pieceBitboards[PAWN];
  while (pawns) {
    PositionIndex pos = getLSB(&pawns);
    attacks |= pawnAttacks[side][pos];
  }

  return attacks;
}

bool kingInCheck(Board* board, Color side) {
  if (side == WHITE) {
    return (board->whitePieceBoards[KING] & colorAttack(board, BLACK)) != 0;
  }
  return (board->blackPieceBoards[KING] & colorAttack(board, WHITE)) != 0;
}

void generatePseudoLegalMoves(Board* board, Move** movePtr) {
  BitBoard* pieceBitboards = (board->turn == WHITE) ? board->whitePieceBoards : board->blackPieceBoards;
  BitBoard friendlyPieces  = (board->turn == WHITE) ? board->whites : board->blacks;
  BitBoard enemyPieces     = (board->turn == WHITE) ? board->blacks : board->whites;
  BitBoard occupancy       = board->whites | board->blacks;

  CastlingAvailability canCastleKingSide =
      (board->turn == WHITE) ? board->castlingAvailability & CASTLE_WHITE_KING : board->castlingAvailability & CASTLE_BLACK_KING;
  BitBoard kingSideCastleMask  = (board->turn == WHITE) ? WHITE_KING_CASTLE_MASK : BLACK_KING_CASTLE_MASK;
  BitBoard queenSideCastleMask = (board->turn == WHITE) ? WHITE_QUEEN_CASTLE_MASK : BLACK_QUEEN_CASTLE_MASK;

  CastlingAvailability canCastleQueenSide =
      (board->turn == WHITE) ? board->castlingAvailability & CASTLE_WHITE_QUEEN : board->castlingAvailability & CASTLE_BLACK_QUEEN;

  BitBoard kings = pieceBitboards[KING];
  while (kings) {
    PositionIndex src = getLSB(&kings);

    if (canCastleKingSide && ((occupancy & kingSideCastleMask) == 0)) {
      *(*movePtr)++ = MOVE(KING_CASTLE, src + 2, src);
    } else if (canCastleQueenSide && ((occupancy & queenSideCastleMask) == 0)) {
      *(*movePtr)++ = MOVE(QUEEN_CASTLE, src - 2, src);
    }

    BitBoard moveBitboard = kingAttacks[src] & ~friendlyPieces;
    pushMoves(occupancy, src, &moveBitboard, movePtr);
  }

  BitBoard queens = pieceBitboards[QUEEN];
  while (queens) {
    PositionIndex src     = getLSB(&queens);
    BitBoard moveBitboard = (rookMoves(src, occupancy) | bishopMoves(src, occupancy)) & ~friendlyPieces;
    pushMoves(occupancy, src, &moveBitboard, movePtr);
  }

  BitBoard rooks = pieceBitboards[ROOK];
  while (rooks) {
    PositionIndex src     = getLSB(&rooks);
    BitBoard moveBitboard = rookMoves(src, occupancy) & ~friendlyPieces;
    pushMoves(occupancy, src, &moveBitboard, movePtr);
  }

  BitBoard bishops = pieceBitboards[BISHOP];
  while (bishops) {
    PositionIndex src     = getLSB(&bishops);
    BitBoard moveBitboard = bishopMoves(src, occupancy) & ~friendlyPieces;
    pushMoves(occupancy, src, &moveBitboard, movePtr);
  }

  BitBoard knights = pieceBitboards[KNIGHT];
  while (knights) {
    PositionIndex src     = getLSB(&knights);
    BitBoard moveBitboard = knightAttacks[src] & ~friendlyPieces;
    pushMoves(occupancy, src, &moveBitboard, movePtr);
  }

  BitBoard pawns = pieceBitboards[PAWN];
  while (pawns) {
    PositionIndex src     = getLSB(&pawns);
    BitBoard moveBitboard = pawnAttacks[board->turn][src] & enemyPieces;
    BitBoard singleMove   = pawnSingleMoves[board->turn][src] & ~occupancy;
    BitBoard doubleMove   = singleMove ? pawnDoubleMoves[board->turn][src] & ~occupancy : (BitBoard)0;
    moveBitboard |= singleMove | doubleMove;
    while (moveBitboard) {
      PositionIndex dst = getLSB(&moveBitboard);
      int rank          = INDEX_RANK(dst);
      if ((board->turn == WHITE && rank == 7) || (board->turn == BLACK && rank == 0)) {
        *(*movePtr)++ = MOVE(QUEEN_PROMOTION, dst, src);
        *(*movePtr)++ = MOVE(ROOK_PROMOTION, dst, src);
        *(*movePtr)++ = MOVE(BISHOP_PROMOTION, dst, src);
        *(*movePtr)++ = MOVE(KNIGHT_PROMOTION, dst, src);
      } else {
        *(*movePtr)++ = MOVE(QUIET, dst, src);
      }
    }
  }
}

unsigned int generateLegalMoves(Board* board, Move* moves) {
  for (int i = 0; i < MAX_CHESS_MOVES; ++i) moves[i] = (Move)0;

  Move pseudoLegalMoves[MAX_CHESS_MOVES] = {0};
  Move* movePtr                          = pseudoLegalMoves;
  generatePseudoLegalMoves(board, &movePtr);

  // TODO:
  //  - [ ] Can't castle through check
  //  - [ ] Can't castle out of check
  //  - [ ] Can't castle into check

  Move* end  = movePtr;
  Move* wPtr = moves;
  for (Move* rPtr = pseudoLegalMoves; rPtr < end; ++rPtr) {
    Color movingSide = board->turn;
    UndoMove undo;
    boardMakeMove(board, *rPtr, &undo);
    if (!kingInCheck(board, movingSide)) {
      *wPtr++ = *rPtr;
    }
    boardUnmakeMove(board, &undo);
  }

  return wPtr - moves;
}
