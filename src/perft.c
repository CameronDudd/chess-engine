/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include "perft.h"

#include <stddef.h>

#include "board.h"

void addPerft(PerftResult* a, const PerftResult b) {
  a->nodes += b.nodes;
  a->captures += b.captures;
  a->ep += b.ep;
  a->castles += b.castles;
  a->promotions += b.promotions;
  a->checks += b.checks;
  a->checkmates += b.checkmates;
}

PerftResult perft(Board* board, int depth) {
  if (depth == 0)
    return (PerftResult){
        .nodes      = 1,
        .captures   = 0,
        .ep         = 0,
        .castles    = 0,
        .promotions = 0,
        .checks     = 0,
        .checkmates = 0,
    };
  ;

  PerftResult result = {
      .nodes      = 0,
      .captures   = 0,
      .ep         = 0,
      .castles    = 0,
      .promotions = 0,
      .checks     = 0,
      .checkmates = 0,
  };

  Move moves[MAX_CHESS_MOVES];

  unsigned int numMoves = generateLegalMoves(board, moves);

  for (unsigned int i = 0; i < numMoves; ++i) {
    Move move = moves[i];

    if (moveCheck(move)) {
      result.checks += 1;
    } else if (moveCastle(move)) {
      result.castles += 1;
    } else if (moveCapture(move)) {
      result.captures += 1;
    } else if (moveCheckmate(move)) {
      result.checkmates += 1;
    } else if (movePromotion(move)) {
      result.promotions += 1;
    }

    UndoMove undo;
    boardMakeMove(board, move, &undo);
    addPerft(&result, perft(board, depth - 1));
    boardUnmakeMove(board, &undo);
  }

  return result;
}
