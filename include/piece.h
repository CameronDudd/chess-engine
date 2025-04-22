/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#ifndef PIECE_H
#define PIECE_H

#include <stdint.h>

// https://www.chessprogramming.org/Pieces
// +-----+-----------+----------+
// | Bit |  Binary   | Semantic |
// +-----+-----------+----------+
// |  0  | 0000 0001 |  White   |
// |  1  | 0000 0010 |  Black   |
// +-----+-----------+----------+
// |  2  | 0000 0100 |  Pawn    |
// |  3  | 0000 1000 |  Knight  |
// |  4  | 0001 0000 |  Bishop  |
// |  5  | 0010 0000 |  Rook    |
// |  6  | 0100 0000 |  Queen   |
// |  7  | 1000 0000 |  King    |
// +-----+-----------+----------+

// Unicode Chess Symbols https://en.wikipedia.org/wiki/Chess_symbols_in_Unicode
// U+265x
// 4-K 5-Q 6-R 7-B 8-N 9-P A-k B-q C-r D-b E-n F-p

typedef uint8_t Piece;

#define PIECE_NULL   (Piece)0b00000000
#define PIECE_WHITE  (Piece)0b00000001
#define PIECE_BLACK  (Piece)0b00000010
#define PIECE_PAWN   (Piece)0b00000100
#define PIECE_KNIGHT (Piece)0b00001000
#define PIECE_BISHOP (Piece)0b00010000
#define PIECE_ROOK   (Piece)0b00100000
#define PIECE_QUEEN  (Piece)0b01000000
#define PIECE_KING   (Piece)0b10000000

const char *pieceStr(const Piece *piece);

#endif  // PIECE_H
