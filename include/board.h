/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdint.h>

#include "defs.h"

typedef enum {
  PIECE_NULL = 0,
  WK,
  WQ,
  WR,
  WB,
  WN,
  WP,
  BK,
  BQ,
  BR,
  BB,
  BN,
  BP,
  PIECE_END,
} Piece;

typedef enum {
  KING = 0,
  QUEEN,
  ROOK,
  BISHOP,
  KNIGHT,
  PAWN,
  NUM_PIECE_TYPES,
} PieceBoardIndex;

// https://www.chessprogramming.org/Encoding_Moves#MoveIndex
// R6R/3Q4/1Q4Q1/4Q3/2Q4Q/Q4Q2/pp1Q4/kBNN1KB1 w - - 0 1
// 3Q4/1Q4Q1/4Q3/2Q4R/Q4Q2/3Q4/1Q4Rp/1K1BBNNk w - - 0 1
#define MAX_CHESS_MOVES 218

// 2 source
// 2 destination
// 1 Capture
// 1 Check
// 1 Null Termination
#define MOVE_STRING_SIZE 7

// Field: [   FLAG    ][     DST     ][    SRC    ]
// Bits:  [15 14 13 12][11 10 9 8 7 6][5 4 3 2 1 0]
typedef uint16_t Move;

// Maximum number of unique flags 15 (0b1111)
typedef enum {
  QUIET = 0,
  KING_CASTLE,
  QUEEN_CASTLE,
  QUEEN_PROMOTION,
  ROOK_PROMOTION,
  BISHOP_PROMOTION,
  KNIGHT_PROMOTION,
  CAPTURE,
  CHECK,
  CHECKMATE,
} MoveFlag;

#define _6BIT_MASK 0x3F
#define _4BIT_MASK 0x0F

#define SRC(move) (PositionIndex)((move) & _6BIT_MASK)
#define DST(move) (PositionIndex)(((move) >> 6) & _6BIT_MASK)
#define FLAG(move) (MoveFlag)(((move) >> 12) & _4BIT_MASK)
#define MOVE(flag, dst, src) (Move)((((flag) & _4BIT_MASK) << 12) | (((dst) & _6BIT_MASK) << 6) | ((src) & _6BIT_MASK))

typedef enum {
  CASTLE_WHITE_KING  = 0x1,
  CASTLE_WHITE_QUEEN = 0x2,
  CASTLE_BLACK_KING  = 0x4,
  CASTLE_BLACK_QUEEN = 0x8,
} CastlingAvailability;

typedef struct {
  Move move;
  Piece captured;
  CastlingAvailability castlingAvailability;
} UndoMove;

typedef struct {
  Piece squares[NUM_POSITIONS];

  Color turn;
  CastlingAvailability castlingAvailability;

  // Bitboards
  BitBoard whites;
  BitBoard blacks;

  BitBoard enemyAttacks;

  BitBoard whitePieceBoards[NUM_PIECE_TYPES];
  BitBoard blackPieceBoards[NUM_PIECE_TYPES];

} Board;

void displayBoard(const Board* board);
void displayBitBoard(const uint64_t bitboard);

bool moveCheck(const Move move);
bool moveCastle(const Move move);
bool moveCapture(const Move move);
bool moveCheckmate(const Move move);
bool movePromotion(const Move move);

void initBoard(Board* board);
void boardSetPiece(Board* board, const PositionIndex position, const Piece piece);
Piece boardGetPiece(const Board* board, const PositionIndex position);
void boardMakeMove(Board* board, Move move, UndoMove* undo);
void boardUnmakeMove(Board* board, const UndoMove* undo);
void boardSetTurnColor(Board* board, Color turnColor);
void boardSetCastlingAvailability(Board* board, CastlingAvailability castlingAvailability);
bool kingInCheck(Board* board, Color side);
unsigned int generateLegalMoves(Board* board, Move* moves);

#endif  // BOARD_H
