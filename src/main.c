/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include <log.h>
#include <stdint.h>

#include "board.h"
#include "fen.h"
#include "magics.h"
#include "perft.h"

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

  PerftResult result = perft(&board, 6);  // Currently only correct up to perft 6
  printf("nodes %i | captures %i | e.p. %i | promotions %i | checks %i | checkmates %i \r\n", result.nodes, result.captures, result.ep, result.promotions,
         result.checks, result.checkmates);
}
