/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "board.h"

void displayBoard(const Board *board);
void displaySelectedPieces(const Board *board, const Piece *selectedPiece);
void displaySelectedPosition(const Board *board, const Position *selectedPosition);

#endif  // DISPLAY_H
