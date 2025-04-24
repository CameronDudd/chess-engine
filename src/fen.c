/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "fen.h"
#include "piece.h"

static void _offsetToSectionOfInterest(const Fen **fen,
                                       FenWhitespaceOffsets offset) {
  uint8_t numWhiteSpace = 0;
  while (numWhiteSpace < offset && **fen != '\0') {
    if (**fen == ' ') {
      numWhiteSpace++;
    }
    (*fen)++;
  };
}

Piece fenPiece(Fen *fen) {
  switch (*fen) {
  case 'r':
    return PIECE_BLACK | PIECE_ROOK;
  case 'n':
    return PIECE_BLACK | PIECE_KNIGHT;
  case 'b':
    return PIECE_BLACK | PIECE_BISHOP;
  case 'q':
    return PIECE_BLACK | PIECE_QUEEN;
  case 'k':
    return PIECE_BLACK | PIECE_KING;
  case 'p':
    return PIECE_BLACK | PIECE_PAWN;
  case 'R':
    return PIECE_WHITE | PIECE_ROOK;
  case 'N':
    return PIECE_WHITE | PIECE_KNIGHT;
  case 'B':
    return PIECE_WHITE | PIECE_BISHOP;
  case 'Q':
    return PIECE_WHITE | PIECE_QUEEN;
  case 'K':
    return PIECE_WHITE | PIECE_KING;
  case 'P':
    return PIECE_WHITE | PIECE_PAWN;
  default:
    return PIECE_NULL;
  }
}

uint8_t fenWhitespace(const Fen *fen) {
  if ('0' <= *fen && *fen <= '9') {
    return *fen - '0';
  }
  return 0;
}

uint8_t fenActiveColor(const Fen *fen) {
  _offsetToSectionOfInterest(&fen, PIECE_ACTIVE_COLOR_OFFSET);
  return (*fen == 'w') ? 1 : 0;
}

uint8_t fenCastlingBits(const Fen *fen) { // TODO (cameron):
  _offsetToSectionOfInterest(&fen, CASTLING_FLAGS_OFFSET);
  uint8_t castlingBits = 0;
  return castlingBits;
}

void fenPopulateBoard(const Fen *fen, Board board) {
  _offsetToSectionOfInterest(&fen, PIECE_PLACEMENT_OFFSET);
  do {
    if (('a' <= *fen && *fen <= 'z') || ('A' <= *fen && *fen <= 'Z')) {
      *board++ = fenPiece(fen);
    } else if ('0' <= *fen && *fen <= '9') {
      for (int i = 0; i < fenWhitespace(fen); i++) {
        *board++ = PIECE_NULL;
      }
    }
  } while (*fen++ != ' ');
}
