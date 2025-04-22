/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#include "piece.h"

#define ROWS       8
#define COLS       8
#define NUM_PIECES 64

typedef uint8_t Position;
typedef Piece Board[NUM_PIECES];

#endif  // BOARD_H
