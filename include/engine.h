/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"

typedef struct {
  int score;
  Move move;
} ScoredMove;

int search(Board* board, unsigned int depth, Move* bestMove, int alpha, int beta);

#endif  // ENGINE_H
