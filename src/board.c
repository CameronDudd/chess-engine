/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include "board.h"

#include <log.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "magics.h"

#define ESC "\x1b"
#define COLOR_CLEAR ESC "[0m"

// background;24-bit;r;g;b;execute
#define _24_BIT_BLACK_PIECE ESC "[38;2;0;0;0m"
#define _24_BIT_WHITE_PIECE ESC "[38;2;255;255;255m"
#define _24_BIT_BLACK_SQUARE ESC "[48;2;153;76;0m"
#define _24_BIT_WHITE_SQUARE ESC "[48;2;255;204;153m"

#define BORDER "+---------------+"

#define PIECE_WHITE(piece) ((PIECE_NULL < (piece)) && ((piece) <= WP))

static CastlingAvailability castlingAvailabilityLookup[NUM_POSITIONS];

static ZobristHash zobristPositionHashes[PIECE_END][NUM_POSITIONS];
static ZobristHash zobristBlackToMoveHash;
static ZobristHash zobristCastlingHashes[CAN_CASTLE_MAX_NUM];
static ZobristHash zobristEnPassantFileHashes[NUM_FILES];

static void pushMoves(PositionIndex src, BitBoard* moves, Move** movePtr) {
  while (*moves) {
    PositionIndex dst = popLSB(moves);
    *(*movePtr)++     = MOVE(MOVE_META_QUIET, MOVE_FLAG_QUIET, dst, src);
  }
}

static const char* pieceCharacterLookup[PIECE_END] = {" ", "♚", "♛", "♜", "♝", "♞", "♟", "♚", "♛", "♜", "♝", "♞", "♟"};

static const char* pieceStr(const Piece piece) {
  if ((piece < 0) || (piece >= PIECE_END)) return "?";
  return pieceCharacterLookup[piece];
}

void displayMove(Move move) {
  PositionIndex src = SRC(move);
  PositionIndex dst = DST(move);
  printf("%c%c -> %c%c\r\n", 'a' + INDEX_FILE(src), '1' + INDEX_RANK(src), 'a' + INDEX_FILE(dst), '1' + INDEX_RANK(dst));
}

void displayBoard(const Board* board) {
  for (int rank = MAX_RANK; rank >= 0; --rank) {
    printf("\r\n%c ", '1' + rank);
    for (int file = 0; file < NUM_FILES; ++file) {
      PositionIndex i = POS_INDEX(rank, file);
      const char* fg  = PIECE_WHITE(board->squares[i]) ? _24_BIT_WHITE_PIECE : _24_BIT_BLACK_PIECE;
      const char* bg  = ((rank + file) % 2) ? _24_BIT_WHITE_SQUARE : _24_BIT_BLACK_SQUARE;
      printf("%s%s%s %s", fg, bg, pieceStr(board->squares[i]), COLOR_CLEAR);
    }
  }
  printf("\r\n  ");
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
  return FLAG(move) == MOVE_FLAG_CASTLE;
}

bool moveCapture(Board* board, Move move) {
  PositionIndex dst = DST(move);
  MoveFlag flag     = FLAG(move);
  return ((flag == MOVE_FLAG_EP) || (board->squares[dst] != PIECE_NULL));
}

bool movePromotion(Move move) {
  return FLAG(move) == MOVE_FLAG_PROMOTION;
}

bool moveEP(Move move) {
  return (FLAG(move) == MOVE_FLAG_EP);
}

static void initCastlingAvailability(void) {
  for (PositionIndex i = 0; i < NUM_POSITIONS; ++i) castlingAvailabilityLookup[i] = NAP;
  castlingAvailabilityLookup[A1] = ~CASTLE_WHITE_QUEEN;
  castlingAvailabilityLookup[E1] = ~(CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN);
  castlingAvailabilityLookup[H1] = ~CASTLE_WHITE_KING;
  castlingAvailabilityLookup[A8] = ~CASTLE_BLACK_QUEEN;
  castlingAvailabilityLookup[E8] = ~(CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN);
  castlingAvailabilityLookup[H8] = ~CASTLE_BLACK_KING;
}

static void initZobristHashes(void) {
  for (unsigned int i = 0; i < NUM_PIECE_TYPES * 2; ++i) {
    for (unsigned int j = 0; j < NUM_POSITIONS; ++j) {
      zobristPositionHashes[i][j] = sparsexorshift64();
    }
  }
  zobristBlackToMoveHash = sparsexorshift64();
  for (unsigned int i = 0; i < CAN_CASTLE_MAX_NUM; ++i) zobristCastlingHashes[i] = sparsexorshift64();
  for (unsigned int i = 0; i < NUM_FILES; ++i) zobristEnPassantFileHashes[i] = sparsexorshift64();
}

void initBoard(Board* board) {
  // Zero out the board
  for (int i = 0; i < NUM_POSITIONS; ++i) board->squares[i] = (Piece)0;
  board->turn                 = WHITE;
  board->castlingAvailability = 0;
  board->epSquare             = NAP;
  board->whites               = (BitBoard)0;
  board->blacks               = (BitBoard)0;
  board->zobrist              = (ZobristHash)0;
  for (int i = 0; i < NUM_PIECE_TYPES; ++i) board->whitePieceBoards[i] = (BitBoard)0;
  for (int i = 0; i < NUM_PIECE_TYPES; ++i) board->blackPieceBoards[i] = (BitBoard)0;

  initMagicAttacks();
  initZobristHashes();
  initCastlingAvailability();
}

void initBoardZobrist(Board* board) {
  board->zobrist = (ZobristHash)0;

  for (PositionIndex p = 0; p < NUM_POSITIONS; ++p) {
    Piece piece = board->squares[p];
    if (piece != PIECE_NULL) board->zobrist ^= zobristPositionHashes[piece][p];
  }

  if (board->turn == BLACK) board->zobrist ^= zobristBlackToMoveHash;

  board->zobrist ^= zobristCastlingHashes[board->castlingAvailability];

  if (board->epSquare != NAP) board->zobrist ^= zobristEnPassantFileHashes[INDEX_FILE(board->epSquare)];
}

void boardSetPiece(Board* board, PositionIndex position, Piece piece) {
  if (piece == PIECE_NULL) return;

  BitBoard positionBit = POSITION_BIT(position);

  if (PIECE_WHITE(piece)) {
    board->whites |= positionBit;
    board->whitePieceBoards[piece - WK] |= positionBit;
  } else {
    board->blacks |= positionBit;
    board->blackPieceBoards[piece - BK] |= positionBit;
  }

  board->squares[position] = piece;
  board->zobrist ^= zobristPositionHashes[piece][position];
}

static void boardRemovePiece(Board* board, const PositionIndex position, const Piece piece) {
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
  board->zobrist ^= zobristPositionHashes[piece][position];
}

void boardMakeMove(Board* board, Move move, UndoMove* undo) {  // NOLINT(readability-function-cognitive-complexity)
  PositionIndex src = SRC(move);
  PositionIndex dst = DST(move);
  MoveFlag flag     = FLAG(move);
  MoveMeta meta     = META(move);

  Piece piece  = board->squares[src];
  Piece target = board->squares[dst];

  undo->move                 = move;
  undo->captured             = target;
  undo->castlingAvailability = board->castlingAvailability;
  undo->epSquare             = board->epSquare;
  undo->epCapturedSquare     = NAP;
  undo->zobrist              = board->zobrist;

  if (board->epSquare != NAP) board->zobrist ^= zobristEnPassantFileHashes[INDEX_FILE(board->epSquare)];
  board->epSquare = NAP;

  if (target != PIECE_NULL) boardRemovePiece(board, dst, target);

  boardRemovePiece(board, src, piece);
  boardSetPiece(board, dst, piece);

  board->zobrist ^= zobristCastlingHashes[board->castlingAvailability];
  board->castlingAvailability &= castlingAvailabilityLookup[src];
  board->castlingAvailability &= castlingAvailabilityLookup[dst];
  board->zobrist ^= zobristCastlingHashes[board->castlingAvailability];

  if ((piece == WK || piece == BK) && (flag == MOVE_FLAG_CASTLE)) {
    Piece rook = (board->turn == WHITE) ? WR : BR;
    if (meta == MOVE_META_CASTLE_KINGSIDE) {
      boardRemovePiece(board, dst + 1, rook);
      boardSetPiece(board, src + 1, rook);
    } else if (meta == MOVE_META_CASTLE_QUEENSIDE) {
      boardRemovePiece(board, dst - 2, rook);
      boardSetPiece(board, dst + 1, rook);
    }
  }

  if (piece == WP || piece == BP) {
    if (abs(dst - src) == (NUM_FILES * 2)) {
      board->epSquare = (src + dst) / 2;
      board->zobrist ^= zobristEnPassantFileHashes[INDEX_FILE(board->epSquare)];
    }
    if (flag == MOVE_FLAG_EP) {
      undo->epCapturedSquare = (board->turn == WHITE) ? dst - NUM_FILES : dst + NUM_FILES;
      boardRemovePiece(board, undo->epCapturedSquare, (board->turn == WHITE) ? BP : WP);
    }
    if (flag == MOVE_FLAG_PROMOTION) {
      boardRemovePiece(board, dst, piece);
      if (meta == MOVE_META_PROMOTION_QUEEN) boardSetPiece(board, dst, (board->turn == WHITE) ? WQ : BQ);
      if (meta == MOVE_META_PROMOTION_ROOK) boardSetPiece(board, dst, (board->turn == WHITE) ? WR : BR);
      if (meta == MOVE_META_PROMOTION_BISHOP) boardSetPiece(board, dst, (board->turn == WHITE) ? WB : BB);
      if (meta == MOVE_META_PROMOTION_KNIGHT) boardSetPiece(board, dst, (board->turn == WHITE) ? WN : BN);
    }
  }

  board->zobrist ^= zobristBlackToMoveHash;
  board->turn = (board->turn == WHITE) ? BLACK : WHITE;
}

void boardUnmakeMove(Board* board, const UndoMove* undo) {
  Move move = undo->move;

  PositionIndex src = SRC(move);
  PositionIndex dst = DST(move);
  MoveFlag flag     = FLAG(move);
  MoveMeta meta     = META(move);

  Piece piece = board->squares[dst];

  boardRemovePiece(board, dst, piece);

  if (flag == MOVE_FLAG_PROMOTION) {
    piece = (board->turn == WHITE) ? BP : WP;
  }

  boardSetPiece(board, src, piece);

  if (undo->captured != PIECE_NULL) boardSetPiece(board, dst, undo->captured);

  if (flag == MOVE_FLAG_CASTLE) {
    Piece rook = (board->turn == WHITE) ? BR : WR;
    if (meta == MOVE_META_CASTLE_KINGSIDE) {
      boardRemovePiece(board, src + 1, rook);
      boardSetPiece(board, dst + 1, rook);
    } else if (meta == MOVE_META_CASTLE_QUEENSIDE) {
      boardRemovePiece(board, dst + 1, rook);
      boardSetPiece(board, dst - 2, rook);
    }
  } else if (flag == MOVE_FLAG_EP) {
    boardSetPiece(board, undo->epCapturedSquare, board->turn == WHITE ? WP : BP);
  }

  board->castlingAvailability = undo->castlingAvailability;
  board->epSquare             = undo->epSquare;
  board->zobrist              = undo->zobrist;

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

static BitBoard rookMoves(PositionIndex pos, BitBoard occupancy) {
  occupancy &= rookMasks[pos];
  uint64_t magic = (occupancy * RookMagics[pos]) >> (NUM_POSITIONS - rookNumBits[pos]);
  return rookAttacks[pos][magic];
}

static BitBoard bishopMoves(PositionIndex pos, BitBoard occupancy) {
  occupancy &= bishopMasks[pos];
  uint64_t magic = (occupancy * BishopMagics[pos]) >> (NUM_POSITIONS - bishopNumBits[pos]);
  return bishopAttacks[pos][magic];
}

static BitBoard colorAttack(Board* board, Color side) {
  BitBoard attacks = (BitBoard)0;

  BitBoard occupancy = board->whites | board->blacks;

  BitBoard* pieceBitboards = (side == WHITE) ? board->whitePieceBoards : board->blackPieceBoards;

  BitBoard kings = pieceBitboards[KING];
  while (kings) {
    PositionIndex pos = popLSB(&kings);
    attacks |= kingAttacks[pos];
  }

  BitBoard queens = pieceBitboards[QUEEN];
  while (queens) {
    PositionIndex pos = popLSB(&queens);
    attacks |= rookMoves(pos, occupancy) | bishopMoves(pos, occupancy);
  }

  BitBoard rooks = pieceBitboards[ROOK];
  while (rooks) {
    PositionIndex pos = popLSB(&rooks);
    attacks |= rookMoves(pos, occupancy);
  }

  BitBoard bishops = pieceBitboards[BISHOP];
  while (bishops) {
    PositionIndex pos = popLSB(&bishops);
    attacks |= bishopMoves(pos, occupancy);
  }

  BitBoard knights = pieceBitboards[KNIGHT];
  while (knights) {
    PositionIndex pos = popLSB(&knights);
    attacks |= knightAttacks[pos];
  }

  BitBoard pawns = pieceBitboards[PAWN];
  while (pawns) {
    PositionIndex pos = popLSB(&pawns);
    attacks |= pawnAttacks[side][pos];
  }

  return attacks;
}

static bool colorAttacksSquare(Board* board, PositionIndex position, Color side) {
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

bool kingInCheck(Board* board, Color side) {
  if (side == WHITE) {
    return colorAttacksSquare(board, __builtin_ctzll(board->whitePieceBoards[KING]), BLACK);
  }
  return colorAttacksSquare(board, __builtin_ctzll(board->blackPieceBoards[KING]), WHITE);
}

static void generatePseudoLegalMoves(Board* board, Move** movePtr) {  // NOLINT(readability-function-cognitive-complexity)
  BitBoard* pieceBitboards = (board->turn == WHITE) ? board->whitePieceBoards : board->blackPieceBoards;
  BitBoard friendlyPieces  = (board->turn == WHITE) ? board->whites : board->blacks;
  BitBoard enemyPieces     = (board->turn == WHITE) ? board->blacks : board->whites;
  BitBoard occupancy       = board->whites | board->blacks;
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
    *(*movePtr)++     = MOVE(MOVE_META_CASTLE_KINGSIDE, MOVE_FLAG_CASTLE, src + 2, src);
  }
  if (queenCastleAvailable && ((queenCastleMask & enemyAttacks) == 0) && ((queenRookCastleMask & occupancy) == 0)) {
    PositionIndex src = __builtin_ctzll(pieceBitboards[KING]);
    *(*movePtr)++     = MOVE(MOVE_META_CASTLE_QUEENSIDE, MOVE_FLAG_CASTLE, src - 2, src);
  }

  BitBoard kings = pieceBitboards[KING];
  while (kings) {
    PositionIndex src     = popLSB(&kings);
    BitBoard moveBitboard = kingAttacks[src] & ~friendlyPieces;
    pushMoves(src, &moveBitboard, movePtr);
  }

  BitBoard queens = pieceBitboards[QUEEN];
  while (queens) {
    PositionIndex src     = popLSB(&queens);
    BitBoard moveBitboard = (rookMoves(src, occupancy) | bishopMoves(src, occupancy)) & ~friendlyPieces;
    pushMoves(src, &moveBitboard, movePtr);
  }

  BitBoard rooks = pieceBitboards[ROOK];
  while (rooks) {
    PositionIndex src     = popLSB(&rooks);
    BitBoard moveBitboard = rookMoves(src, occupancy) & ~friendlyPieces;
    pushMoves(src, &moveBitboard, movePtr);
  }

  BitBoard bishops = pieceBitboards[BISHOP];
  while (bishops) {
    PositionIndex src     = popLSB(&bishops);
    BitBoard moveBitboard = bishopMoves(src, occupancy) & ~friendlyPieces;
    pushMoves(src, &moveBitboard, movePtr);
  }

  BitBoard knights = pieceBitboards[KNIGHT];
  while (knights) {
    PositionIndex src     = popLSB(&knights);
    BitBoard moveBitboard = knightAttacks[src] & ~friendlyPieces;
    pushMoves(src, &moveBitboard, movePtr);
  }

  BitBoard pawns = pieceBitboards[PAWN];
  while (pawns) {
    PositionIndex src = popLSB(&pawns);

    BitBoard attacks = pawnAttacks[board->turn][src] & enemyPieces;

    if (board->epSquare != NAP) {
      BitBoard ep = POSITION_BIT(board->epSquare);
      if (pawnAttacks[board->turn][src] & ep) {
        attacks |= ep;
      }
    }

    while (attacks) {
      PositionIndex dst = popLSB(&attacks);
      int rank          = INDEX_RANK(dst);
      if (dst == board->epSquare) {
        *(*movePtr)++ = MOVE(MOVE_META_QUIET, MOVE_FLAG_EP, dst, src);
      } else if ((board->turn == WHITE && rank == MAX_RANK) || (board->turn == BLACK && rank == 0)) {
        *(*movePtr)++ = MOVE(MOVE_META_PROMOTION_QUEEN, MOVE_FLAG_PROMOTION, dst, src);
        *(*movePtr)++ = MOVE(MOVE_META_PROMOTION_ROOK, MOVE_FLAG_PROMOTION, dst, src);
        *(*movePtr)++ = MOVE(MOVE_META_PROMOTION_BISHOP, MOVE_FLAG_PROMOTION, dst, src);
        *(*movePtr)++ = MOVE(MOVE_META_PROMOTION_KNIGHT, MOVE_FLAG_PROMOTION, dst, src);
      } else {
        *(*movePtr)++ = MOVE(MOVE_META_QUIET, MOVE_FLAG_QUIET, dst, src);
      }
    }

    BitBoard moves = pawnSingleMoves[board->turn][src] & ~occupancy;
    moves |= moves ? pawnDoubleMoves[board->turn][src] & ~occupancy : (BitBoard)0;
    while (moves) {
      PositionIndex dst = popLSB(&moves);
      int rank          = INDEX_RANK(dst);
      if ((board->turn == WHITE && rank == MAX_RANK) || (board->turn == BLACK && rank == 0)) {
        *(*movePtr)++ = MOVE(MOVE_META_PROMOTION_QUEEN, MOVE_FLAG_PROMOTION, dst, src);
        *(*movePtr)++ = MOVE(MOVE_META_PROMOTION_ROOK, MOVE_FLAG_PROMOTION, dst, src);
        *(*movePtr)++ = MOVE(MOVE_META_PROMOTION_BISHOP, MOVE_FLAG_PROMOTION, dst, src);
        *(*movePtr)++ = MOVE(MOVE_META_PROMOTION_KNIGHT, MOVE_FLAG_PROMOTION, dst, src);
      } else {
        *(*movePtr)++ = MOVE(MOVE_META_QUIET, MOVE_FLAG_QUIET, dst, src);
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

  unsigned int numMoves = 0;
  for (Move* rPtr = pseudoLegalMoves; rPtr < end; ++rPtr) {
    Color movingSide = board->turn;
    UndoMove undo;
    boardMakeMove(board, *rPtr, &undo);
    if (!kingInCheck(board, movingSide)) {
      if (wPtr != NULL) *wPtr++ = *rPtr;
      ++numMoves;
    }
    boardUnmakeMove(board, &undo);
  }

  return numMoves;
}

bool getLegalMove(Move* move, const Move moves[MAX_CHESS_MOVES], unsigned int numMoves) {
  for (unsigned int i = 0; i < numMoves; ++i) {
    Move fullMove = moves[i];
    if ((*move & MOVE_DST_SRC_MASK) == (fullMove & MOVE_DST_SRC_MASK)) {
      *move = fullMove;
      return true;
    }
  }
  return false;
}
