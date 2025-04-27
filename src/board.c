/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "board.h"
#include "piece.h"

#include <log.h>
#include <stdlib.h>

uint64_t pieceBitBoards[NUM_UNIQUE_PIECES] = {
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
};

static PieceLookupKey _piece2lookup(Piece piece) {
  switch (piece) {
  case PIECE_BLACK | PIECE_ROOK:
    return BLACK_ROOK_KEY;
  case PIECE_BLACK | PIECE_KNIGHT:
    return BLACK_KNIGHT_KEY;
  case PIECE_BLACK | PIECE_BISHOP:
    return BLACK_BISHOP_KEY;
  case PIECE_BLACK | PIECE_QUEEN:
    return BLACK_QUEEN_KEY;
  case PIECE_BLACK | PIECE_KING:
    return BLACK_KING_KEY;
  case PIECE_BLACK | PIECE_PAWN:
    return BLACK_PAWN_KEY;
  case PIECE_WHITE | PIECE_ROOK:
    return WHITE_ROOK_KEY;
  case PIECE_WHITE | PIECE_KNIGHT:
    return WHITE_KNIGHT_KEY;
  case PIECE_WHITE | PIECE_BISHOP:
    return WHITE_BISHOP_KEY;
  case PIECE_WHITE | PIECE_QUEEN:
    return WHITE_QUEEN_KEY;
  case PIECE_WHITE | PIECE_KING:
    return WHITE_KING_KEY;
  case PIECE_WHITE | PIECE_PAWN:
    return WHITE_PAWN_KEY;
  default:
    log_error("Invalid piece '%i'", piece);
    exit(1);
  }
}

void initBitBoards(Board board) {
  for (Position position = 0; position < NUM_POSITIONS; ++position) {
    Piece piece = board[position];
    if (piece != PIECE_NULL) {
      PieceLookupKey lookupKey = _piece2lookup(piece);
      pieceBitBoards[lookupKey] |= ((uint64_t)1 << position);
    }
  }
}

void makeMove(Board board, const Move *move) {
  board[move->dst] = board[move->src];
  board[move->src] = PIECE_NULL;
}

void strMakeMove(Board board, const char *c) {
  if (!(('A' <= c[0]) && (c[0] <= 'H')) && !(('1' <= c[1]) && (c[1] <= '8')) &&
      !(('A' <= c[2]) && (c[2] <= 'H')) && !(('1' <= c[3]) && (c[3] <= '8'))) {
    log_error("Invalid move string '%s'", c);
    exit(1);
  }
  Move move = {
      .src = (7 - (c[1] - '1')) * 8 + (7 - ('H' - c[0])),
      .dst = (7 - (c[3] - '1')) * 8 + (7 - ('H' - c[2])),
  };
  makeMove(board, &move);
}
