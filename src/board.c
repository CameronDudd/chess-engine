/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include "board.h"

#include <log.h>
#include <stdio.h>

#include "defs.h"

static const char pieceCharacterLookup[PIECE_END] = {' ', 'K', 'Q', 'R', 'B', 'N', 'P', 'k', 'q', 'r', 'b', 'n', 'p'};

static char pieceChar(const Piece piece) {
  if ((piece < 0) || (piece >= PIECE_END)) return '?';
  return pieceCharacterLookup[piece];
}

void displayBoard(const Board* board) {
  for (int rank = 7; rank >= 0; --rank) {
    for (int file = 0; file < 8; ++file) {
      PositionIndex i = rank * 8 + file;
      printf("|%c", pieceChar(board->squares[i]));
    }
    printf("|\r\n");
  }
}

void boardSetPiece(Board* board, const PositionIndex position, const Piece piece) {
  if (board->turn == WHITE) {
    board->whites |= BIT_SQUARE(position);
  } else {
    board->blacks |= BIT_SQUARE(position);
  }
  board->squares[position] = piece;
}

Piece boardGetPiece(const Board* board, PositionIndex position) {
  return board->squares[position];
}

void boardSetTurnColor(Board* board, Color turnColor) {
  log_debug("Setting board turn to %s", (turnColor == WHITE) ? "WHITE" : "BLACK");
  board->turn = turnColor;
}

void boardSetCastlingAvailability(Board* board, CastlingAvailability castlingAvailability) {
#if DEBUG
  switch (castlingAvailability) {
    case CASTLE_WHITE_KING:
      log_debug("Setting board white king castle available");
      break;
    case CASTLE_WHITE_QUEEN:
      log_debug("Setting board white queen castle available");
      break;
    case CASTLE_BLACK_KING:
      log_debug("Setting board black king castle available");
      break;
    case CASTLE_BLACK_QUEEN:
      log_debug("Setting board black queen castle available");
      break;
  }
#endif
  board->castlingAvailability |= castlingAvailability;
}
