/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include <log.h>

#include "board.h"
#include "fen.h"
#include "magics.h"

void logInit(void) {
#ifdef DEBUG
  log_set_level(LOG_TRACE);
#else
  log_set_level(LOG_INFO);
#endif
}

static void init(void) {
  logInit();
  initMagicAttacks();
}

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;

  init();

  Board board;
  initBoard(&board);

  fenPopulateBoard(FEN_STARTING_POSITION, &board);
  generateLegalMoves(&board);
}
