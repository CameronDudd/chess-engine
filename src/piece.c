/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "piece.h"

static int pieceMSBOffset(const Piece *piece) {
  for (int i = 7; i >= 0; --i) {
    if ((1 << i) & *piece) {
      return 7 - i;
    }
  }
  return 0;
}

const char *pieceStr(const Piece *piece) {
  static const char *symbols[] = {"♚", "♛", "♜", "♝", "♞", "♟",
                                  "♔", "♕", "♖", "♗", "♘", "♙"};
  if (*piece == PIECE_NULL) {
    return " ";
  }
  int index = (*piece & PIECE_WHITE) ? 0 : 6;
  index += pieceMSBOffset(piece);
  return symbols[index];
}
