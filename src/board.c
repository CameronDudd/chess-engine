/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include "board.h"

#include <stdbool.h>
#include <stdio.h>

static const char pieceCharacterLookup[] = {'K', 'Q', 'R', 'B', 'N', 'P'};

static uint8_t pieceMSB(const Piece piece) {
  return __builtin_clz(piece) - 24;
}

static char pieceChar(const Piece piece) {
  if (piece == PIECE_NULL) {
    return ' ';
  }
  if (piece & PIECE_BLACK) {
    return 'a' + (pieceCharacterLookup[pieceMSB(piece)] - 'A');
  }
  return pieceCharacterLookup[pieceMSB(piece)];
}

void displayBoard(const Board board) {
  for (PositionIndex row = 0; row < N; ++row) {
    printf("|");
    for (PositionIndex col = 0; col < N; ++col) {
      printf("%c|", pieceChar(board[row][col]));
    }
    printf("\r\n");
  }
}
