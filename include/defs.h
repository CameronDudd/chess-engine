/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>

#define NUM_POSITIONS 64  // 8 rows x 8 cols

#define POS_INDEX(rank, file) (((rank) * 8) + (file))
#define INDEX_RANK(index) ((index) / 8)
#define INDEX_FILE(index) ((index) % 8)
#define BIT_SQUARE(square) (((uint64_t)1) << (square))

typedef uint8_t PositionIndex;  // 0 bottom-left to 63 top-right
typedef uint64_t BitBoard;

typedef enum {
  WHITE = 0,
  BLACK = 1,
} Color;

#endif  // DEFS_H
