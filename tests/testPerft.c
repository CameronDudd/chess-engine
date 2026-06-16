/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include <unity_fixture.h>

#include "board.h"
#include "fen.h"
#include "perft.h"

Board board;

TEST_GROUP(testPerft);

TEST_SETUP(testPerft) {
  initBoard(&board);
  fenPopulateBoard(FEN_STARTING_POSITION, &board);
}

TEST_TEAR_DOWN(testPerft) {
}

TEST(testPerft, perft0) {
  PerftResult result = perft(&board, 0, false);
  TEST_ASSERT_EQUAL_UINT64(1, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(0, result.captures);
  TEST_ASSERT_EQUAL_UINT64(0, result.ep);
  TEST_ASSERT_EQUAL_UINT64(0, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(0, result.checks);
  TEST_ASSERT_EQUAL_UINT64(0, result.checkmates);
}

TEST(testPerft, perft1) {
  PerftResult result = perft(&board, 1, false);
  TEST_ASSERT_EQUAL_UINT64(20, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(0, result.captures);
  TEST_ASSERT_EQUAL_UINT64(0, result.ep);
  TEST_ASSERT_EQUAL_UINT64(0, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(0, result.checks);
  TEST_ASSERT_EQUAL_UINT64(0, result.checkmates);
}

TEST(testPerft, perft2) {
  PerftResult result = perft(&board, 2, false);
  TEST_ASSERT_EQUAL_UINT64(400, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(0, result.captures);
  TEST_ASSERT_EQUAL_UINT64(0, result.ep);
  TEST_ASSERT_EQUAL_UINT64(0, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(0, result.checks);
  TEST_ASSERT_EQUAL_UINT64(0, result.checkmates);
}

TEST(testPerft, perft3) {
  PerftResult result = perft(&board, 3, false);
  TEST_ASSERT_EQUAL_UINT64(8902, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(34, result.captures);
  TEST_ASSERT_EQUAL_UINT64(0, result.ep);
  TEST_ASSERT_EQUAL_UINT64(0, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(12, result.checks);
  TEST_ASSERT_EQUAL_UINT64(0, result.checkmates);
}

TEST(testPerft, perft4) {
  PerftResult result = perft(&board, 4, false);
  TEST_ASSERT_EQUAL_UINT64(197281, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(1576, result.captures);
  TEST_ASSERT_EQUAL_UINT64(0, result.ep);
  TEST_ASSERT_EQUAL_UINT64(0, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(469, result.checks);
  TEST_ASSERT_EQUAL_UINT64(8, result.checkmates);
}

TEST(testPerft, perft5) {
  PerftResult result = perft(&board, 5, false);
  TEST_ASSERT_EQUAL_UINT64(4865609, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(82719, result.captures);
  TEST_ASSERT_EQUAL_UINT64(258, result.ep);
  TEST_ASSERT_EQUAL_UINT64(0, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(27351, result.checks);
  TEST_ASSERT_EQUAL_UINT64(347, result.checkmates);
}

TEST_GROUP_RUNNER(testPerft) {
  RUN_TEST_CASE(testPerft, perft0);
  RUN_TEST_CASE(testPerft, perft1);
  RUN_TEST_CASE(testPerft, perft2);
  RUN_TEST_CASE(testPerft, perft3);
  RUN_TEST_CASE(testPerft, perft4);
  RUN_TEST_CASE(testPerft, perft5);
}
