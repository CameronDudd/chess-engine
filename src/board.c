/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "board.h"
#include "piece.h"

#include <log.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const Direction _directions[NUM_DIRECTIONS] = {
    FORWARD,       BACKWARD,       LEFT,          RIGHT,
    FORWARD_RIGHT, BACKWARD_RIGHT, BACKWARD_LEFT, FORWARD_LEFT};

static const Direction _pawnDirections[NUM_PAWN_DIRECTIONS] = {
    FORWARD,  FORWARD_LEFT,  FORWARD_RIGHT,
    BACKWARD, BACKWARD_LEFT, BACKWARD_RIGHT,
};

static const int _knightDirections[] = {-17, -15, -10, -6, 6, 10, 15, 17};

int distanceToEdgeLookup[NUM_POSITIONS][NUM_DIRECTIONS] = {0};

BitBoard pieceBitBoards[NUM_UNIQUE_PIECES] = {
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
};

static char *_piece2str(const Piece *piece) __attribute__((unused));
static char *_piece2str(const Piece *piece) {
  static char str[100] = {'\0'};
  memset(str, '\0', 100);
  switch (*piece) {
  case PIECE_BLACK | PIECE_ROOK:
    strcpy(str, "BLACK ROOK");
    break;
  case PIECE_BLACK | PIECE_KNIGHT:
    strcpy(str, "BLACK KNIGHT");
    break;
  case PIECE_BLACK | PIECE_BISHOP:
    strcpy(str, "BLACK BISHOP");
    break;
  case PIECE_BLACK | PIECE_QUEEN:
    strcpy(str, "BLACK QUEEN");
    break;
  case PIECE_BLACK | PIECE_KING:
    strcpy(str, "BLACK KING");
    break;
  case PIECE_BLACK | PIECE_PAWN:
    strcpy(str, "BLACK PAWN");
    break;
  case PIECE_WHITE | PIECE_ROOK:
    strcpy(str, "WHITE ROOK");
    break;
  case PIECE_WHITE | PIECE_KNIGHT:
    strcpy(str, "WHITE KNIGHT");
    break;
  case PIECE_WHITE | PIECE_BISHOP:
    strcpy(str, "WHITE BISHOP");
    break;
  case PIECE_WHITE | PIECE_QUEEN:
    strcpy(str, "WHITE QUEEN");
    break;
  case PIECE_WHITE | PIECE_KING:
    strcpy(str, "WHITE KING");
    break;
  case PIECE_WHITE | PIECE_PAWN:
    strcpy(str, "WHITE PAWN");
    break;
  default:
    log_error("Invalid piece '%i'", piece);
    exit(1);
  }
  return str;
}

static char *_move2str(const Move *move) __attribute__((unused));
static char *_move2str(const Move *move) {
  static char str[100] = {'\0'};
  memset(str, '\0', 100);
  char *s = str;
  *s++ = 'A' + ((move->src) % 8);
  *s++ = '1' + 7 - ((move->src) / 8);
  strncpy(s, " -> ", 4);
  s += 4;
  *s++ = 'A' + ((move->dst) % 8);
  *s++ = '1' + 7 - ((move->dst) / 8);
  return str;
}

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

static inline bool _validPosition(Position position) {
  return 0 <= position && position < NUM_POSITIONS;
}

static inline bool _sameColor(Piece piece, Piece comparisonPiece) {
  return (piece & PIECE_COLOR_MASK) == (comparisonPiece & PIECE_COLOR_MASK);
}

static inline bool _isRank(int rank, Position position) {
  int rankStart = COLS * (8 - rank);
  int rankEnd = rankStart + COLS;
  return rankStart <= position && position <= rankEnd;
}

void clearBoard(Board board) {
  for (Position position = 0; position < NUM_POSITIONS; ++position) {
    board[position] = 0;
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

void initDistanceToEdgeLookup() {
  for (Position position = 0; position < NUM_POSITIONS; ++position) {
    for (size_t directionIdx = 0;
         directionIdx < sizeof(_directions) / sizeof(_directions[0]);
         ++directionIdx) {
      int dRow = 0;
      int dCol = 0;
      Direction direction = _directions[directionIdx];
      switch (direction) {
      case FORWARD:
        dRow = -1;
        dCol = 0;
        break;
      case FORWARD_RIGHT:
        dRow = -1;
        dCol = 1;
        break;
      case RIGHT:
        dRow = 0;
        dCol = 1;
        break;
      case BACKWARD_RIGHT:
        dRow = 1;
        dCol = 1;
        break;
      case BACKWARD:
        dRow = 1;
        dCol = 0;
        break;
      case BACKWARD_LEFT:
        dRow = 1;
        dCol = -1;
        break;
      case LEFT:
        dRow = 0;
        dCol = -1;
        break;
      case FORWARD_LEFT:
        dRow = -1;
        dCol = -1;
        break;
      }
      int steps = 0;
      int row = (position / 8) + dRow;
      int col = (position % 8) + dCol;
      while (0 <= row && row < 8 && 0 <= col && col < 8) {
        row += dRow;
        col += dCol;
        steps++;
      }
      distanceToEdgeLookup[position][directionIdx] = steps;
    }
  }
}

void makeMove(Board board, const Move *move) {
  Piece pieceToMove = board[move->src];
  Piece pieceToTake = board[move->dst];

  // Detect en passant for bitboard update
  // NOTE (cameron): Only check if pawn is moving diagonally to an empty square,
  // trust the move generation to ensure that it's a legal en passant
  Direction direction = move->dst - move->src;
  if ((pieceToMove == (PIECE_BLACK | PIECE_PAWN)) &&
      (pieceToTake == PIECE_NULL)) {
    if (direction == BACKWARD_LEFT) {
      pieceBitBoards[_piece2lookup(PIECE_WHITE | PIECE_PAWN)] &=
          ~((uint64_t)1 << (move->src - 1));
      board[move->src - 1] = PIECE_NULL;

    } else if (direction == BACKWARD_RIGHT) {
      pieceBitBoards[_piece2lookup(PIECE_WHITE | PIECE_PAWN)] &=
          ~((uint64_t)1 << (move->src + 1));
      board[move->src + 1] = PIECE_NULL;
    }
  } else if ((pieceToMove == (PIECE_WHITE | PIECE_PAWN)) &&
             (pieceToTake == PIECE_NULL)) {
    if (direction == FORWARD_LEFT) {
      pieceBitBoards[_piece2lookup(PIECE_BLACK | PIECE_PAWN)] &=
          ~((uint64_t)1 << (move->src - 1));
      board[move->src - 1] = PIECE_NULL;

    } else if (direction == FORWARD_RIGHT) {
      pieceBitBoards[_piece2lookup(PIECE_BLACK | PIECE_PAWN)] &=
          ~((uint64_t)1 << (move->src + 1));
      board[move->src + 1] = PIECE_NULL;
    }
  }

  // TODO (cameron): Detect castling

  // Update moving piece bit board
  pieceBitBoards[_piece2lookup(pieceToMove)] &= ~((uint64_t)1 << move->src);
  pieceBitBoards[_piece2lookup(pieceToMove)] |= ((uint64_t)1 << move->dst);

  // Update taken piece bit board
  if (pieceToTake != PIECE_NULL) {
    pieceBitBoards[_piece2lookup(pieceToTake)] &= ~((uint64_t)1 << move->dst);
  }

  board[move->dst] = pieceToMove;
  board[move->src] = PIECE_NULL;
}

Move strMakeMove(Board board, const char *c) {
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
  return move;
}

static void _populateSlidingMoves(Move *moves[], Board board, Position position,
                                  Piece piece) {
  size_t startDirectionIdx = 0;
  size_t endDirectionIdx = sizeof(_directions) / sizeof(_directions[0]);

  if ((piece & PIECE_TYPE_MASK) == PIECE_BISHOP) {
    startDirectionIdx = 4;
  } else if ((piece & PIECE_TYPE_MASK) == PIECE_ROOK) {
    endDirectionIdx = 4;
  }

  for (size_t directionIdx = startDirectionIdx; directionIdx < endDirectionIdx;
       ++directionIdx) {
    Direction direction = _directions[directionIdx];
    for (int steps = 1; steps <= distanceToEdgeLookup[position][directionIdx];
         ++steps) {
      Position dst = position + (steps * direction);
      if (!_validPosition(dst) || _sameColor(piece, board[dst])) {
        break;
      }
      (*moves)->src = position;
      (*moves)->dst = position + (steps * direction);
      (*moves)++;
    }
  }
}

static void _populatePawnMoves(Move *moves[], Board board, Position position,
                               Piece piece, Move *previousMove) {
  Piece pieceIsBlack = (piece & PIECE_COLOR_MASK) == PIECE_BLACK;

  // En passant as black
  if (pieceIsBlack && _isRank(4, position) && _isRank(4, previousMove->dst) &&
      _isRank(2, previousMove->src) &&
      (abs(position - previousMove->dst) == 1) &&
      ((board[previousMove->dst] & PIECE_TYPE_MASK) == PIECE_PAWN)) {
    Direction direction =
        (position - previousMove->dst) > 0 ? BACKWARD_LEFT : BACKWARD_RIGHT;
    (*moves)->src = position;
    (*moves)->dst = position + direction;
    (*moves)++;
  }

  // En passant as white
  if (!pieceIsBlack && _isRank(5, position) && _isRank(5, previousMove->dst) &&
      _isRank(7, previousMove->src) &&
      (abs(position - previousMove->dst) == 1) &&
      ((board[previousMove->dst]) & PIECE_TYPE_MASK) == PIECE_PAWN) {
    Direction direction =
        (position - previousMove->dst) < 0 ? FORWARD_LEFT : FORWARD_RIGHT;
    (*moves)->src = position;
    (*moves)->dst = position + direction;
    (*moves)++;
  }

  size_t startDirectionIdx = 0;
  size_t endDirectionIdx = sizeof(_pawnDirections) / sizeof(_pawnDirections[0]);
  if (pieceIsBlack) {
    startDirectionIdx = 3;
  } else {
    endDirectionIdx = 3;
  }
  for (size_t directionIdx = startDirectionIdx; directionIdx < endDirectionIdx;
       ++directionIdx) {
    // FIXME (cameron): shouldn't move 2 places sideways
    Direction direction = _pawnDirections[directionIdx];
    if (board[position + direction] == PIECE_NULL) {
      (*moves)->src = position;
      (*moves)->dst = position + direction;
      (*moves)++;
    }
    if ((pieceIsBlack && _isRank(7, position)) ||
        (!pieceIsBlack && _isRank(2, position))) {
      (*moves)->src = position;
      (*moves)->dst = position + (direction * 2);
      (*moves)++;
    }
  }
}

static void _populateKnightMoves(Move *moves[], Board board, Position position,
                                 Piece piece) {
  int srcRow = position / ROWS;
  int srcCol = position % COLS;
  for (size_t knightDirectionIdx = 0;
       knightDirectionIdx < sizeof(_knightDirections) / sizeof(int);
       ++knightDirectionIdx) {
    int direction = _knightDirections[knightDirectionIdx];
    int dstRow = (position + direction) / ROWS;
    int dstCol = (position + direction) % COLS;
    if (((board[position + direction] & PIECE_COLOR_MASK) ==
         (piece & PIECE_COLOR_MASK)) ||
        (dstRow < 0) || (NUM_POSITIONS <= dstRow) ||
        (abs(dstRow - srcRow) > 2) || (abs(dstCol - srcCol) > 2)) {
      continue;
    }
    (*moves)->src = position;
    (*moves)->dst = position + direction;
    (*moves)++;
  }
}

static void _populateKingMoves(Move *moves[], Board board, Position position,
                               Piece piece) {
  for (size_t directionIdx = 0; directionIdx < NUM_DIRECTIONS; ++directionIdx) {
    Direction direction = _directions[directionIdx];
    if ((board[position + direction] & PIECE_COLOR_MASK) ==
            (piece & PIECE_COLOR_MASK) ||
        (position + direction < 0) || (NUM_POSITIONS <= position + direction)) {
      continue;
    }
    (*moves)->src = position;
    (*moves)->dst = position + direction;
    (*moves)++;
  }
}

void generateLegalMoves(MoveList *moveList, Board board, int activeColor,
                        Move *previousMove) {
  // Fill buffer with empty moves
  for (size_t m = 0; m < MAX_CHESS_MOVES; ++m) {
    moveList->moves[m].src = 0;
    moveList->moves[m].dst = 0;
  }
  moveList->count = 0;

  // Generate moves
  Move *movesPtr = moveList->moves;
  for (Position position = 0; position < NUM_POSITIONS; ++position) {
    Piece piece = board[position];
    Piece isBlack = (piece & PIECE_COLOR_MASK & PIECE_BLACK) ? 1 : 0;
    Piece isBlackMove = (activeColor == 0) ? 1 : 0;
    if ((piece != PIECE_NULL) && (isBlack == isBlackMove)) {
      if (piece & SLIDING_PIECE_MASK) {
        _populateSlidingMoves(&movesPtr, board, position, piece);
      } else if (piece & PIECE_PAWN) {
        _populatePawnMoves(&movesPtr, board, position, piece, previousMove);
      } else if (piece & PIECE_KNIGHT) {
        _populateKnightMoves(&movesPtr, board, position, piece);
      } else if (piece & PIECE_KING) {
        _populateKingMoves(&movesPtr, board, position, piece);
      }
    }
  }
  moveList->count = movesPtr - moveList->moves;
}
