/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "board.h"
#include "piece.h"
#include "utils.h"

#include <log.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const Direction _directions[NUM_DIRECTIONS] = {
    FORWARD,       BACKWARD,       LEFT,          RIGHT,
    FORWARD_RIGHT, BACKWARD_RIGHT, BACKWARD_LEFT, FORWARD_LEFT};

static const Direction _pawnAttackingDirections[NUM_PAWN_DIRECTIONS] = {
    FORWARD_LEFT,
    FORWARD_RIGHT,
    BACKWARD_LEFT,
    BACKWARD_RIGHT,
};

static const int _knightDirections[] = {-17, -15, -10, -6, 6, 10, 15, 17};

int distanceToEdgeLookup[NUM_POSITIONS][NUM_DIRECTIONS] = {0};

BitBoard pieceBitBoards[NUM_UNIQUE_PIECES] = {
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
};

static char _piece2char(const Piece piece) __attribute__((unused));
static char _piece2char(const Piece piece) {
  switch (piece) {
  case PIECE_BLACK | PIECE_ROOK:
    return 'r';
  case PIECE_BLACK | PIECE_KNIGHT:
    return 'n';
  case PIECE_BLACK | PIECE_BISHOP:
    return 'b';
  case PIECE_BLACK | PIECE_QUEEN:
    return 'q';
  case PIECE_BLACK | PIECE_KING:
    return 'k';
  case PIECE_BLACK | PIECE_PAWN:
    return 'p';
  case PIECE_WHITE | PIECE_ROOK:
    return 'R';
  case PIECE_WHITE | PIECE_KNIGHT:
    return 'N';
  case PIECE_WHITE | PIECE_BISHOP:
    return 'B';
  case PIECE_WHITE | PIECE_QUEEN:
    return 'Q';
  case PIECE_WHITE | PIECE_KING:
    return 'K';
  case PIECE_WHITE | PIECE_PAWN:
    return 'P';
  default:
    log_error("Invalid piece '%i' '%s'", piece, uint8t2str(piece));
    exit(1);
  }
}

static char *_piece2str(const Piece piece) __attribute__((unused));
static char *_piece2str(const Piece piece) {
  static char str[100] = {'\0'};
  memset(str, '\0', 100);
  switch (piece) {
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
    log_error("Invalid piece '%i' '%s'", piece, uint8t2str(piece));
    exit(1);
  }
  return str;
}

Position coordinate2pos(const char coordinate[]) {
  // c3
  return (8 * ('8' - coordinate[1])) + (coordinate[0] - 'a');
}

const char *move2str(const Move move, const Board board) {
  uint8_t flag = MOVE_FLAGS(move);
  uint8_t src = MOVE_SRC(move);
  uint8_t dst = MOVE_DST(move);

  static char str[MOVE_STRING_MAX] = {'\0'};
  char *s = str;

  if (flag & CASTLE) {
    if ((dst - src) > 0) {
      strcpy(str, "O-O");
      s += strlen("O-O");
    } else {
      strcpy(str, "O-O-O");
      s += strlen("O-O-O");
    }
  } else {
    Piece srcPiece = board[src];
    if (!(srcPiece & PIECE_TYPE_MASK & PIECE_PAWN)) {
      *s++ = _piece2char(board[src]);
    }

    // Capture
    if (flag & CAPTURE) {
      if (srcPiece & PIECE_TYPE_MASK & PIECE_PAWN) {
        *s++ = 'a' + ((src) % 8);
      }
      *s++ = 'x';
    }

    // Target Square
    *s++ = 'a' + ((dst) % 8);
    *s++ = '1' + 7 - ((dst) / 8);
  }

  // Check
  if (flag & KING_CHECK) {
    *s++ = '+';
  } else if (flag & KING_CHECKMATE) {
    *s++ = '#';
  }

  // Terminate the string
  *s++ = '\0';

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
    log_error("Invalid piece '%i' '%s'", piece, uint8t2str(piece));
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

static inline bool _isBlack(Piece piece) {
  return (piece & PIECE_COLOR_MASK) == PIECE_BLACK;
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

void doMove(Board board, const Move move) {
  // FIXME (cameron): update to Move means a lot of this will be easier
  uint8_t src = MOVE_SRC(move);
  uint8_t dst = MOVE_DST(move);
  Piece pieceToMove = board[src];
  Piece pieceToTake = board[dst];

  // Detect en passant for bitboard update
  // NOTE (cameron): Only check if pawn is moving diagonally to an empty square,
  // trust the move generation to ensure that it's a legal en passant
  Direction direction = dst - src;
  if ((pieceToMove == (PIECE_BLACK | PIECE_PAWN)) &&
      (pieceToTake == PIECE_NULL)) {
    if (direction == BACKWARD_LEFT) {
      pieceBitBoards[_piece2lookup(PIECE_WHITE | PIECE_PAWN)] &=
          ~((uint64_t)1 << (src - 1));
      board[src - 1] = PIECE_NULL;

    } else if (direction == BACKWARD_RIGHT) {
      pieceBitBoards[_piece2lookup(PIECE_WHITE | PIECE_PAWN)] &=
          ~((uint64_t)1 << (src + 1));
      board[src + 1] = PIECE_NULL;
    }
  } else if ((pieceToMove == (PIECE_WHITE | PIECE_PAWN)) &&
             (pieceToTake == PIECE_NULL)) {
    if (direction == FORWARD_LEFT) {
      pieceBitBoards[_piece2lookup(PIECE_BLACK | PIECE_PAWN)] &=
          ~((uint64_t)1 << (src - 1));
      board[src - 1] = PIECE_NULL;

    } else if (direction == FORWARD_RIGHT) {
      pieceBitBoards[_piece2lookup(PIECE_BLACK | PIECE_PAWN)] &=
          ~((uint64_t)1 << (src + 1));
      board[src + 1] = PIECE_NULL;
    }
  }

  // TODO (cameron): Detect castling

  // Update moving piece bit board
  pieceBitBoards[_piece2lookup(pieceToMove)] &= ~((uint64_t)1 << src);
  pieceBitBoards[_piece2lookup(pieceToMove)] |= ((uint64_t)1 << dst);

  // Update taken piece bit board
  if (pieceToTake != PIECE_NULL) {
    pieceBitBoards[_piece2lookup(pieceToTake)] &= ~((uint64_t)1 << dst);
  }

  board[dst] = pieceToMove;
  board[src] = PIECE_NULL;
}

// TODO (cameron): update bitboards
void undoMove(Board board, const Move move) {
  uint8_t src = MOVE_SRC(move);
  uint8_t dst = MOVE_DST(move);
  Piece tmp = board[src];
  board[src] = board[dst];
  board[dst] = tmp;
}

Move strMakeMove(Board board, const char *c) {
  if (!(('A' <= c[0]) && (c[0] <= 'H')) && !(('1' <= c[1]) && (c[1] <= '8')) &&
      !(('A' <= c[2]) && (c[2] <= 'H')) && !(('1' <= c[3]) && (c[3] <= '8'))) {
    log_error("Invalid move string '%s'", c);
    exit(1);
  }
  Move move = MOVE_ENCODE((7 - (c[1] - '1')) * 8 + (7 - ('H' - c[0])),
                          (7 - (c[3] - '1')) * 8 + (7 - ('H' - c[2])), 0);
  doMove(board, move);
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
      if (_validPosition(dst) && !_sameColor(piece, board[dst])) {
        **moves = MOVE_ENCODE(position, dst, 0);
        (*moves)++;
      }
    }
  }
}

static void _populatePawnMoves(Move *moves[], Board board, Position position,
                               Piece piece, Move previousMove) {
  Position dst;
  Direction direction;

  bool isBlack = _isBlack(piece);
  direction = isBlack ? BACKWARD : FORWARD;

  // Normal move
  dst = position + direction;
  if (_validPosition(dst) && board[dst] == PIECE_NULL) {
    **moves = MOVE_ENCODE(position, dst, 0);
    (*moves)++;

    // Double move
    dst += direction;
    bool isStartRank = isBlack ? _isRank(7, position) : _isRank(2, position);
    if (isStartRank && _validPosition(dst) && board[dst] == PIECE_NULL) {
      **moves = MOVE_ENCODE(position, dst, 0);
      (*moves)++;
    }
  }

  // Attacks and en passant
  size_t startIdx = isBlack ? 2 : 0;
  size_t endIdx = isBlack ? 4 : 2;
  for (size_t i = startIdx; i < endIdx; ++i) {
    direction = _pawnAttackingDirections[i];
    dst = position + direction;
    if (!_validPosition(dst)) {
      continue;
    }
    Piece target = board[dst];

    // Normal capture
    if ((target != PIECE_NULL) &&
        ((target & PIECE_COLOR_MASK) != (piece & PIECE_COLOR_MASK))) {
      **moves = MOVE_ENCODE(position, dst, 0);
      (*moves)++;
    }

    // En passant
    Position epSrc = MOVE_SRC(previousMove);
    Position epDst = MOVE_DST(previousMove);
    Piece epPiece = board[epDst];
    bool isEnemyPawn =
        ((epPiece & PIECE_TYPE_MASK) == PIECE_PAWN) &&
        ((epPiece & PIECE_COLOR_MASK) != (piece & PIECE_COLOR_MASK));
    bool movedTwoSquares = abs((int)epDst - (int)epSrc) == 16;
    bool epTargetMatches = epDst == dst;
    if (isEnemyPawn && movedTwoSquares && epTargetMatches) {
      **moves = MOVE_ENCODE(position, dst, 0);
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
    Position dst = position + direction;
    int dstRow = dst / ROWS;
    int dstCol = dst % COLS;
    bool canMove =
        (board[dst] & PIECE_COLOR_MASK) != (piece & PIECE_COLOR_MASK);
    bool validPosition = _validPosition(dst) && (abs(dstRow - srcRow) <= 2) &&
                         (abs(dstCol - srcCol) <= 2);
    if (canMove && validPosition) {
      **moves = MOVE_ENCODE(position, dst, 0);
      (*moves)++;
    }
  }
}

static void _populateKingMoves(Move *moves[], Board board, Position position,
                               Piece piece) {
  for (size_t directionIdx = 0; directionIdx < NUM_DIRECTIONS; ++directionIdx) {
    Direction direction = _directions[directionIdx];
    Position dst = position + direction;
    bool canMove =
        (board[dst] & PIECE_COLOR_MASK) != (piece & PIECE_COLOR_MASK);
    bool validPosition = _validPosition(dst);
    if (canMove & validPosition) {
      **moves = MOVE_ENCODE(position, dst, 0);
      (*moves)++;
    }
  }
}

void generateLegalMoves(MoveList *moveList, Board board, int activeColor,
                        Move previousMove) {
  // FIXME (cameron): Checks
  memset(moveList->moves, 0, sizeof(Move) * MAX_CHESS_MOVES);
  Move *moves = moveList->moves;
  for (Position position = 0; position < NUM_POSITIONS; ++position) {
    Piece piece = board[position];
    Piece isBlack = (piece & PIECE_COLOR_MASK & PIECE_BLACK) ? 1 : 0;
    Piece isBlackMove = (activeColor == 0) ? 1 : 0;
    if ((piece != PIECE_NULL) && (isBlack == isBlackMove)) {
      if (piece & SLIDING_PIECE_MASK) {
        _populateSlidingMoves(&moves, board, position, piece);
      } else if (piece & PIECE_PAWN) {
        _populatePawnMoves(&moves, board, position, piece, previousMove);
      } else if (piece & PIECE_KNIGHT) {
        _populateKnightMoves(&moves, board, position, piece);
      } else if (piece & PIECE_KING) {
        _populateKingMoves(&moves, board, position, piece);
      }
    }
  }
  moveList->count = moves - moveList->moves;
}
