/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>
#include <stdint.h>

#include "board.h"

#define MATE 9999999
#define STALEMATE 500

#define TRANSPOSITION_TABLE_SIZE (ZobristHash)16777216  // (256MB * 1024 * 1024) / 16 (bytes after alignment)

typedef uint8_t TranspositionEntryType;
#define TRANSPOSITION_TYPE_EXACT (TranspositionEntryType)0
#define TRANSPOSITION_TYPE_LOWERBOUND (TranspositionEntryType)1
#define TRANSPOSITION_TYPE_UPPERBOUND (TranspositionEntryType)2

typedef struct {
  ZobristHash zobrist;          // 8 bytes
  Move move;                    // 2 bytes
  uint8_t depth;                // 1 byte
  TranspositionEntryType type;  // 1 byte
  int score;                    // 4 bytes
} TranspositionTableEntry;

typedef struct {
  int score;
  Move move;
} ScoredMove;

typedef struct {
  int score;
  Move move;
  uint64_t nodes;
  uint64_t qnodes;
} SearchResult;

typedef struct {
  uint64_t endTimeNs;
  bool searchInterrupted;
} SearchTimer;

SearchResult iterativeSearch(Board* board, unsigned int depthLimit, unsigned int timeLimitMs);

#endif  // ENGINE_H
