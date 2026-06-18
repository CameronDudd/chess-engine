/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdint.h>

#include "defs.h"

typedef enum {
  PIECE_NULL = 0,
  WK,
  WQ,
  WR,
  WB,
  WN,
  WP,
  BK,
  BQ,
  BR,
  BB,
  BN,
  BP,
  PIECE_END,
} Piece;

typedef enum {
  KING = 0,
  QUEEN,
  ROOK,
  BISHOP,
  KNIGHT,
  PAWN,
  NUM_PIECE_TYPES,
} PieceBoardIndex;

// https://www.chessprogramming.org/Encoding_Moves#MoveIndex
// R6R/3Q4/1Q4Q1/4Q3/2Q4Q/Q4Q2/pp1Q4/kBNN1KB1 w - - 0 1
// 3Q4/1Q4Q1/4Q3/2Q4R/Q4Q2/3Q4/1Q4Rp/1K1BBNNk w - - 0 1
#define MAX_CHESS_MOVES 218

// 2 source
// 2 destination
// 1 Capture
// 1 Check
// 1 Null Termination
#define MOVE_STRING_SIZE 7

// Field: [  FLAG   ][     DST     ][    SRC    ]
// Bits:  [31 ... 12][11 10 9 8 7 6][5 4 3 2 1 0]
typedef uint32_t Move;
typedef uint16_t MoveFlag;

#define MOVE_QUIET 0x0000
#define MOVE_CAPTURE 0x0002
#define MOVE_KING_CASTLE 0x0004
#define MOVE_QUEEN_CASTLE 0x0008
#define MOVE_ROOK_PROMOTION 0x0010
#define MOVE_QUEEN_PROMOTION 0x0020
#define MOVE_BISHOP_PROMOTION 0x0040
#define MOVE_KNIGHT_PROMOTION 0x0080
#define MOVE_EP 0x0100
#define MOVE_DOUBLE_PUSH 0x0200
#define MOVE_CHECKMATE 0x0400

#define MOVE_PROMOTION 0x00F0

#define _6BIT_MASK 0x3F
#define _4BIT_MASK 0x0F

#define SRC(move) (PositionIndex)((move) & _6BIT_MASK)
#define DST(move) (PositionIndex)(((move) >> 6) & _6BIT_MASK)
#define FLAG(move) (MoveFlag)((move) >> 12)
#define MOVE(flag, dst, src) (Move)(((flag) << 12) | (((dst) & _6BIT_MASK) << 6) | ((src) & _6BIT_MASK))

typedef uint8_t CastlingAvailability;
#define CASTLE_WHITE_KING (CastlingAvailability)0x1
#define CASTLE_WHITE_QUEEN (CastlingAvailability)0x2
#define CASTLE_BLACK_KING (CastlingAvailability)0x4
#define CASTLE_BLACK_QUEEN (CastlingAvailability)0x8

typedef struct {
  Move move;
  Piece captured;
  CastlingAvailability castlingAvailability;
  PositionIndex epSquare;
  PositionIndex epCapturedSquare;
} UndoMove;

typedef struct {
  Piece squares[NUM_POSITIONS];

  Color turn;
  CastlingAvailability castlingAvailability;
  PositionIndex epSquare;

  // Bitboards
  BitBoard whites;
  BitBoard blacks;

  BitBoard whitePieceBoards[NUM_PIECE_TYPES];
  BitBoard blackPieceBoards[NUM_PIECE_TYPES];

} Board;

void displayBoard(const Board* board);
void displayBitBoard(uint64_t bitboard);

bool moveCastle(Move move);
bool moveCapture(Move move);
bool movePromotion(Move move);
bool moveEP(Move move);

void initBoard(Board* board);
void boardSetPiece(Board* board, PositionIndex position, Piece piece);
Piece boardGetPiece(const Board* board, PositionIndex position);
void boardMakeMove(Board* board, Move move, UndoMove* undo);
void boardUnmakeMove(Board* board, const UndoMove* undo);
void boardSetTurnColor(Board* board, Color turnColor);
void boardSetCastlingAvailability(Board* board, CastlingAvailability castlingAvailability);
bool kingInCheck(Board* board, Color side);
unsigned int generateLegalMoves(Board* board, Move* moves);

#endif  // BOARD_H
