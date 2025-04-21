/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "fen.h"

#include <unity.h>
#include <unity_fixture.h>

TEST_GROUP(fenTests);

TEST_SETUP(fenTests) {}

TEST_TEAR_DOWN(fenTests) {}

TEST(fenTests, testStartingPosition) {
  TEST_ASSERT_EQUAL_STRING(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
      FEN_STARTING_POSITION);
}

TEST(fenTests, testFenPiece) { // FIXME (cameron): current failing
  // TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_PAWN, fenPiece("p"));
  // TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_ROOK, fenPiece("r"));
}

TEST(fenTests, testFenWhitespace) {
  TEST_ASSERT_EQUAL_INT(0, fenWhitespace("0"));
  TEST_ASSERT_EQUAL_INT(1, fenWhitespace("1"));
  TEST_ASSERT_EQUAL_INT(2, fenWhitespace("2"));
}

TEST(fenTests, testFenWhitespaceBadString) {
  TEST_ASSERT_EQUAL_INT(0, fenWhitespace("string"));
  TEST_ASSERT_EQUAL_INT(0, fenWhitespace("a"));
  TEST_ASSERT_EQUAL_INT(0, fenWhitespace("-1"));
}

TEST_GROUP_RUNNER(fenTests) {
  RUN_TEST_CASE(fenTests, testStartingPosition);
  RUN_TEST_CASE(fenTests, testFenPiece);
  RUN_TEST_CASE(fenTests, testFenWhitespace);
  RUN_TEST_CASE(fenTests, testFenWhitespaceBadString);
}
