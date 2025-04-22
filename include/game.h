/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "fen.h"

typedef struct {
  Position src;
  Position dest;
} Move;

typedef struct {
  int activeColor;
  Board board;
} Game;

void initGame(Game *game, Fen *fen);

#endif  // GAME_H
