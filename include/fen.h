/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

/*
 * https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
 *
 * 6 Fields:
 * 1. Piece Placement:
 *   a. Ranks 8 -> 1
 *   b. "/" between each rank
 *   c. Files a -> h
 *   d. White pieces upper case, Black lower case
 *   e. A set of one or more empty squares denoted by "1" to "8"
 * 2. Active Color:
 *   a. "w" white to move, "b" black to move
 * 3. Castling Availability:
 *   a. "-" neither can castle
 *   b. "K" white can castle king side
 *   c. "Q" white can castle queen side
 *   d. "k" black can castle king side
 *   e. "q" black can castle queen side
 * 4. En Passant: TODO (cameron)
 * 5. Halfmove Clock: TODO (cameron)
 * 6. Fullmove Number: TODO (cameron)
 */

#ifndef FEN_H
#define FEN_H

#include "board.h"
#include "piece.h"

#define PIECE_PLACEMENT_OFFSET    0
#define PIECE_ACTIVE_COLOR_OFFSET 1

#define FEN_STARTING_POSITION "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

typedef const char Fen;

Piece fenPiece(const char *p);
int fenWhitespace(Fen *fen);
int fenActiveColor(Fen *fen);
void fenPopulateBoard(Fen *fen, Piece board[NUM_PIECES]);

#endif  // FEN_H
