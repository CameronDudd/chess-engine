/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include <stdio.h>

#include "fen.h"
#include "game.h"

int main(int argc, char *argv[]) {
  for (int i = 0; i < argc; ++i) {
    printf("arg %i = %s\n", i, *argv++);
  }
  Game game;
  initGame(&game, FEN_STARTING_POSITION);
  return 0;
}
