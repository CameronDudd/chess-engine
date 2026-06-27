/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>

#define FILE_A (BitBoard)0x0101010101010101
#define FILE_B (BitBoard)0x0202020202020202
#define FILE_C (BitBoard)0x0404040404040404
#define FILE_D (BitBoard)0x0808080808080808
#define FILE_E (BitBoard)0x1010101010101010
#define FILE_F (BitBoard)0x2020202020202020
#define FILE_G (BitBoard)0x4040404040404040
#define FILE_H (BitBoard)0x8080808080808080

#define RANK_1 (BitBoard)0x00000000000000FF
#define RANK_2 (BitBoard)0x000000000000FF00
#define RANK_3 (BitBoard)0x0000000000FF0000
#define RANK_4 (BitBoard)0x00000000FF000000
#define RANK_5 (BitBoard)0x000000FF00000000
#define RANK_6 (BitBoard)0x0000FF0000000000
#define RANK_7 (BitBoard)0x00FF000000000000
#define RANK_8 (BitBoard)0xFF00000000000000

#define CORNERS (BitBoard)0x8100000000000081

#define WHITE_KING_CASTLE_MASK (BitBoard)0x0000000000000070
#define WHITE_KING_ROOK_CASTLE_MASK (BitBoard)0x0000000000000060

#define WHITE_QUEEN_CASTLE_MASK (BitBoard)0x000000000000001C
#define WHITE_QUEEN_ROOK_CASTLE_MASK (BitBoard)0x000000000000000E

#define BLACK_KING_CASTLE_MASK (BitBoard)0x7000000000000000
#define BLACK_KING_ROOK_CASTLE_MASK (BitBoard)0x6000000000000000

#define BLACK_QUEEN_CASTLE_MASK (BitBoard)0x1C00000000000000
#define BLACK_QUEEN_ROOK_CASTLE_MASK (BitBoard)0x0E00000000000000

#define NUM_FILES 8
#define NUM_RANKS 8

#define MAX_FILE 7
#define MAX_RANK 7

#define NUM_POSITIONS 64  // 8 rows x 8 cols

#define POS_INDEX(rank, file) (((rank) * 8) + (file))
#define INDEX_RANK(index) ((index) / 8)
#define INDEX_FILE(index) ((index) % 8)
#define POSITION_BIT(square) (((uint64_t)1) << (square))

typedef uint8_t PositionIndex;  // 0 bottom-left to 63 top-right
typedef uint64_t BitBoard;

#define NAP (PositionIndex)0xFF  // Not A Position
#define A1 (PositionIndex)0
#define E1 (PositionIndex)4
#define H1 (PositionIndex)7
#define A8 (PositionIndex)56
#define E8 (PositionIndex)60
#define H8 (PositionIndex)63

typedef enum {
  WHITE      = 0,
  BLACK      = 1,
  NUM_COLORS = 2,
} Color;

extern const BitBoard files[NUM_FILES];
extern const BitBoard ranks[NUM_RANKS];

PositionIndex popLSB(BitBoard* bitboard);
PositionIndex getPopCount(BitBoard bitboard);

#endif  // DEFS_H
