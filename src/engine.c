/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include "engine.h"

#include <stdlib.h>

#include "board.h"

Move engineBestMove(Board* board) {
  Move moves[MAX_CHESS_MOVES];
  unsigned int numMoves = generateLegalMoves(board, moves);
  if (numMoves > 0) {
    unsigned int randIdx = (unsigned int)(rand() % numMoves);  // NOLINT(cert-msc30-c,cert-msc50-cpp,concurrency-mt-unsafe)
    return moves[randIdx];
  }
  return (Move)0;
}
