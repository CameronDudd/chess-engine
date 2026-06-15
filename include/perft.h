/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#ifndef PERFT_H
#define PERFT_H

#include <stdint.h>

#include "board.h"

typedef struct {
  unsigned long long nodes;
  unsigned long long captures;
  unsigned long long ep;
  unsigned long long castles;
  unsigned long long promotions;
  unsigned long long checks;
  unsigned long long checkmates;
} PerftResult;

PerftResult perft(Board* board, int depth, bool bulk);

#endif  // PERFT_H
