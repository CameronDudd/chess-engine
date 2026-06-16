/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include "board.h"

#include <log.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "magics.h"

#define BORDER "+---------------+"
#define PIECE_WHITE(piece) ((PIECE_NULL < (piece)) && ((piece) <= WP))

static CastlingAvailability castlingAvailabilityLookup[NUM_POSITIONS];

static PositionIndex getLSB(BitBoard* bitboard) {
  PositionIndex pos = __builtin_ctzll(*bitboard);
  *bitboard &= *bitboard - 1;
  return pos;
}

static void pushMoves(BitBoard enemyKing, BitBoard enemyPieces, PositionIndex src, BitBoard* moves, Move** movePtr) {
  enemyPieces &= ~enemyKing;
  while (*moves) {
    PositionIndex dst = getLSB(moves);
    MoveFlag flag     = MOVE_QUIET;
    if (POSITION_BIT(dst) & enemyPieces) flag |= MOVE_CAPTURE;
    *(*movePtr)++ = MOVE(flag, dst, src);
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
  for (int rank = MAX_RANK; rank >= 0; --rank) {
    printf("%c ", '1' + rank);
    for (int file = 0; file < NUM_FILES; ++file) {
      PositionIndex i = POS_INDEX(rank, file);
      printf("|%c", pieceChar(board->squares[i]));
    }
    printf("|\r\n");
  }
  printf("  %s\r\n   ", BORDER);
  for (int file = 0; file < NUM_FILES; ++file) {
    printf("%c ", 'a' + file);
  }
  printf("\r\n");
}

void displayBitBoard(BitBoard board) {
  printf("  %s\r\n", BORDER);
  for (int rank = MAX_RANK; rank >= 0; --rank) {
    printf("%c ", '1' + rank);
    for (int file = 0; file < NUM_FILES; ++file) {
      PositionIndex i = POS_INDEX(rank, file);
      printf("|%c", (board & POSITION_BIT(i)) ? '*' : ' ');
    }
    printf("|\r\n");
  }
  printf("  %s\r\n   ", BORDER);
  for (int file = 0; file < NUM_FILES; ++file) {
    printf("%c ", 'a' + file);
  }
  printf("\r\n");
}

bool moveCastle(Move move) {
  return FLAG(move) & (MOVE_KING_CASTLE | MOVE_QUEEN_CASTLE);
}

bool moveCapture(Move move) {
  return FLAG(move) & MOVE_CAPTURE;
}

bool movePromotion(Move move) {
  return FLAG(move) & (MOVE_QUEEN_PROMOTION | MOVE_ROOK_PROMOTION | MOVE_BISHOP_PROMOTION | MOVE_KNIGHT_PROMOTION);
}

bool moveEP(Move move) {
  return FLAG(move) & MOVE_EP;
}

void initCastlingAvailability(void) {
  for (PositionIndex i = 0; i < NUM_POSITIONS; ++i) castlingAvailabilityLookup[i] = NAP;
  castlingAvailabilityLookup[A1] = ~CASTLE_WHITE_QUEEN;
  castlingAvailabilityLookup[E1] = ~(CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN);
  castlingAvailabilityLookup[H1] = ~CASTLE_WHITE_KING;
  castlingAvailabilityLookup[A8] = ~CASTLE_BLACK_QUEEN;
  castlingAvailabilityLookup[E8] = ~(CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN);
  castlingAvailabilityLookup[H8] = ~CASTLE_BLACK_KING;
}

void initBoard(Board* board) {
  // Zero out the board
  for (int i = 0; i < NUM_POSITIONS; ++i) board->squares[i] = (Piece)0;
  board->turn                 = WHITE;
  board->castlingAvailability = 0;
  board->epSquare             = NAP;
  board->whites               = (BitBoard)0;
  board->blacks               = (BitBoard)0;
  for (int i = 0; i < NUM_PIECE_TYPES; ++i) board->whitePieceBoards[i] = (BitBoard)0;
  for (int i = 0; i < NUM_PIECE_TYPES; ++i) board->blackPieceBoards[i] = (BitBoard)0;

  initMagicAttacks();
  initCastlingAvailability();
}

void boardSetPiece(Board* board, PositionIndex position, Piece piece) {
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

  if (PIECE_WHITE(piece)) {
    board->whites &= ~positionBit;
    board->whitePieceBoards[piece - WK] &= ~positionBit;
  } else {
    board->blacks &= ~positionBit;
    board->blackPieceBoards[piece - BK] &= ~positionBit;
  }

  board->squares[position] = PIECE_NULL;
}

Piece boardGetPiece(const Board* board, PositionIndex position) {
  return board->squares[position];
}

void boardMakeMove(Board* board, Move move, UndoMove* undo) {  // NOLINT(readability-function-cognitive-complexity)
  PositionIndex src = SRC(move);
  PositionIndex dst = DST(move);
  MoveFlag flag     = FLAG(move);

  Piece piece  = board->squares[src];
  Piece target = board->squares[dst];

  undo->move                 = move;
  undo->captured             = target;
  undo->castlingAvailability = board->castlingAvailability;
  undo->epSquare             = board->epSquare;
  undo->epCapturedSquare     = NAP;

  board->epSquare = NAP;

  if (target != PIECE_NULL) boardRemovePiece(board, dst, target);

  boardRemovePiece(board, src, piece);
  boardSetPiece(board, dst, piece);

  board->castlingAvailability &= castlingAvailabilityLookup[src];
  board->castlingAvailability &= castlingAvailabilityLookup[dst];

  if (flag & MOVE_KING_CASTLE) {
    Piece rook = (board->turn == WHITE) ? WR : BR;
    boardRemovePiece(board, dst + 1, rook);
    boardSetPiece(board, src + 1, rook);
  } else if (flag & MOVE_QUEEN_CASTLE) {
    Piece rook = (board->turn == WHITE) ? WR : BR;
    boardRemovePiece(board, dst - 2, rook);
    boardSetPiece(board, dst + 1, rook);
  } else if (flag & MOVE_EP) {
    undo->epCapturedSquare = (board->turn == WHITE) ? dst - NUM_FILES : dst + NUM_FILES;
    boardRemovePiece(board, undo->epCapturedSquare, (board->turn == WHITE) ? BP : WP);
  } else if (flag & MOVE_DOUBLE_PUSH) {
    board->epSquare = (src + dst) / 2;
  } else if (flag & MOVE_QUEEN_PROMOTION) {
    boardRemovePiece(board, dst, piece);
    boardSetPiece(board, dst, (board->turn == WHITE) ? WQ : BQ);
  } else if (flag & MOVE_ROOK_PROMOTION) {
    boardRemovePiece(board, dst, piece);
    boardSetPiece(board, dst, (board->turn == WHITE) ? WR : BR);
  } else if (flag & MOVE_BISHOP_PROMOTION) {
    boardRemovePiece(board, dst, piece);
    boardSetPiece(board, dst, (board->turn == WHITE) ? WB : BB);
  } else if (flag & MOVE_KNIGHT_PROMOTION) {
    boardRemovePiece(board, dst, piece);
    boardSetPiece(board, dst, (board->turn == WHITE) ? WN : BN);
  }

  board->turn = (board->turn == WHITE) ? BLACK : WHITE;
}

void boardUnmakeMove(Board* board, const UndoMove* undo) {
  Move move = undo->move;

  PositionIndex src = SRC(move);
  PositionIndex dst = DST(move);
  MoveFlag flag     = FLAG(move);

  Piece piece = board->squares[dst];

  boardRemovePiece(board, dst, piece);

  if (flag & (MOVE_QUEEN_PROMOTION | MOVE_ROOK_PROMOTION | MOVE_BISHOP_PROMOTION | MOVE_KNIGHT_PROMOTION)) {
    piece = (board->turn == WHITE) ? BP : WP;
  }

  boardSetPiece(board, src, piece);

  board->castlingAvailability = undo->castlingAvailability;
  board->epSquare             = undo->epSquare;

  if (undo->captured != PIECE_NULL) boardSetPiece(board, dst, undo->captured);

  if (flag & MOVE_KING_CASTLE) {
    Piece rook = (board->turn == WHITE) ? BR : WR;
    boardRemovePiece(board, src + 1, rook);
    boardSetPiece(board, dst + 1, rook);
  } else if (flag & MOVE_QUEEN_CASTLE) {
    Piece rook = (board->turn == WHITE) ? BR : WR;
    boardRemovePiece(board, dst + 1, rook);
    boardSetPiece(board, dst - 2, rook);
  } else if (flag & MOVE_EP) {
    boardSetPiece(board, undo->epCapturedSquare, board->turn == WHITE ? WP : BP);
  }

  board->turn = (board->turn == WHITE) ? BLACK : WHITE;
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
    default:
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

bool colorAttacksSquare(Board* board, PositionIndex position, Color side) {
  BitBoard occupancy       = board->whites | board->blacks;
  BitBoard* pieceBitboards = (side == WHITE) ? board->whitePieceBoards : board->blackPieceBoards;
  Color oppositeSide       = (side == WHITE) ? BLACK : WHITE;

  if (pieceBitboards[KING] & kingAttacks[position]) return true;
  if (pieceBitboards[KNIGHT] & knightAttacks[position]) return true;
  if (pieceBitboards[PAWN] & pawnAttacks[oppositeSide][position]) return true;
  if ((pieceBitboards[ROOK] | pieceBitboards[QUEEN]) & rookMoves(position, occupancy)) return true;
  if ((pieceBitboards[BISHOP] | pieceBitboards[QUEEN]) & bishopMoves(position, occupancy)) return true;

  return false;
}

bool colorAttacksBitboard(Board* board, BitBoard bitboard, Color side) {
  while (bitboard) {
    PositionIndex position = getLSB(&bitboard);
    if (colorAttacksSquare(board, position, side)) return true;
  }
  return false;
}

bool kingInCheck(Board* board, Color side) {
  if (side == WHITE) {
    return colorAttacksSquare(board, __builtin_ctzll(board->whitePieceBoards[KING]), BLACK);
  }
  return colorAttacksSquare(board, __builtin_ctzll(board->blackPieceBoards[KING]), WHITE);
}

void generatePseudoLegalMoves(Board* board, Move** movePtr) {  // NOLINT(readability-function-cognitive-complexity)
  BitBoard* pieceBitboards = (board->turn == WHITE) ? board->whitePieceBoards : board->blackPieceBoards;
  BitBoard friendlyPieces  = (board->turn == WHITE) ? board->whites : board->blacks;
  BitBoard enemyPieces     = (board->turn == WHITE) ? board->blacks : board->whites;
  BitBoard occupancy       = board->whites | board->blacks;
  BitBoard enemyKing       = (board->turn == WHITE) ? board->blackPieceBoards[KING] : board->whitePieceBoards[KING];
  BitBoard enemyAttacks    = colorAttack(board, board->turn == WHITE ? BLACK : WHITE);

  CastlingAvailability kingCastleAvailable =
      (board->turn == WHITE) ? board->castlingAvailability & CASTLE_WHITE_KING : board->castlingAvailability & CASTLE_BLACK_KING;
  CastlingAvailability queenCastleAvailable =
      (board->turn == WHITE) ? board->castlingAvailability & CASTLE_WHITE_QUEEN : board->castlingAvailability & CASTLE_BLACK_QUEEN;

  BitBoard kingCastleMask      = (board->turn == WHITE) ? WHITE_KING_CASTLE_MASK : BLACK_KING_CASTLE_MASK;
  BitBoard kingRookCastleMask  = (board->turn == WHITE) ? WHITE_KING_ROOK_CASTLE_MASK : BLACK_KING_ROOK_CASTLE_MASK;
  BitBoard queenCastleMask     = (board->turn == WHITE) ? WHITE_QUEEN_CASTLE_MASK : BLACK_QUEEN_CASTLE_MASK;
  BitBoard queenRookCastleMask = (board->turn == WHITE) ? WHITE_QUEEN_ROOK_CASTLE_MASK : BLACK_QUEEN_ROOK_CASTLE_MASK;

  if (kingCastleAvailable && ((kingCastleMask & enemyAttacks) == 0) && ((kingRookCastleMask & occupancy) == 0)) {
    PositionIndex src = __builtin_ctzll(pieceBitboards[KING]);
    *(*movePtr)++     = MOVE(MOVE_KING_CASTLE, src + 2, src);
  }
  if (queenCastleAvailable && ((queenCastleMask & enemyAttacks) == 0) && ((queenRookCastleMask & occupancy) == 0)) {
    PositionIndex src = __builtin_ctzll(pieceBitboards[KING]);
    *(*movePtr)++     = MOVE(MOVE_QUEEN_CASTLE, src - 2, src);
  }

  BitBoard kings = pieceBitboards[KING];
  while (kings) {
    PositionIndex src     = getLSB(&kings);
    BitBoard moveBitboard = kingAttacks[src] & ~friendlyPieces;
    pushMoves(enemyKing, enemyPieces, src, &moveBitboard, movePtr);
  }

  BitBoard queens = pieceBitboards[QUEEN];
  while (queens) {
    PositionIndex src     = getLSB(&queens);
    BitBoard moveBitboard = (rookMoves(src, occupancy) | bishopMoves(src, occupancy)) & ~friendlyPieces;
    pushMoves(enemyKing, enemyPieces, src, &moveBitboard, movePtr);
  }

  BitBoard rooks = pieceBitboards[ROOK];
  while (rooks) {
    PositionIndex src     = getLSB(&rooks);
    BitBoard moveBitboard = rookMoves(src, occupancy) & ~friendlyPieces;
    pushMoves(enemyKing, enemyPieces, src, &moveBitboard, movePtr);
  }

  BitBoard bishops = pieceBitboards[BISHOP];
  while (bishops) {
    PositionIndex src     = getLSB(&bishops);
    BitBoard moveBitboard = bishopMoves(src, occupancy) & ~friendlyPieces;
    pushMoves(enemyKing, enemyPieces, src, &moveBitboard, movePtr);
  }

  BitBoard knights = pieceBitboards[KNIGHT];
  while (knights) {
    PositionIndex src     = getLSB(&knights);
    BitBoard moveBitboard = knightAttacks[src] & ~friendlyPieces;
    pushMoves(enemyKing, enemyPieces, src, &moveBitboard, movePtr);
  }

  BitBoard pawns = pieceBitboards[PAWN];
  while (pawns) {
    PositionIndex src = getLSB(&pawns);

    BitBoard attacks = pawnAttacks[board->turn][src] & enemyPieces;

    if (board->epSquare != NAP) {
      BitBoard ep = POSITION_BIT(board->epSquare);
      if (pawnAttacks[board->turn][src] & ep) {
        attacks |= ep;
      }
    }

    while (attacks) {
      PositionIndex dst = getLSB(&attacks);
      int rank          = INDEX_RANK(dst);
      if (dst == board->epSquare) {
        *(*movePtr)++ = MOVE(MOVE_CAPTURE | MOVE_EP, dst, src);
      } else if ((board->turn == WHITE && rank == MAX_RANK) || (board->turn == BLACK && rank == 0)) {
        *(*movePtr)++ = MOVE(MOVE_CAPTURE | MOVE_QUEEN_PROMOTION, dst, src);
        *(*movePtr)++ = MOVE(MOVE_CAPTURE | MOVE_ROOK_PROMOTION, dst, src);
        *(*movePtr)++ = MOVE(MOVE_CAPTURE | MOVE_BISHOP_PROMOTION, dst, src);
        *(*movePtr)++ = MOVE(MOVE_CAPTURE | MOVE_KNIGHT_PROMOTION, dst, src);
      } else {
        *(*movePtr)++ = MOVE(MOVE_CAPTURE, dst, src);
      }
    }

    BitBoard moves = pawnSingleMoves[board->turn][src] & ~occupancy;
    moves |= moves ? pawnDoubleMoves[board->turn][src] & ~occupancy : (BitBoard)0;
    while (moves) {
      PositionIndex dst = getLSB(&moves);
      int rank          = INDEX_RANK(dst);
      if ((board->turn == WHITE && rank == MAX_RANK) || (board->turn == BLACK && rank == 0)) {
        *(*movePtr)++ = MOVE(MOVE_QUEEN_PROMOTION, dst, src);
        *(*movePtr)++ = MOVE(MOVE_ROOK_PROMOTION, dst, src);
        *(*movePtr)++ = MOVE(MOVE_BISHOP_PROMOTION, dst, src);
        *(*movePtr)++ = MOVE(MOVE_KNIGHT_PROMOTION, dst, src);
      } else if (abs(dst - src) == (NUM_FILES * 2)) {
        *(*movePtr)++ = MOVE(MOVE_DOUBLE_PUSH, dst, src);
      } else {
        *(*movePtr)++ = MOVE(MOVE_QUIET, dst, src);
      }
    }
  }
}

unsigned int generateLegalMoves(Board* board, Move* moves) {
  Move pseudoLegalMoves[MAX_CHESS_MOVES];
  Move* movePtr = pseudoLegalMoves;
  generatePseudoLegalMoves(board, &movePtr);

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
