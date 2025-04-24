/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "game.h"
#include "fen.h"

void initGame(Game *game, Fen *fen) {
  game->state = 0;                // Reset state
  if (fenActiveColor(fen) == 1) { // White to move
    game->state |= (1 << ACTIVE_COLOR_BIT);
  }
  game->state |= fenCastlingBits(fen);
  fenPopulateBoard(fen, game->board);
}

uint8_t activeTurn(Game *game) {
  return ((game->state & (1 << ACTIVE_COLOR_BIT)) == 0) ? 0 : 1;
}
