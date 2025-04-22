/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#include "piece.h"

#define NUM_PIECES 64

typedef uint8_t Position;
typedef Piece Board[NUM_PIECES];

#endif  // BOARD_H
