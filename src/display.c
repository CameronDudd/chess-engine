/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "display.h"
#include "board.h"
#include "piece.h"

#include <stdio.h>

static const char *_rgbEscapeStringForeground(uint8_t r, uint8_t g, uint8_t b) {
  static char fgBuff[30];
  snprintf(fgBuff, sizeof(fgBuff), "\033[38;2;%i;%i;%im", r, g, b);
  return fgBuff;
}

static const char *_rgbEscapeStringBackground(uint8_t r, uint8_t g, uint8_t b) {
  static char bgBuff[30];
  snprintf(bgBuff, sizeof(bgBuff), "\033[48;2;%i;%i;%im", r, g, b);
  return bgBuff;
}

void displayBoard(const Board board) {
  for (Position row = 0; row < ROWS; ++row) {
    for (Position col = 0; col < COLS; ++col) {
      Piece p = board[(row * COLS) + col];
      const char *pieceColor = (p & PIECE_WHITE)
                                   ? _rgbEscapeStringForeground(255, 255, 255)
                                   : _rgbEscapeStringForeground(0, 0, 0);
      const char *boardColor = ((row + col) % 2)
                                   ? _rgbEscapeStringBackground(181, 136, 99)
                                   : _rgbEscapeStringBackground(240, 217, 181);
      printf("%s%s %s \033[0m", pieceColor, boardColor, pieceStr(&p));
    }
    printf("\n");
  }
}

void displaySelectedPieces(const Board board, const Piece *selectedPiece) {
  for (Position row = 0; row < ROWS; ++row) {
    for (Position col = 0; col < COLS; ++col) {
      Piece p = board[(row * COLS) + col];
      const char *bg = ((row + col) % 2) ? "\033[100m" : "\033[47m";
      if (p == *selectedPiece) {
        printf("%s(%s)\033[0m", bg, pieceStr(&p));
      } else {
        printf("%s %s \033[0m", bg, pieceStr(&p));
      }
    }
    printf("\n");
  }
}

void displaySelectedPosition(const Board board,
                             const Position *selectedPosition) {
  for (Position row = 0; row < ROWS; ++row) {
    for (Position col = 0; col < COLS; ++col) {
      Position position = (row * COLS) + col;
      Piece piece = board[position];
      const char *bg = ((row + col) % 2) ? "\033[100m" : "\033[47m";
      if (position == *selectedPosition) {
        printf("%s(%s)\033[0m", bg, pieceStr(&piece));
      } else {
        printf("%s %s \033[0m", bg, pieceStr(&piece));
      }
    }
    printf("\n");
  }
}
