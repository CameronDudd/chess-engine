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

#define BORDER "+---------------+"

static const char pieceCharacterLookup[PIECE_END] = {' ', 'K', 'Q', 'R', 'B', 'N', 'P', 'k', 'q', 'r', 'b', 'n', 'p'};

static char pieceChar(const Piece piece) {
  if ((piece < 0) || (piece >= PIECE_END)) return '?';
  return pieceCharacterLookup[piece];
}

void displayBoard(const Board* board) {
  printf("  %s\r\n", BORDER);
  for (int rank = 7; rank >= 0; --rank) {
    printf("%c ", 'a' + rank);
    for (int file = 0; file < 8; ++file) {
      PositionIndex i = POS_INDEX(rank, file);
      printf("|%c", pieceChar(board->squares[i]));
    }
    printf("|\r\n");
  }
  printf("  %s\r\n   ", BORDER);
  for (int file = 0; file < 8; ++file) {
    printf("%c ", '1' + file);
  }
  printf("\r\n");
}

void displayBitBoard(const BitBoard board) {
  printf("  %s\r\n", BORDER);
  for (int rank = 7; rank >= 0; --rank) {
    printf("%c ", 'a' + rank);
    for (int file = 0; file < 8; ++file) {
      PositionIndex i = POS_INDEX(rank, file);
      printf("|%c", (board & BIT_SQUARE(i)) ? '*' : ' ');
    }
    printf("|\r\n");
  }
  printf("  %s\r\n   ", BORDER);
  for (int file = 0; file < 8; ++file) {
    printf("%c ", '1' + file);
  }
  printf("\r\n");
}

void initBoard(Board* board) {
  // Zero out the board
  for (int i = 0; i < NUM_POSITIONS; ++i) board->squares[i] = (Piece)0;
  for (int i = 0; i < MAX_CHESS_MOVES; ++i) board->legalMoves[i] = (Move)0;
  board->turn                 = WHITE;
  board->castlingAvailability = 0;
  board->lastMove             = (Move)0;
  board->whites               = (BitBoard)0;
  board->blacks               = (BitBoard)0;
  board->enemyAttacks         = (BitBoard)0;
  for (int i = 0; i < NUM_PIECE_TYPES; ++i) board->whitePieces[i] = (BitBoard)0;
  for (int i = 0; i < NUM_PIECE_TYPES; ++i) board->blackPieces[i] = (BitBoard)0;
}

void boardSetPiece(Board* board, const PositionIndex position, const Piece piece) {
  if (PIECE_WHITE(piece)) {
    board->whites |= BIT_SQUARE(position);
    board->whitePieces[piece - WK] |= BIT_SQUARE(position);
  } else {
    board->blacks |= BIT_SQUARE(position);
    board->blackPieces[piece - BK] |= BIT_SQUARE(position);
  }
  board->squares[position] = piece;
}

void boardRemovePiece(Board* board, const PositionIndex position, const Piece piece) {
  if (PIECE_WHITE(piece)) {
    board->whites &= ~BIT_SQUARE(position);
    board->whitePieces[piece - WK] &= ~BIT_SQUARE(position);
  } else {
    board->blacks &= ~BIT_SQUARE(position);
    board->blackPieces[piece - BK] &= ~BIT_SQUARE(position);
  }
  board->squares[position] = PIECE_NULL;
}

Piece boardGetPiece(const Board* board, PositionIndex position) {
  return board->squares[position];
}

void boardMakeMove(Board* board, Move move, Piece* lastCapture) {
  PositionIndex src = SRC(move);
  PositionIndex dst = DST(move);

  Piece piece  = board->squares[src];
  Piece target = board->squares[dst];

  if (target != PIECE_NULL) boardRemovePiece(board, dst, target);
  if (lastCapture != NULL) *lastCapture = target;
  boardRemovePiece(board, src, piece);
  boardSetPiece(board, dst, piece);
}

void boardUnmakeMove(Board* board, Move move, Piece lastCapture) {
  PositionIndex src = SRC(move);
  PositionIndex dst = DST(move);

  Piece piece = board->squares[dst];

  boardRemovePiece(board, dst, piece);
  boardSetPiece(board, src, piece);

  if (lastCapture != PIECE_NULL) boardSetPiece(board, dst, lastCapture);
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

  BitBoard* pieceBitboards = (side == WHITE) ? board->whitePieces : board->blackPieces;

  BitBoard kings = pieceBitboards[WK - WK];
  while (kings) {
    PositionIndex pos = getLSB(&kings);
    attacks |= kingAttacks[pos];
  }

  BitBoard queens = pieceBitboards[WQ - WK];
  while (queens) {
    PositionIndex pos = getLSB(&queens);
    attacks |= rookMoves(pos, occupancy) | bishopMoves(pos, occupancy);
  }

  BitBoard rooks = pieceBitboards[WR - WK];
  while (rooks) {
    PositionIndex pos = getLSB(&rooks);
    attacks |= rookMoves(pos, occupancy);
  }

  BitBoard bishops = pieceBitboards[WB - WK];
  while (bishops) {
    PositionIndex pos = getLSB(&bishops);
    attacks |= bishopMoves(pos, occupancy);
  }

  BitBoard knights = pieceBitboards[WN - WK];
  while (knights) {
    PositionIndex pos = getLSB(&knights);
    attacks |= knightAttacks[pos];
  }

  BitBoard pawns = pieceBitboards[WP - WK];
  while (pawns) {
    PositionIndex pos = getLSB(&pawns);
    attacks |= pawnAttacks[side][pos];
  }

  return attacks;
}

bool kingInCheck(Board* board, Color side) {
  if (side == WHITE) {
    return (board->whitePieces[WK - WK] & colorAttack(board, BLACK)) != 0;
  }
  return (board->blackPieces[BK - BK] & colorAttack(board, WHITE)) != 0;
}

void generatePseudoLegalMoves(Board* board, Move** movePtr) {
  BitBoard* pieceBitboards = (board->turn == WHITE) ? board->whitePieces : board->blackPieces;
  BitBoard friendlyPieces  = (board->turn == WHITE) ? board->whites : board->blacks;
  BitBoard enemyPieces     = (board->turn == WHITE) ? board->blacks : board->whites;
  BitBoard occupancy       = board->whites | board->blacks;

  BitBoard kings = pieceBitboards[WK - WK];
  while (kings) {
    PositionIndex src     = getLSB(&kings);
    BitBoard moveBitboard = kingAttacks[src] & ~friendlyPieces;
    while (moveBitboard) {
      PositionIndex dst = getLSB(&moveBitboard);
      *(*movePtr)++     = MOVE(0, dst, src);
    }
  }

  BitBoard queens = pieceBitboards[WQ - WK];
  while (queens) {
    PositionIndex src     = getLSB(&queens);
    BitBoard moveBitboard = (rookMoves(src, occupancy) | bishopMoves(src, occupancy)) & ~friendlyPieces;
    while (moveBitboard) {
      PositionIndex dst = getLSB(&moveBitboard);
      *(*movePtr)++     = MOVE(0, dst, src);
    }
  }

  BitBoard rooks = pieceBitboards[WR - WK];
  while (rooks) {
    PositionIndex src     = getLSB(&rooks);
    BitBoard moveBitboard = rookMoves(src, occupancy) & ~friendlyPieces;
    while (moveBitboard) {
      PositionIndex dst = getLSB(&moveBitboard);
      *(*movePtr)++     = MOVE(0, dst, src);
    }
  }

  BitBoard bishops = pieceBitboards[WB - WK];
  while (bishops) {
    PositionIndex src     = getLSB(&bishops);
    BitBoard moveBitboard = bishopMoves(src, occupancy) & ~friendlyPieces;
    while (moveBitboard) {
      PositionIndex dst = getLSB(&moveBitboard);
      *(*movePtr)++     = MOVE(0, dst, src);
    }
  }

  BitBoard knights = pieceBitboards[WN - WK];
  while (knights) {
    PositionIndex src     = getLSB(&knights);
    BitBoard moveBitboard = knightAttacks[src] & ~friendlyPieces;
    while (moveBitboard) {
      PositionIndex dst = getLSB(&moveBitboard);
      *(*movePtr)++     = MOVE(0, dst, src);
    }
  }

  BitBoard pawns = pieceBitboards[WP - WK];
  while (pawns) {
    PositionIndex src     = getLSB(&pawns);
    BitBoard moveBitboard = pawnAttacks[board->turn][src] & enemyPieces;
    moveBitboard |= pawnMoves[board->turn][src] & ~friendlyPieces;
    // TODO:
    //  - [x] pushes
    //  - [x] double pushes
    //  - [~] promotions
    //  - [ ] en passant
    while (moveBitboard) {
      PositionIndex dst = getLSB(&moveBitboard);
      *(*movePtr)++     = MOVE(0, dst, src);
    }
  }
}

void generateLegalMoves(Board* board) {
  for (int i = 0; i < MAX_CHESS_MOVES; ++i) board->legalMoves[i] = (Move)0;

  Move pseudoLegalMoves[MAX_CHESS_MOVES] = {0};
  Move* movePtr                          = pseudoLegalMoves;
  generatePseudoLegalMoves(board, &movePtr);

  Move* end  = movePtr;
  Move* wPtr = board->legalMoves;
  for (Move* rPtr = pseudoLegalMoves; rPtr < end; ++rPtr) {
    Piece lastCapture = PIECE_NULL;
    boardMakeMove(board, *rPtr, &lastCapture);
    if (!kingInCheck(board, board->turn)) {
      *wPtr++ = *rPtr;
    }
    boardUnmakeMove(board, *rPtr, lastCapture);
  }

  printf("Num moves: %lu\r\n", wPtr - board->legalMoves);
}
