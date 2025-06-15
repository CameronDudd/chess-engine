/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "game.h"

#include "board.h"

void initGame(Game *game, Fen *fen) {
  game->state = 0;                 // Reset state
  if (fenActiveColor(fen) == 1) {  // White to move
    game->state |= (1 << ACTIVE_COLOR_BIT);
  }
  game->state |= fenCastlingBits(fen);
  fenPopulateBoard(fen, game->board);
  game->legalMoves.count    = 0;
  game->previousMoves.count = 0;
  for (size_t i = 0; i < MAX_CHESS_MOVES; ++i) {
    game->legalMoves.moves[i]    = 0;
    game->previousMoves.moves[i] = 0;
  }
}

int activeColor(Game *game) { return game->state & (1 << ACTIVE_COLOR_BIT); }
