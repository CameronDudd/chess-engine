/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "display.h"
#include "board.h"
#include "piece.h"

#include <stddef.h>
#include <stdio.h>

// HELPERS ---------------------------------------------------------------------
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

static const char *getPieceColor(Piece piece) {
  return (piece & PIECE_WHITE) ? _rgbEscapeStringForeground(255, 255, 255)
                               : _rgbEscapeStringForeground(0, 0, 0);
}

static const char *getBoardColor(Position row, Position col) {
  return ((row + col) % 2) ? _rgbEscapeStringBackground(181, 136, 99)
                           : _rgbEscapeStringBackground(240, 217, 181);
}

// DISPLAYERS ------------------------------------------------------------------
typedef void (*_SquareDisplayer)(Position row, Position col, Piece piece,
                                 void *data);

static void _displaySquare(Position row, Position col, Piece piece, void *_) {
  (void)_;
  printf("%s%s %s \033[0m", getPieceColor(piece), getBoardColor(row, col),
         pieceStr(&piece));
}

static void _displayBitBoardSquare(Position row, Position col, Piece piece,
                                   void *data) {
  const char *boardColor;
  BitBoard bitBoard = *(BitBoard *)data;
  if (bitBoard & (1ul << ((row * COLS) + col))) {
    boardColor = _rgbEscapeStringBackground(255, 0, 0);
  } else {
    boardColor = getBoardColor(row, col);
  }
  printf("%s%s %s \033[0m", getPieceColor(piece), boardColor, pieceStr(&piece));
}

static void _displayPieceSelectionSquare(Position row, Position col,
                                         Piece piece, void *data) {
  Piece selectedPiece = *(Piece *)data;
  if (piece == selectedPiece) {
    printf("%s%s(%s)\033[0m", getPieceColor(piece), getBoardColor(row, col),
           pieceStr(&piece));
  } else {
    printf("%s%s %s \033[0m", getPieceColor(piece), getBoardColor(row, col),
           pieceStr(&piece));
  }
}

static void _displayPositionSelectionSquare(Position row, Position col,
                                            Piece piece, void *data) {
  Position selectedPosition = *(Position *)data;
  Position position = (row * COLS) + col;
  const char *boardColor;
  if (position == selectedPosition) {
    boardColor = _rgbEscapeStringBackground(255, 0, 0);
  } else {
    boardColor = getBoardColor(row, col);
  }
  printf("%s%s %s \033[0m", getPieceColor(piece), boardColor, pieceStr(&piece));
}

static void _displayLegalMoves(Position row, Position col, Piece piece,
                               void *data) {
  MoveList moveList = *(MoveList *)data;
  int positionInDst = 0;
  Position position = (row * COLS) + col;
  for (size_t t = 0; t < moveList.count; ++t) {
    positionInDst |= (MOVE_DST(moveList.moves[t]) == position);
  }
  if (positionInDst) {
    printf("%s%s[%s]\033[0m", getPieceColor(piece), getBoardColor(row, col),
           pieceStr(&piece));
  } else {
    printf("%s%s %s \033[0m", getPieceColor(piece), getBoardColor(row, col),
           pieceStr(&piece));
  }
}

// DISPLAY ---------------------------------------------------------------------
static void _displayBoardWithDisplayer(const Board board,
                                       _SquareDisplayer display, void *data) {
  printf("\n");
  for (Position row = 0; row < ROWS; ++row) {
    printf("%i ", ROWS - row);
    for (Position col = 0; col < COLS; ++col) {
      Piece piece = board[(row * COLS) + col];
      display(row, col, piece, data);
    }
    printf("\n");
  }
  printf("   A  B  C  D  E  F  G  H\n");
}

// PUBLIC ----------------------------------------------------------------------
void displayBoard(const Board board) {
  _displayBoardWithDisplayer(board, _displaySquare, NULL);
}

void displayBitBoard(const Board board, const BitBoard bitBoard) {
  _displayBoardWithDisplayer(board, _displayBitBoardSquare, (void *)&bitBoard);
}

void displaySelectedPieces(const Board board, const Piece selectedPiece) {
  _displayBoardWithDisplayer(board, _displayPieceSelectionSquare,
                             (void *)&selectedPiece);
}

void displaySelectedPosition(const Board board,
                             const Position selectedPosition) {
  _displayBoardWithDisplayer(board, _displayPositionSelectionSquare,
                             (void *)&selectedPosition);
}

void displayLegalMoves(const Board board, MoveList *moveList) {
  _displayBoardWithDisplayer(board, _displayLegalMoves, (void *)moveList);
}
