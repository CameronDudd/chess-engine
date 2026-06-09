/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include <log.h>

#include "board.h"
#include "fen.h"

void logInit(void) {
#ifdef DEBUG
  log_set_level(LOG_TRACE);
#else
  log_set_level(LOG_INFO);
#endif
}

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;

  logInit();

  Board board = {0};

  fenPopulateBoard(FEN_STARTING_POSITION, &board);

  displayBoard(&board);
}
