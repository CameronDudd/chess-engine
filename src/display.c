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
  printf("\n");
  for (Position row = 0; row < ROWS; ++row) {
    printf("%i ", ROWS - row);
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
  printf("   A  B  C  D  E  F  G  H\n");
}

void displayBitBoard(const Board board, const BitBoard bitBoard) {
  printf("\n");
  for (Position row = 0; row < ROWS; ++row) {
    printf("%i ", ROWS - row);
    for (Position col = 0; col < COLS; ++col) {
      Position boardPos = (row * COLS) + col;
      Piece p = board[boardPos];
      const char *pieceColor = (p & PIECE_WHITE)
                                   ? _rgbEscapeStringForeground(255, 255, 255)
                                   : _rgbEscapeStringForeground(0, 0, 0);
      const char *boardColor;
      if (bitBoard & (1ul << boardPos)) {
        boardColor = _rgbEscapeStringBackground(255, 0, 0);
      } else {
        boardColor = ((row + col) % 2)
                         ? _rgbEscapeStringBackground(181, 136, 99)
                         : _rgbEscapeStringBackground(240, 217, 181);
      }
      printf("%s%s %s \033[0m", pieceColor, boardColor, pieceStr(&p));
    }
    printf("\n");
  }
  printf("   A  B  C  D  E  F  G  H\n");
}

void displaySelectedPieces(const Board board, const Piece *selectedPiece) {
  printf("\n");
  for (Position row = 0; row < ROWS; ++row) {
    printf("%i ", ROWS - row);
    for (Position col = 0; col < COLS; ++col) {
      Piece p = board[(row * COLS) + col];
      const char *pieceColor = (p & PIECE_WHITE)
                                   ? _rgbEscapeStringForeground(255, 255, 255)
                                   : _rgbEscapeStringForeground(0, 0, 0);
      const char *boardColor = ((row + col) % 2)
                                   ? _rgbEscapeStringBackground(181, 136, 99)
                                   : _rgbEscapeStringBackground(240, 217, 181);
      if (p == *selectedPiece) {
        printf("%s%s(%s)\033[0m", pieceColor, boardColor, pieceStr(&p));
      } else {
        printf("%s%s %s \033[0m", pieceColor, boardColor, pieceStr(&p));
      }
    }
    printf("\n");
  }
  printf("   A  B  C  D  E  F  G  H\n");
}

void displaySelectedPosition(const Board board,
                             const Position *selectedPosition) {
  printf("\n");
  for (Position row = 0; row < ROWS; ++row) {
    printf("%i ", ROWS - row);
    for (Position col = 0; col < COLS; ++col) {
      Position position = (row * COLS) + col;
      Piece p = board[position];
      const char *pieceColor = (p & PIECE_WHITE)
                                   ? _rgbEscapeStringForeground(255, 255, 255)
                                   : _rgbEscapeStringForeground(0, 0, 0);
      const char *boardColor = ((row + col) % 2)
                                   ? _rgbEscapeStringBackground(181, 136, 99)
                                   : _rgbEscapeStringBackground(240, 217, 181);
      if (position == *selectedPosition) {
        printf("%s%s(%s)\033[0m", pieceColor, boardColor, pieceStr(&p));
      } else {
        printf("%s%s %s \033[0m", pieceColor, boardColor, pieceStr(&p));
      }
    }
    printf("\n");
  }
  printf("   A  B  C  D  E  F  G  H\n");
}
