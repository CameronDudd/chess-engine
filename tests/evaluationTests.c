/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "evaluation.h"
#include "fen.h"

#include <unity.h>
#include <unity_fixture.h>

TEST_GROUP(evaluationTests);

TEST_SETUP(evaluationTests) {}

TEST_TEAR_DOWN(evaluationTests) {}

TEST(evaluationTests, testEvaluate) {
  Board board;
  fenPopulateBoard(FEN_STARTING_POSITION, board);
  TEST_ASSERT_EQUAL_INT(0, evaluate(board));
}

TEST_GROUP_RUNNER(evaluationTests) {
  RUN_TEST_CASE(evaluationTests, testEvaluate);
}
