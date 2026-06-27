/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>
#include <stdint.h>

#include "board.h"

#define MATE 9999999
#define STALEMATE 500

typedef struct {
  int score;
  Move move;
} ScoredMove;

typedef struct {
  int score;
  Move move;
  uint64_t nodes;
  uint64_t qnodes;
} SearchResult;

typedef struct {
  uint64_t endTimeNs;
  bool searchInterrupted;
} SearchTimer;

SearchResult iterativeSearch(Board* board, unsigned int depthLimit, unsigned int timeLimitMs);

#endif  // ENGINE_H
