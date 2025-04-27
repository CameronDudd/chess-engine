/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <time.h>
#include <unity_fixture.h>

static double deltaMs(struct timespec start, struct timespec end) {
  return 1000 * 1000 *
         ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9);
}

static void _runAllTests(void) {
  struct timespec start, end;

  // FEN
  printf("\nFEN Tests: ");
  clock_gettime(CLOCK_MONOTONIC, &start);
  RUN_TEST_GROUP(fenTests);
  clock_gettime(CLOCK_MONOTONIC, &end);
  printf(" (%.3f us)", deltaMs(start, end));

  // PIECE
  printf("\nPiece Tests: ");
  clock_gettime(CLOCK_MONOTONIC, &start);
  RUN_TEST_GROUP(pieceTests);
  clock_gettime(CLOCK_MONOTONIC, &end);
  printf(" (%.3f us)", deltaMs(start, end));

  // EVALUATION
  printf("\nEvaluation Tests: ");
  clock_gettime(CLOCK_MONOTONIC, &start);
  RUN_TEST_GROUP(evaluationTests);
  clock_gettime(CLOCK_MONOTONIC, &end);
  printf(" (%.3f us)", deltaMs(start, end));

  // BOARD
  printf("\nBoard Tests: ");
  clock_gettime(CLOCK_MONOTONIC, &start);
  RUN_TEST_GROUP(boardTests);
  clock_gettime(CLOCK_MONOTONIC, &end);
  printf(" (%.3f us)", deltaMs(start, end));
}

int main(int argc, const char *argv[]) {
  return UnityMain(argc, argv, _runAllTests);
}
