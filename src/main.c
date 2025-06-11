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
  setlocale(LC_ALL, "");
  for (int i = 0; i < argc; ++i) {
    log_debug("arg %i = %s", i, *argv++);
  }

  Board board;
  fenPopulateBoard(FEN_STARTING_POSITION, board);
  log_debug("+-------+-------+----------+---------+--------+-------------+");
  log_debug("| Depth | Nodes | Captures | Castles | Checks | Check Mates |");
  log_debug("+-------+-------+----------+---------+--------+-------------+");
  for (int depth = 0; depth < 4; ++depth) {
    struct PerftRes result = perft(board, 1, depth, 0);
    log_debug("|%6i |%6llu |%9lu |%8lu |%7lu |%12lu |", result.depth,
              result.nodes, result.captures, result.castles, result.checks,
              result.checkMates);
  }
  log_debug("+-------+-------+----------+---------+--------+-------------+");
  return 0;
}
