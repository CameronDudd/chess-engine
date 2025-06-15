/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "fen.h"

typedef enum {
  ACTIVE_COLOR_BIT           = 0,  // 0 if black to move else 1
  WHITE_KINGSIDE_CASTLE_BIT  = 1,  // 1 if white can castle king-side else 0
  WHITE_QUEENSIDE_CASTLE_BIT = 2,  // 1 if white can castle queen-side else 0
  BLACK_KINGSIDE_CASTLE_BIT  = 3,  // 1 if black can castle king-side else 0
  BLACK_QUEENSIDE_CASTLE_BIT = 4,  // 1 if black can castle queen-side else 0
} GameStateBitOffsets;

typedef struct {
  Board board;
  uint8_t state;
  MoveList legalMoves;
  MoveList previousMoves;
} Game;

void initGame(Game *game, Fen *fen);
int activeColor(Game *game);

#endif  // GAME_H
