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

// Field: [ META  ][ FLAG  ][     DST     ][    SRC    ]
// Bits:  [ 15 14 ][ 13 12 ][11 10 9 8 7 6][5 4 3 2 1 0]
typedef uint16_t Move;
typedef uint8_t MoveFlag;
typedef uint8_t MoveMeta;

#define MOVE_FLAG_QUIET (MoveFlag)0
#define MOVE_FLAG_EP (MoveFlag)1
#define MOVE_FLAG_CASTLE (MoveFlag)2
#define MOVE_FLAG_PROMOTION (MoveFlag)3

#define MOVE_META_QUIET (MoveMeta)0

#define MOVE_META_PROMOTION_KNIGHT (MoveMeta)0
#define MOVE_META_PROMOTION_BISHOP (MoveMeta)1
#define MOVE_META_PROMOTION_ROOK (MoveMeta)2
#define MOVE_META_PROMOTION_QUEEN (MoveMeta)3

#define MOVE_META_CASTLE_KINGSIDE (MoveMeta)0
#define MOVE_META_CASTLE_QUEENSIDE (MoveMeta)1

#define _2BIT_MASK 0x3
#define _6BIT_MASK 0x3F
#define MOVE_DST_SRC_MASK 0x00000FFF

#define SRC(move) (PositionIndex)((move) & _6BIT_MASK)
#define DST(move) (PositionIndex)(((move) >> 6) & _6BIT_MASK)
#define FLAG(move) (MoveFlag)(((move) >> 12) & _2BIT_MASK)
#define META(move) (MoveMeta)(((move) >> 14) & _2BIT_MASK)
#define MOVE(meta, flag, dst, src) (Move)((((meta) & _2BIT_MASK) << 14) | (((flag) & _2BIT_MASK) << 12) | (((dst) & _6BIT_MASK) << 6) | ((src) & _6BIT_MASK))

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

void displayMove(Move move);
void displayBoard(const Board* board);
void displayBitBoard(uint64_t bitboard);

bool moveCastle(Move move);
bool moveCapture(Board* board, Move move);
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
bool getLegalMove(Move* move, const Move moves[MAX_CHESS_MOVES], unsigned int numMoves);

#endif  // BOARD_H
