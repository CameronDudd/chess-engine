/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#ifndef ENGINE_H
#define ENGINE_H

#define MATE 9999999
#define STALEMATE 0

#include <stdint.h>

#include "board.h"

typedef struct {
  int score;
  Move move;
} ScoredMove;

typedef struct {
  int score;
  uint64_t nodes;
  uint64_t qnodes;
} SearchResult;

int search(Board* board, int ply, unsigned int depth, Move* bestMove, int alpha, int beta, SearchResult* result);

#endif  // ENGINE_H
