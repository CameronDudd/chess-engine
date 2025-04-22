/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "fen.h"
#include "piece.h"

typedef struct {
  int activeColor;
  Piece board[NUM_PIECES];
} Game;

void initGame(Game *game, Fen *fen);

#endif  // GAME_H
