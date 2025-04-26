/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "board.h"

#include <log.h>
#include <stdlib.h>

void makeMove(Board board, const Move *move) {
  board[move->dst] = board[move->src];
  board[move->src] = PIECE_NULL;
}

void strMakeMove(Board board, const char *c) {
  if (!(('A' <= c[0]) && (c[0] <= 'H')) && !(('1' <= c[1]) && (c[1] <= '8')) &&
      !(('A' <= c[2]) && (c[2] <= 'H')) && !(('1' <= c[3]) && (c[3] <= '8'))) {
    log_error("Invalid move string '%s'", c);
    exit(1);
  }
  Move move = {
      .src = (7 - (c[1] - '1')) * 8 + (7 - ('H' - c[0])),
      .dst = (7 - (c[3] - '1')) * 8 + (7 - ('H' - c[2])),
  };
  makeMove(board, &move);
}
