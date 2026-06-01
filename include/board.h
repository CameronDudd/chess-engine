/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#define N 8
#define NUM_POSITIONS 64  // N rows x N cols

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

#define CASTLE (uint8_t)0x01
#define CAPTURE (uint8_t)0x02
#define CHECK (uint8_t)0x04
#define CHECKMATE (uint8_t)0x8

#define _6BIT_MASK 0x3F
#define _4BIT_MASK 0x0F

#define SRC(move) (uint8_t)((move) & _6BIT_MASK)
#define DST(move) (uint8_t)(((move) >> 6) & _6BIT_MASK)
#define FLAGS(move) (uint8_t)(((move) >> 12) & _4BIT_MASK)
#define MOVE(flags, dst, src) (Move)((((flags) & _4BIT_MASK) << 12) | (((dst) & _6BIT_MASK) << 6) | ((src) & _6BIT_MASK))

typedef uint8_t PositionIndex;  // 0 bottom-left to 63 top-right
typedef uint8_t Piece;
typedef Piece Board[NUM_POSITIONS];

#endif  // BOARD_H
