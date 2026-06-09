/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include "fen.h"

#include "board.h"

static Piece fenPiece(const char fen) {
  switch (fen) {
    case 'r':
      return BR;
    case 'n':
      return BN;
    case 'b':
      return BB;
    case 'q':
      return BQ;
    case 'k':
      return BK;
    case 'p':
      return BP;
    case 'R':
      return WR;
    case 'N':
      return WN;
    case 'B':
      return WB;
    case 'Q':
      return WQ;
    case 'K':
      return WK;
    case 'P':
      return WP;
    default:
      return PIECE_NULL;
  }
}

void fenPopulateBoard(const char* fen, Board* board) {
  char c;
  PositionIndex position = NUM_POSITIONS - 1;

  // Piece Placement
  while ((c = *fen++) != ' ') {
    if (c == '/') continue;
    if ('0' <= c && c <= '8') {
      position -= (c - '0' - 1);
    }
    boardSetPiece(board, position, fenPiece(c));
    --position;
  }

  // Side to move
  boardSetTurnColor(board, (*fen++ == 'w') ? WHITE : BLACK);
  ++fen;

  // Castling Availability
  while ((c = *fen++) != ' ') {
    if (c == '-') continue;
    switch (c) {
      case 'K':
        boardSetCastlingAvailability(board, CASTLE_WHITE_KING);
        break;
      case 'Q':
        boardSetCastlingAvailability(board, CASTLE_WHITE_QUEEN);
        break;
      case 'k':
        boardSetCastlingAvailability(board, CASTLE_BLACK_KING);
        break;
      case 'q':
        boardSetCastlingAvailability(board, CASTLE_BLACK_QUEEN);
        break;
    }
    if ('0' <= c && c <= '8') {
      position -= (c - '0' - 1);
    }
    boardSetPiece(board, position, fenPiece(c));
    --position;
  }

  // TODO: En Passant
  while ((c = *fen++) != ' ') {
  }

  // TODO: Halfmove
  while ((c = *fen++) != ' ') {
  }

  // TODO: Fullmove
  while ((c = *fen++) != '\0') {
  }
}
