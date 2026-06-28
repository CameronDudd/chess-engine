/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#define _POSIX_C_SOURCE 199309L  // Required for clock_getttime

#include "engine.h"

#include <limits.h>
#include <log.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

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

#define S_TO_NS 100000000ULL
#define MS_TO_NS 100000ULL

const int pieceValueLookup[NUM_PIECE_TYPES]                          = {KING_VALUE, QUEEN_VALUE, ROOK_VALUE, BISHOP_VALUE, KNIGHT_VALUE, PAWN_VALUE};
TranspositionTableEntry transpositionTable[TRANSPOSITION_TABLE_SIZE] = {0};

static uint64_t monoTimeNs(void) {
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC, &tp);
  return (uint64_t)((tp.tv_sec * S_TO_NS) + tp.tv_nsec);
}

static int countPieceValues(BitBoard* pieces) {
  return (getPopCount(pieces[PAWN]) * PAWN_VALUE) + (getPopCount(pieces[KNIGHT]) * KNIGHT_VALUE) + (getPopCount(pieces[BISHOP]) * BISHOP_VALUE) +
         (getPopCount(pieces[ROOK]) * ROOK_VALUE) + (getPopCount(pieces[QUEEN]) * QUEEN_VALUE);
}

// TODO: Create pre-computed pawn lookahead table for all pawn counters
// 1.
static int countDoublePawns(BitBoard pawns) {
  int sum          = 0;
  BitBoard scanned = pawns;
  while (scanned) {
    PositionIndex pos = popLSB(&scanned);
    int file          = INDEX_FILE(pos);
    if (getPopCount(pawns & files[file]) > 1) sum += 1;
  }
  return sum;
}

// TODO: 2.
static int countBlockedPawns(BitBoard pawns) {
  int sum = 0;
  for (unsigned int fileIdx = 0; fileIdx < NUM_FILES; ++fileIdx) {
    int numPawns = getPopCount(pawns & files[fileIdx]);
    if (numPawns > 1) sum += numPawns - 1;
  }
  return sum;
}

// TODO: 3.
static int countIsolatedPawns(BitBoard pawns) {
  int sum = 0;
  for (size_t fileIdx = 0; fileIdx < NUM_FILES; ++fileIdx) {
    int filePawns = getPopCount(pawns & files[fileIdx]);

    if (filePawns == 0) continue;

    BitBoard neighborMask = (BitBoard)0;
    if (fileIdx > 0) neighborMask |= files[fileIdx - 1];
    if (fileIdx < MAX_FILE) neighborMask |= files[fileIdx + 1];

    if (getPopCount(pawns & neighborMask) == 0) sum += filePawns;
  }
  return sum;
}

static int evaluate(Board* board, Color color, unsigned int numMoves) {
  BitBoard* friendlyPieces = (color == WHITE) ? board->whitePieceBoards : board->blackPieceBoards;
  BitBoard* enemyPieces    = (color == WHITE) ? board->blackPieceBoards : board->whitePieceBoards;

  BitBoard friendlyPawns = friendlyPieces[PAWN];
  BitBoard enemyPawns    = enemyPieces[PAWN];

  int pieceCountEvaluation = countPieceValues(friendlyPieces) - countPieceValues(enemyPieces);

  int pawnEvaluation = countDoublePawns(friendlyPawns) - countDoublePawns(enemyPawns) + countBlockedPawns(friendlyPawns) - countBlockedPawns(enemyPawns) +
                       countIsolatedPawns(friendlyPawns) - countIsolatedPawns(enemyPawns);

  return pieceCountEvaluation - (PAWN_PENALTY_MULTIPLIER * pawnEvaluation) + (MOBILITY_MULTIPLIER * (int)numMoves);
}

static int scoreMove(Board* board, Move move) {
  PositionIndex src = SRC(move);
  PositionIndex dst = DST(move);
  Piece attacker    = board->squares[src];
  Piece defender    = board->squares[dst];
  if (moveCapture(board, move)) return pieceValueLookup[defender] - pieceValueLookup[attacker];
  return 0;
}

static int compareMoves(const void* a, const void* b) {
  if (a == NULL || b == NULL) return -MATE;
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
    if (!inCheck && !moveCapture(board, move)) continue;
    UndoMove undo;
    boardMakeMove(board, move, &undo);
    int score = -quiescentSearch(board, -beta, -alpha);
    boardUnmakeMove(board, &undo);

    if (score >= beta) return score;
    if (score > alpha) alpha = score;
  }

  return alpha;
}

int search(Board* board, int ply, unsigned int depth, int alpha, int beta, Move* bestMove,  // NOLINT(readability-function-cognitive-complexity)
           SearchTimer* searchTimer) {
  int originalAlpha              = alpha;
  ZobristHash transpositionIndex = board->zobrist & (TRANSPOSITION_TABLE_SIZE - 1);
  TranspositionTableEntry tte    = transpositionTable[transpositionIndex];

  if ((tte.zobrist == board->zobrist) && (tte.depth >= depth)) {
    if (tte.type == TRANSPOSITION_TYPE_EXACT) {
      if (ply == 0 && bestMove != NULL) *bestMove = tte.move;
      return tte.score;
    }
    if (tte.type == TRANSPOSITION_TYPE_LOWERBOUND && tte.score >= beta) return beta;
    if (tte.type == TRANSPOSITION_TYPE_UPPERBOUND && tte.score <= alpha) return alpha;
  }

  // TODO: Don't check timeout every call
  if (monoTimeNs() >= searchTimer->endTimeNs) {
    searchTimer->searchInterrupted = true;
    return -MATE;
  }

  Move moves[MAX_CHESS_MOVES];
  unsigned int numMoves = generateLegalMoves(board, moves);

  if (numMoves == 0) {
    return kingInCheck(board, board->turn) ? -MATE + ply : STALEMATE;
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

  int localBestScore = -MATE;
  Move localBestMove = (Move)0;

  for (unsigned int i = 0; i < numMoves; ++i) {
    Move move = scoredMoves[i].move;
    UndoMove undo;

    // Make the move and opponent evaluates new position
    boardMakeMove(board, move, &undo);
    int score = -search(board, ply + 1, depth - 1, -beta, -alpha, NULL, searchTimer);
    boardUnmakeMove(board, &undo);

    if (searchTimer->searchInterrupted) break;

    if (score > localBestScore) {
      localBestScore = score;
      localBestMove  = move;
    }

    // This branch found a move better than what the opponent will allow so no need to search further
    if (score >= beta) break;  // Don't bother searching further

    // Found a better move
    if (score > alpha) alpha = score;
  }

  if ((ply == 0) && (bestMove != NULL)) *bestMove = localBestMove;

  if (!searchTimer->searchInterrupted) {
    TranspositionEntryType tteType = TRANSPOSITION_TYPE_EXACT;
    if (localBestScore >= beta) {
      tteType = TRANSPOSITION_TYPE_LOWERBOUND;
    } else if (localBestScore <= originalAlpha) {
      tteType = TRANSPOSITION_TYPE_UPPERBOUND;
    }

    transpositionTable[transpositionIndex].zobrist = board->zobrist;
    transpositionTable[transpositionIndex].move    = localBestMove;
    transpositionTable[transpositionIndex].depth   = depth;
    transpositionTable[transpositionIndex].type    = tteType;
    transpositionTable[transpositionIndex].score   = localBestScore;
  }

  return localBestScore;
}

SearchResult iterativeSearch(Board* board, unsigned int depthLimit, unsigned int timeLimitMs) {
  SearchTimer searchTimer = {
      .endTimeNs         = monoTimeNs() + (timeLimitMs * MS_TO_NS),
      .searchInterrupted = false,
  };

  SearchResult searchResult;

  int iterBestScore = 0;
  Move iterBestMove = (Move)0;
  for (unsigned int depth = 1; depth <= depthLimit; ++depth) {
    if (monoTimeNs() >= searchTimer.endTimeNs) break;
    iterBestScore = search(board, 0, depth, -MATE, MATE, &iterBestMove, &searchTimer);
    if (searchTimer.searchInterrupted) break;
    searchResult.move  = iterBestMove;
    searchResult.score = iterBestScore;
  }

  return searchResult;
}
