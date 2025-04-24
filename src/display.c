/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "display.h"
#include "board.h"
#include "piece.h"

#include <stdio.h>

void displayBoard(const Board *board) {
  for (Position row = 0; row < ROWS; ++row) {
    for (Position col = 0; col < COLS; ++col) {
      Piece p = (*board)[(row * COLS) + col];
      const char *bg = ((row + col) % 2) ? "\033[100m" : "\033[47m";
      printf("%s %s \033[0m", bg, pieceStr(&p));
    }
    printf("\n");
  }
}

void displaySelectedPieces(const Board *board, const Piece *selectedPiece) {
  for (Position row = 0; row < ROWS; ++row) {
    for (Position col = 0; col < COLS; ++col) {
      Piece p = (*board)[(row * COLS) + col];
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

void displaySelectedPosition(const Board *board,
                             const Position *selectedPosition) {
  for (Position row = 0; row < ROWS; ++row) {
    for (Position col = 0; col < COLS; ++col) {
      Position position = (row * COLS) + col;
      Piece piece = (*board)[position];
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
