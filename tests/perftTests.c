/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

// perft results from https://www.chessprogramming.org/Perft_Results

#include <unity.h>
#include <unity_fixture.h>

#include "fen.h"
#include "perft.h"

Board board;

TEST_GROUP(perftTests);

TEST_SETUP(perftTests) { fenPopulateBoard(FEN_STARTING_POSITION, board); }

TEST_TEAR_DOWN(perftTests) {}

TEST(perftTests, perft0) {
  struct PerftRes perft0 = perft(board, 1, 0, 0);
  TEST_ASSERT_EQUAL_INT(0, perft0.depth);
  TEST_ASSERT_EQUAL_UINT64(1, perft0.nodes);
  TEST_ASSERT_EQUAL_UINT32(0, perft0.captures);
  TEST_ASSERT_EQUAL_UINT32(0, perft0.castles);
  TEST_ASSERT_EQUAL_UINT32(0, perft0.checks);
  TEST_ASSERT_EQUAL_UINT32(0, perft0.checkMates);
}

TEST(perftTests, perft1) {
  struct PerftRes perft1 = perft(board, 1, 1, 0);
  TEST_ASSERT_EQUAL_INT(1, perft1.depth);
  TEST_ASSERT_EQUAL_UINT64(20, perft1.nodes);
  TEST_ASSERT_EQUAL_UINT32(0, perft1.captures);
  TEST_ASSERT_EQUAL_UINT32(0, perft1.castles);
  TEST_ASSERT_EQUAL_UINT32(0, perft1.checks);
  TEST_ASSERT_EQUAL_UINT32(0, perft1.checkMates);
}

TEST(perftTests, perft2) {
  struct PerftRes perft2 = perft(board, 1, 2, 0);
  TEST_ASSERT_EQUAL_INT(2, perft2.depth);
  TEST_ASSERT_EQUAL_UINT64(400, perft2.nodes);
  TEST_ASSERT_EQUAL_UINT32(0, perft2.captures);
  TEST_ASSERT_EQUAL_UINT32(0, perft2.castles);
  TEST_ASSERT_EQUAL_UINT32(0, perft2.checks);
  TEST_ASSERT_EQUAL_UINT32(0, perft2.checkMates);
}

TEST(perftTests, perft3) {
  struct PerftRes perft3 = perft(board, 1, 3, 0);
  TEST_ASSERT_EQUAL_INT(3, perft3.depth);
  TEST_ASSERT_EQUAL_UINT64(8902, perft3.nodes);
  TEST_ASSERT_EQUAL_UINT32(34, perft3.captures);
  TEST_ASSERT_EQUAL_UINT32(0, perft3.castles);
  TEST_ASSERT_EQUAL_UINT32(12, perft3.checks);
  TEST_ASSERT_EQUAL_UINT32(0, perft3.checkMates);
}

TEST_GROUP_RUNNER(perftTests) {
  RUN_TEST_CASE(perftTests, perft0);
  RUN_TEST_CASE(perftTests, perft1);
  RUN_TEST_CASE(perftTests, perft2);
  RUN_TEST_CASE(perftTests, perft3);
}
