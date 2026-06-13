/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include "perft.h"

#include <stddef.h>

#include "board.h"

bool bulk = false;

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
  if (depth == 0) return (PerftResult){.nodes = 1};

  Move moves[MAX_CHESS_MOVES];
  unsigned int numMoves = generateLegalMoves(board, moves);
  bool isCheckmate      = (numMoves == 0 && kingInCheck(board, board->turn));

  PerftResult result = {0};

  if (isCheckmate) {
    result.checkmates = 1;
    return result;
  }

  if (bulk && depth == 1) {
    result.nodes = numMoves;
    return result;
  }

  if (depth == 1) {
    result.nodes = numMoves;

    for (unsigned int i = 0; i < numMoves; ++i) {
      Move move = moves[i];
      if (moveCastle(move)) result.castles += 1;
      if (moveCapture(move)) result.captures += 1;
      if (movePromotion(move)) result.promotions += 1;

      UndoMove undo;
      boardMakeMove(board, move, &undo);

      if (kingInCheck(board, board->turn)) {
        result.checks += 1;

        Move tmp[MAX_CHESS_MOVES];
        if (generateLegalMoves(board, tmp) == 0) {
          result.checkmates += 1;
        }
      }

      boardUnmakeMove(board, &undo);
    }

    return result;
  }

  for (unsigned int i = 0; i < numMoves; ++i) {
    UndoMove undo;
    boardMakeMove(board, moves[i], &undo);
    addPerft(&result, perft(board, depth - 1));
    boardUnmakeMove(board, &undo);
  }

  return result;
}
