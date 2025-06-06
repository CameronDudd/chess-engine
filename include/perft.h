/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#ifndef PERFT
#define PERFT

#include "board.h"

struct PerftRes {
  unsigned int depth;
  unsigned long long nodes;
  unsigned long captures;
  unsigned long castles;
  unsigned long checks;
  unsigned long checkMates;
};

struct PerftRes perft(Board board, int activeColor, int depth, Move previousMove);

#endif  // PERFT
