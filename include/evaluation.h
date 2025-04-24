/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#ifndef EVALUATION_H
#define EVALUATION_H

#include "board.h"

typedef enum {
  NULL_VALUE   = 0,
  PAWN_VALUE   = 1,
  KNIGHT_VALUE = 3,
  BISHOP_VALUE = 3,
  ROOK_VALUE   = 5,
  QUEEN_VALUE  = 9,
} PieceValue;

int evaluate(Board board);

#endif  // EVALUATION_H
