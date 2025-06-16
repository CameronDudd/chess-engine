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
  log_set_level(LOG_DEBUG);

  setlocale(LC_ALL, "");
  for (int i = 0; i < argc; ++i) {
    log_debug("arg %i = %s", i, *argv++);
  }

  Game game;
  initGame(&game, FEN_STARTING_POSITION);
  generateLegalMoves(&game.legalMoves, game.board, activeColor(&game), game.previousMoves.moves[game.previousMoves.count - 1]);
  displayBitBoard(game.board, colorAttackBitBoard(activeColor(&game)));

  return 0;
}
