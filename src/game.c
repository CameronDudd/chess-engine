/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "game.h"

void initGame(Game *game, Fen *fen) {
  game->state = 0;                // Reset state
  if (fenActiveColor(fen) == 1) { // White to move
    game->state |= (1 << ACTIVE_COLOR_BIT);
  }
  game->state |= fenCastlingBits(fen);
  fenPopulateBoard(fen, game->board);
  game->previousMove = 0;
}

int activeTurn(Game *game) { return game->state & (1 << ACTIVE_COLOR_BIT); }
