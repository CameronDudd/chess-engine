/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include "defs.h"

const BitBoard files[NUM_FILES] = {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H};
const BitBoard ranks[NUM_RANKS] = {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8};

PositionIndex popLSB(BitBoard* bitboard) {
  PositionIndex pos = __builtin_ctzll(*bitboard);
  *bitboard &= *bitboard - 1;
  return pos;
}

PositionIndex getPopCount(BitBoard bitboard) {
  return __builtin_popcountll(bitboard);
}
