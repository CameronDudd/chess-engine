/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "perft.h"

#include <stdio.h>
#include <stdlib.h>

struct PerftRes perft(Board board, int activeColor, int depth,
                      Move previousMove) {
  MoveList moveList;
  struct PerftRes result = {depth, 0, 0, 0, 0, 0};
  if (depth == 0) {
    result.nodes = 1;
    return result;
  }
  generateLegalMoves(&moveList, board, activeColor, previousMove);
  for (size_t i = 0; i < moveList.count; ++i) {
    const Move move = moveList.moves[i];
    const uint8_t flags = MOVE_FLAGS(move);
    if (flags & CASTLE) {
      result.castles += 1;
    } else if (flags & CAPTURE) {
      result.captures += 1;
    } else if (flags & KING_CHECK) {
      result.checks += 1;
    } else if (flags & KING_CHECKMATE) {
      result.checkMates += 1;
    }
    doMove(board, move);
    struct PerftRes recRes = perft(board, !activeColor, depth - 1, move);
    result.nodes += recRes.nodes;
    result.captures += recRes.captures;
    result.castles += recRes.castles;
    result.checks += recRes.checks;
    result.checkMates += recRes.checkMates;
    undoMove(board, move);
  }
  return result;
}
