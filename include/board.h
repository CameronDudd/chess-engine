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

typedef uint8_t Position;  // 0 top-left to 63 bottom-right
typedef Piece Board[NUM_PIECES];

typedef struct {
  Position src;
  Position dst;
} Move;

void makeMove(Board board, const Move *move);
void strMakeMove(Board board, const char *moveStr);

#endif  // BOARD_H
