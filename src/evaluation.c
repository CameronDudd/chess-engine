/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "evaluation.h"
#include "board.h"
#include "piece.h"

static PieceValue pieceValue(Piece *piece) {
  switch (*piece & PIECE_TYPE_MASK) {
  case PIECE_PAWN:
    return PAWN_VALUE;
  case PIECE_KNIGHT:
    return KNIGHT_VALUE;
  case PIECE_BISHOP:
    return BISHOP_VALUE;
  case PIECE_ROOK:
    return ROOK_VALUE;
  case PIECE_QUEEN:
    return QUEEN_VALUE;
  default:
    return NULL_VALUE;
  }
}

int evaluate(Board board) {
  unsigned int blackEval = 0;
  unsigned int whiteEval = 0;
  for (int i = 0; i < NUM_PIECES; ++i) {
    Piece p = board[i];
    if ((p & PIECE_BLACK) > 0) {
      blackEval += pieceValue(&p);
    } else {
      whiteEval += pieceValue(&p);
    }
  }
  return whiteEval - blackEval;
}
