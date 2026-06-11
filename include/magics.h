/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#ifndef MAGICS_H
#define MAGICS_H

#include <stdint.h>

#include "defs.h"

#define ROOK_VARIATIONS 4096
#define BISHOP_VARIATIONS 512

#define FILE_A (BitBoard)0x0101010101010101
#define FILE_H (BitBoard)0x8080808080808080

extern const BitBoard RookMagics[NUM_POSITIONS];
extern const BitBoard BishopMagics[NUM_POSITIONS];

extern BitBoard rookMasks[NUM_POSITIONS];
extern BitBoard bishopMasks[NUM_POSITIONS];

extern BitBoard kingAttacks[NUM_POSITIONS];
extern BitBoard rookAttacks[NUM_POSITIONS][ROOK_VARIATIONS];
extern BitBoard bishopAttacks[NUM_POSITIONS][BISHOP_VARIATIONS];
extern BitBoard knightAttacks[NUM_POSITIONS];
extern BitBoard pawnAttacks[2][NUM_POSITIONS];

BitBoard occupancyMask(int index, BitBoard mask);

BitBoard rookMask(int rank, int file);
BitBoard rookLegalMoves(int rank, int file, uint64_t occupancyMask);

BitBoard bishopMask(int rank, int file);
BitBoard bishopLegalMoves(int rank, int file, uint64_t occupancyMask);

void rookMagicNumbers(void);
void bishopMagicNumbers(void);

void initMagicAttacks(void);

#endif  // MAGICS_H
