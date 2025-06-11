/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include <locale.h>
#include <log.h>
#include <stdint.h>

#include "board.h"
#include "fen.h"
#include "perft.h"

int main(int argc, char *argv[]) {
  log_set_level(LOG_INFO);

  setlocale(LC_ALL, "");
  for (int i = 0; i < argc; ++i) {
    log_debug("arg %i = %s", i, *argv++);
  }

  Board board;
  fenPopulateBoard(FEN_STARTING_POSITION, board);
  // TODO (cameron): use printf instead of log_info
  log_info("+-------+-------+----------+---------+--------+-------------+");
  log_info("| Depth | Nodes | Captures | Castles | Checks | Check Mates |");
  log_info("+-------+-------+----------+---------+--------+-------------+");
  for (int depth = 0; depth < 4; ++depth) {
    struct PerftRes result = perft(board, 1, depth, 0);
    log_info("|%6i |%6llu |%9lu |%8lu |%7lu |%12lu |", result.depth, result.nodes, result.captures, result.castles, result.checks, result.checkMates);
  }
  log_info("+-------+-------+----------+---------+--------+-------------+");
  return 0;
}
