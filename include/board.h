/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#ifndef BOARD_H
#define BOARD_H

#include <stddef.h>
#include <stdint.h>

#include "piece.h"

#define ROWS                8
#define COLS                8
#define NUM_DIRECTIONS      8
#define NUM_PAWN_DIRECTIONS 6
#define NUM_POSITIONS       64
// https://www.chessprogramming.org/Encoding_Moves#MoveIndex
// R6R/3Q4/1Q4Q1/4Q3/2Q4Q/Q4Q2/pp1Q4/kBNN1KB1 w - - 0 1
// 3Q4/1Q4Q1/4Q3/2Q4R/Q4Q2/3Q4/1Q4Rp/1K1BBNNk w - - 0 1
#define MAX_CHESS_MOVES 219  // + 1 for array allocation

// 2 source
// 2 destination
// 1 Capture
// 1 Check
#define MOVE_STRING_MAX 7

typedef int Position;  // 0 top-left to 63 bottom-right
typedef uint64_t BitBoard;
typedef Piece Board[NUM_POSITIONS];

typedef enum {
  CASTLE     = 1,  // 01
  CAPTURE    = 2,  // 10
  EN_PASSANT = 3,  // 11
} MoveFlag;

typedef enum {
  CHECK     = 1,  // 01
  CHECKMATE = 2,  // 10
  // RESERVED = 3, // 11
} CheckFlag;

// Field:  CHECK  FLAG   DST            SRC
// Bits:  [15 14][13 12][11 10 9 8 7 6][5 4 3 2 1 0]
typedef uint16_t Move;
#define _2BIT_MASK 0x03  // 0b00000011
#define _4BIT_MASK 0x0F  // 0b00001111
#define _6BIT_MASK 0x3F  // 0b00111111
#define MOVE_ENCODE(src, dst, flag, check) \
  (uint16_t)((((check) & _2BIT_MASK) << 14) | (((flag) & _2BIT_MASK) << 12) | (((dst) & _6BIT_MASK) << 6) | ((src) & _6BIT_MASK))
#define MOVE_SRC(move)   (uint8_t)((move) & _6BIT_MASK)
#define MOVE_DST(move)   (uint8_t)(((move) >> 6) & _6BIT_MASK)
#define MOVE_FLAG(move)  (uint8_t)(((move) >> 12) & _2BIT_MASK)
#define MOVE_CHECK(move) (uint8_t)(((move) >> 14) & _2BIT_MASK)

typedef enum {
  FORWARD        = -8,
  FORWARD_RIGHT  = -7,
  RIGHT          = 1,
  BACKWARD_RIGHT = 9,
  BACKWARD       = 8,
  BACKWARD_LEFT  = 7,
  LEFT           = -1,
  FORWARD_LEFT   = -9,
} Direction;

typedef struct {
  Move moves[MAX_CHESS_MOVES];
  size_t count;
} MoveList;

extern BitBoard pieceBitBoards[NUM_UNIQUE_PIECES];
extern int distanceToEdgeLookup[NUM_POSITIONS][NUM_DIRECTIONS];

Position coordinate2pos(const char coordinate[]);
void clearBoard(Board board);
void initBitBoards(Board board);
void initDistanceToEdgeLookup();
void doMove(Board board, const Move move);
void undoMove(Board board, const Move move);
Move strMakeMove(Board board, const char *moveStr);
void generateLegalMoves(MoveList *moveList, Board board, int activeColor, Move previousMove);

const char *move2str(const Move move, const Board board);

#endif  // BOARD_H
