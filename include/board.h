/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#include "piece.h"

#define ROWS          8
#define COLS          8
#define NUM_POSITIONS 64

typedef int Position;  // 0 top-left to 63 bottom-right
typedef uint64_t BitBoard;
typedef Piece Board[NUM_POSITIONS];

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
  Position src;
  Position dst;
} Move;

extern BitBoard pieceBitBoards[NUM_UNIQUE_PIECES];

void clearBoard(Board board);
void initBitBoards(Board board);
void makeMove(Board board, const Move *move);
void strMakeMove(Board board, const char *moveStr);

#endif  // BOARD_H
