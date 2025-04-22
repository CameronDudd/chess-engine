/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "game.h"
#include "fen.h"

void initGame(Game *game, Fen *fen) {
  game->activeColor = fenActiveColor(fen);
  fenPopulateBoard(fen, game->board);
}
