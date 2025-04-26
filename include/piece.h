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
#define PIECE_NULL       (Piece)0b00000000
#define PIECE_WHITE      (Piece)0b00000001
#define PIECE_BLACK      (Piece)0b00000010
#define PIECE_PAWN       (Piece)0b00000100
#define PIECE_KNIGHT     (Piece)0b00001000
#define PIECE_BISHOP     (Piece)0b00010000
#define PIECE_ROOK       (Piece)0b00100000
#define PIECE_QUEEN      (Piece)0b01000000
#define PIECE_KING       (Piece)0b10000000
#define PIECE_TYPE_MASK  (Piece)0b11111100
#define PIECE_COLOR_MASK (Piece)0b00000011

// Used to create the keys for tables for things like move, bitboards etc
#define MIN_PIECE_KEY     0
#define MAX_PIECE_KEY     11
#define NUM_UNIQUE_PIECES 12

// Must always start from zero and go in ascending order
typedef enum {
  BLACK_PAWN_KEY   = 0,  // MIN_PIECE_KEY
  BLACK_KNIGHT_KEY = 1,
  BLACK_BISHOP_KEY = 2,
  BLACK_ROOK_KEY   = 3,
  BLACK_QUEEN_KEY  = 4,
  BLACK_KING_KEY   = 5,
  WHITE_PAWN_KEY   = 6,
  WHITE_KNIGHT_KEY = 7,
  WHITE_BISHOP_KEY = 8,
  WHITE_ROOK_KEY   = 9,
  WHITE_QUEEN_KEY  = 10,
  WHITE_KING_KEY   = 11,  // MAX_PIECE_KEY
} PieceLookupKey;

const char *pieceStr(const Piece *piece);

#endif  // PIECE_H
