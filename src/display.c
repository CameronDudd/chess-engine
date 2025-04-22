/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "display.h"
#include "piece.h"

#include <stdio.h>

void displayBoard(const Board *board) {
  printf("+---+---+---+---+---+---+---+---+\n");
  for (Position row = 0; row < ROWS; ++row) {
    printf("|");
    for (Position col = 0; col < COLS; ++col) {
      Piece p = (*board)[(row * 8) + col];
      printf(" %s |", pieceStr(&p));
    }
    printf("\n+---+---+---+---+---+---+---+---+\n");
  }
}

void displaySelectedPieces(const Board *board, const Piece *selectedPiece) {
  printf("+---+---+---+---+---+---+---+---+\n");
  for (Position row = 0; row < ROWS; ++row) {
    printf("|");
    for (Position col = 0; col < COLS; ++col) {
      Piece p = (*board)[(row * 8) + col];
      if (p == *selectedPiece) {
        printf("(%s)|", pieceStr(&p));
      } else {
        printf(" %s |", pieceStr(&p));
      }
    }
    printf("\n+---+---+---+---+---+---+---+---+\n");
  }
}

void displaySelectedPosition(const Board *board,
                             const Position *selectedPosition) {
  printf("+---+---+---+---+---+---+---+---+\n");
  for (Position row = 0; row < ROWS; ++row) {
    printf("|");
    for (Position col = 0; col < COLS; ++col) {
      Position position = (row * 8) + col;
      Piece piece = (*board)[position];
      if (position == *selectedPosition) {
        printf("(%s)|", pieceStr(&piece));
      } else {
        printf(" %s |", pieceStr(&piece));
      }
    }
    printf("\n+---+---+---+---+---+---+---+---+\n");
  }
}
