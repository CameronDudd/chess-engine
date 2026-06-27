/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include "player.h"

#include <stdio.h>

#include "board.h"
#include "log.h"

#define MAX_PLAYER_MOVE_STR 6  // 'a2a4\n\0'

static bool decodePlayerMoveStr(const char playerMove[MAX_PLAYER_MOVE_STR], Move* move) {
  char srcFile = playerMove[0];
  char srcRow  = playerMove[1];
  char dstFile = playerMove[2];
  char dstRow  = playerMove[3];
  if (('a' <= srcFile && srcFile <= 'h') && ('1' <= srcRow && srcRow <= '8') && ('a' <= dstFile && dstFile <= 'h') && ('1' <= dstRow && dstRow <= '8')) {
    *move = MOVE(MOVE_META_QUIET, MOVE_FLAG_QUIET, (dstRow - '1') * 8 + (dstFile - 'a'), (srcRow - '1') * 8 + (srcFile - 'a'));
    return true;
  }
  return false;
}

bool getPlayerMove(Move* move) {
  char playerMove[MAX_PLAYER_MOVE_STR] = {'\0'};

  printf("Player Move: ");
  if (!fgets(playerMove, sizeof(playerMove), stdin)) {
    log_error("failed to obtain player move string");
    return false;
  }

  return decodePlayerMoveStr(playerMove, move);
}
