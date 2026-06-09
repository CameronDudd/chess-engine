/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

/*
 * https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
 *
 * <FEN> ::=  <Piece Placement>
 *         ' ' <Side to move>
 *         ' ' <Castling ability>
 *         ' ' <En passant target square>
 *         ' ' <Halfmove clock>
 *         ' ' <Fullmove counter>
 *
 * 6 Fields:
 * 1. Piece Placement:
 *   a. Ranks 8 -> 1
 *   b. "/" between each rank
 *   c. Files a -> h
 *   d. White pieces upper case, Black lower case
 *   e. A set of one or more empty squares denoted by "1" to "8"
 *
 * 2. Active Color:
 *   a. "w" white to move, "b" black to move
 *
 * 3. Castling Availability:
 *   a. "-" neither can castle
 *   b. "K" white can castle king side
 *   c. "Q" white can castle queen side
 *   d. "k" black can castle king side
 *   e. "q" black can castle queen side
 *
 * TODO: 4. En Passant
 *
 * TODO: 5. Halfmove Clock
 *
 * TODO: 6. Fullmove Number
 */

#ifndef FEN_H
#define FEN_H

#include "board.h"

#define FEN_STARTING_POSITION "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

void fenPopulateBoard(const char* fen, Board* board);

#endif  // FEN_H
