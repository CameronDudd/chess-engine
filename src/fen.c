/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "fen.h"

Piece fenPiece(Fen *fen) {
  char f = *fen;
  Piece p = PIECE_NULL;

  // Determine color
  if ('a' <= f && f <= 'z') {
    p |= PIECE_BLACK;
  } else if ('A' <= f && f <= 'Z') {
    f = 'a' + (f - 'A'); // to lower case
    p |= PIECE_WHITE;
  } else {
    return p;
  }

  // Determine type
  switch (f) {
  case 'p':
    p |= PIECE_PAWN;
    break;
  case 'r':
    p |= PIECE_ROOK;
    break;
  case 'n':
    p |= PIECE_KNIGHT;
    break;
  case 'b':
    p |= PIECE_BISHOP;
    break;
  case 'q':
    p |= PIECE_QUEEN;
    break;
  case 'k':
    p |= PIECE_KING;
    break;
  }

  return p;
}

int fenWhitespace(Fen *fen) {
  if ('0' <= *fen && *fen <= '9') {
    return *fen - '0';
  }
  return 0;
}

int fenActiveColor(Fen *fen) {
  int activeColor = 0, numWhiteSpace = 0;
  while (numWhiteSpace < PIECE_ACTIVE_COLOR_OFFSET && *fen++ != '\0') {
    if (*fen == ' ') {
      numWhiteSpace++;
    }
  }
  return activeColor;
}

void fenPopulateBoard(Fen *fen, Piece board[NUM_PIECES]) {
  do {
    if ('a' <= *fen && *fen <= 'z') {
      *board++ = *fen;
    } else if ('0' <= *fen && *fen <= '9') {
      for (int i = 0; i < fenWhitespace(fen); i++) {
        *board++ = PIECE_NULL;
      }
    }
  } while (*fen++ != ' ');
}
