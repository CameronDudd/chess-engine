/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#ifndef MAGICS_H
#define MAGICS_H

#include <stdint.h>

extern const uint64_t RookMagics[64];
extern const uint64_t BishopMagics[64];

void rookMagicNumbers(void);
void bishopMagicNumbers(void);

#endif  // MAGICS_H
