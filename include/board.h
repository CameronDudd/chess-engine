/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#include "piece.h"

#define ROWS           8
#define COLS           8
#define NUM_DIRECTIONS 8
#define NUM_POSITIONS  64
// https://www.chessprogramming.org/Encoding_Moves#MoveIndex
// R6R/3Q4/1Q4Q1/4Q3/2Q4Q/Q4Q2/pp1Q4/kBNN1KB1 w - - 0 1
// 3Q4/1Q4Q1/4Q3/2Q4R/Q4Q2/3Q4/1Q4Rp/1K1BBNNk w - - 0 1
#define MAX_CHESS_MOVES 218

typedef int Position;  // 0 top-left to 63 bottom-right
typedef uint64_t BitBoard;
typedef Piece Board[NUM_POSITIONS];

typedef enum {
  FORWARD        = -8,
  FORWARD_RIGHT  = -7,
  RIGHT          = 1,
  BACKWARD_RIGHT = 9,
  BACKWARD       = 8,
  BACKWARD_LEFT  = 7,
  LEFT           = -1,
  FORWARD_LEFT   = -9,
} Direction;

typedef struct {
  Position src;
  Position dst;
} Move;

extern BitBoard pieceBitBoards[NUM_UNIQUE_PIECES];
extern int distanceToEdgeLookup[NUM_POSITIONS][NUM_DIRECTIONS];

void clearBoard(Board board);
void initBitBoards(Board board);
void initDistanceToEdgeLookup();
void makeMove(Board board, const Move *move);
void strMakeMove(Board board, const char *moveStr);

#endif  // BOARD_H
