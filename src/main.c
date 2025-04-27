/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include <locale.h>
#include <log.h>
#include <stdint.h>

#include "board.h"
#include "display.h"
#include "fen.h"
#include "game.h"

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  for (int i = 0; i < argc; ++i) {
    log_debug("arg %i = %s\n", i, *argv++);
  }

  // Init
  Game game;
  initGame(&game, FEN_STARTING_POSITION);
  initBitBoards(game.board);

  // Main
  displayBoard(game.board);

  return 0;
}
