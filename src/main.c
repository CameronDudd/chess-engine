/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include <locale.h>
#include <stdio.h>

#include "display.h"
#include "fen.h"
#include "game.h"

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");

  for (int i = 0; i < argc; ++i) {
    printf("arg %i = %s\n", i, *argv++);
  }

  Game game;
  initGame(&game, FEN_STARTING_POSITION);
  displayBoard(&game.board);
  return 0;
}
