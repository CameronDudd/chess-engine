/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#ifndef PERFT_H
#define PERFT_H

#include <stdint.h>

#include "board.h"

unsigned int perft(Board* board, int depth);

#endif  // PERFT_H
