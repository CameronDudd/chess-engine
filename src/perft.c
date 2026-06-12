/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include "perft.h"

#include <stddef.h>

#include "board.h"

unsigned int perft(Board* board, int depth) {
  if (depth == 0) return 1;

  unsigned int totalNodes = 0;
  Move moves[MAX_CHESS_MOVES];

  unsigned int numMoves = generateLegalMoves(board, moves);

  for (unsigned int i = 0; i < numMoves; ++i) {
    UndoMove undo;
    boardMakeMove(board, moves[i], &undo);
    totalNodes += perft(board, depth - 1);
    boardUnmakeMove(board, &undo);
  }

  return totalNodes;
}
