/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include "engine.h"

#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

#include "board.h"
#include "defs.h"

#define PAWN_VALUE 100
#define KNIGHT_VALUE 350
#define BISHOP_VALUE 350
#define ROOK_VALUE 525
#define QUEEN_VALUE 1000
#define KING_VALUE 3000

#define PAWN_PENALTY_MULTIPLIER 50
#define MOBILITY_MULTIPLIER 10

#define MATE 9999
#define STALEMATE 0

const int pieceValueLookup[NUM_PIECE_TYPES] = {KING_VALUE, QUEEN_VALUE, ROOK_VALUE, BISHOP_VALUE, KNIGHT_VALUE, PAWN_VALUE};

static int countPieceValues(Board* board, Color color) {
  BitBoard* pieces = (color == WHITE) ? board->whitePieceBoards : board->blackPieceBoards;
  return (__builtin_popcountll(pieces[PAWN]) * PAWN_VALUE) + (__builtin_popcountll(pieces[KNIGHT]) * KNIGHT_VALUE) +
         (__builtin_popcountll(pieces[BISHOP]) * BISHOP_VALUE) + (__builtin_popcountll(pieces[ROOK]) * ROOK_VALUE) +
         (__builtin_popcountll(pieces[QUEEN]) * QUEEN_VALUE);
}

static int countDoublePawns(Board* board, Color color) {
  return 0;
}

static int countBlockedPawns(Board* board, Color color) {
  return 0;
}

static int countIsolatedPawns(Board* board, Color color) {
  return 0;
}

static int evaluate(Board* board, Color color, unsigned int numMoves) {
  int pieceCountEvaluation = countPieceValues(board, color) - countPieceValues(board, !color);
  int pawnEvaluation = countDoublePawns(board, color) - countDoublePawns(board, !color) + countBlockedPawns(board, color) - countBlockedPawns(board, !color) +
                       countIsolatedPawns(board, color) - countIsolatedPawns(board, !color);
  return pieceCountEvaluation - (PAWN_PENALTY_MULTIPLIER * pawnEvaluation) + (MOBILITY_MULTIPLIER * (int)numMoves);
}

static int scoreMove(Board* board, Move move) {
  PositionIndex src = SRC(move);
  PositionIndex dst = DST(move);
  MoveFlag flag     = FLAG(move);
  if (flag & MOVE_CAPTURE) return pieceValueLookup[board->squares[dst]] - pieceValueLookup[board->squares[src]];
  return 0;
}

static int compareMoves(const void* a, const void* b) {
  if (a == NULL || b == NULL) return INT_MIN;
  return ((ScoredMove*)b)->score - ((ScoredMove*)a)->score;
}

static int quiescentSearch(Board* board, int alpha, int beta) {
  Move moves[MAX_CHESS_MOVES];
  unsigned int numMoves = generateLegalMoves(board, moves);
  bool inCheck          = kingInCheck(board, board->turn);

  if (numMoves == 0) {
    return inCheck ? -MATE : STALEMATE;
  };

  if (!inCheck) {
    int standPat = evaluate(board, board->turn, numMoves);

    if (standPat >= beta) return standPat;

    if (standPat > alpha) alpha = standPat;
  }

  ScoredMove scoredMoves[MAX_CHESS_MOVES];
  for (unsigned int i = 0; i < numMoves; ++i) {
    scoredMoves[i] = (ScoredMove){
        scoreMove(board, moves[i]),
        moves[i],
    };
  }
  qsort(scoredMoves, numMoves, sizeof(ScoredMove), compareMoves);

  for (unsigned int i = 0; i < numMoves; ++i) {
    Move move = scoredMoves[i].move;

    if (!inCheck && ((FLAG(move) & MOVE_CAPTURE) == 0)) continue;

    UndoMove undo;
    boardMakeMove(board, move, &undo);
    int score = -quiescentSearch(board, -beta, -alpha);
    boardUnmakeMove(board, &undo);

    if (score >= beta) return score;
    if (score > alpha) alpha = score;
  }

  return alpha;
}

int search(Board* board, unsigned int depth, Move* bestMove, int alpha, int beta) {
  Move moves[MAX_CHESS_MOVES];
  unsigned int numMoves = generateLegalMoves(board, moves);

  if (numMoves == 0) {
    return kingInCheck(board, board->turn) ? -MATE : STALEMATE;
  };

  if (depth == 0) return quiescentSearch(board, alpha, beta);

  ScoredMove scoredMoves[MAX_CHESS_MOVES];
  for (unsigned int i = 0; i < numMoves; ++i) {
    scoredMoves[i] = (ScoredMove){
        scoreMove(board, moves[i]),
        moves[i],
    };
  }
  qsort(scoredMoves, numMoves, sizeof(ScoredMove), compareMoves);

  int bestScore      = INT_MIN;
  Move localBestMove = (Move)0;

  for (unsigned int i = 0; i < numMoves; ++i) {
    Move move = scoredMoves[i].move;
    UndoMove undo;

    boardMakeMove(board, move, &undo);
    int score = -search(board, depth - 1, NULL, -beta, -alpha);
    boardUnmakeMove(board, &undo);

    if (score > bestScore) {
      bestScore     = score;
      localBestMove = move;
      if (score > alpha) alpha = score;
    }
    if (score >= beta) {
      if (bestMove != NULL) *bestMove = move;
      return score;
    };
  }

  if (bestMove != NULL) *bestMove = localBestMove;

  return bestScore;
}
