/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#ifndef PERFT_H
#define PERFT_H

#include <stdint.h>

#include "board.h"

typedef struct {
  unsigned int nodes;
  unsigned int captures;
  unsigned int ep;
  unsigned int castles;
  unsigned int promotions;
  unsigned int checks;
  unsigned int checkmates;
} PerftResult;

PerftResult perft(Board* board, int depth);

#endif  // PERFT_H
