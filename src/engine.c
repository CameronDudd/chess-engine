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
    int randIdx = ((float)rand() * (float)numMoves) / (float)RAND_MAX;
    return moves[randIdx];
  }
  return (Move)0;
}
