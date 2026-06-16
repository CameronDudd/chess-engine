/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include "fen.h"

#include <stdint.h>

#include "board.h"
#include "defs.h"

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
  uint8_t rank = MAX_RANK;
  uint8_t file = 0;
  char c;

  // Init board
  board->castlingAvailability = 0;

  // Piece Placement
  while ((c = *fen++) != ' ') {
    if (c == '/') {
      --rank;
      file = 0;
      continue;
    };

    if ('0' <= c && c <= '8') {
      file += (c - '0');
      continue;
    }

    boardSetPiece(board, (rank * NUM_FILES) + file, fenPiece(c));
    ++file;
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
      default:
        break;
    }
  }

  // TODO: En Passant
  while ((c = *fen++) != ' ') {  // NOLINT(clang-analyzer-deadcode.DeadStores)
  }

  // TODO: Halfmove
  while ((c = *fen++) != ' ') {  // NOLINT(clang-analyzer-deadcode.DeadStores)
  }

  // TODO: Fullmove
  while ((c = *fen++) != '\0') {  // NOLINT(clang-analyzer-deadcode.DeadStores)
  }
}
