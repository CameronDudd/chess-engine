/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "board.h"
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

TEST(fenTests, testFenPiece) {
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_PAWN, fenPiece("p"));
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_ROOK, fenPiece("r"));
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_KNIGHT, fenPiece("n"));
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_BISHOP, fenPiece("b"));
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_QUEEN, fenPiece("q"));
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_KING, fenPiece("k"));
  TEST_ASSERT_EQUAL_UINT8(PIECE_WHITE | PIECE_PAWN, fenPiece("P"));
  TEST_ASSERT_EQUAL_UINT8(PIECE_WHITE | PIECE_ROOK, fenPiece("R"));
  TEST_ASSERT_EQUAL_UINT8(PIECE_WHITE | PIECE_KNIGHT, fenPiece("N"));
  TEST_ASSERT_EQUAL_UINT8(PIECE_WHITE | PIECE_BISHOP, fenPiece("B"));
  TEST_ASSERT_EQUAL_UINT8(PIECE_WHITE | PIECE_QUEEN, fenPiece("Q"));
  TEST_ASSERT_EQUAL_UINT8(PIECE_WHITE | PIECE_KING, fenPiece("K"));
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

TEST(fenTests, fenActiveColor) {
  TEST_ASSERT_EQUAL_INT(0, fenActiveColor(" b"));
  TEST_ASSERT_EQUAL_INT(1, fenActiveColor(" w"));
  TEST_ASSERT_EQUAL_INT(1, fenActiveColor(FEN_STARTING_POSITION));
}

TEST(fenTests, fenPopulateBoard) {
  Piece board[NUM_POSITIONS];
  fenPopulateBoard(FEN_STARTING_POSITION, board);
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_ROOK, board[0]);
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_KNIGHT, board[1]);
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_BISHOP, board[2]);
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_QUEEN, board[3]);
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_KING, board[4]);
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_BISHOP, board[5]);
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_KNIGHT, board[6]);
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_ROOK, board[7]);
  for (int i = 8; i < 16; i++) {
    TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_PAWN, board[i]);
  }
  for (int i = 17; i < 48; i++) {
    TEST_ASSERT_EQUAL_UINT8(PIECE_NULL, board[i]);
  }
  for (int i = 48; i < 54; i++) {
    TEST_ASSERT_EQUAL_UINT8(PIECE_WHITE | PIECE_PAWN, board[i]);
  }
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_ROOK, board[0]);
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_KNIGHT, board[1]);
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_BISHOP, board[2]);
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_QUEEN, board[3]);
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_KING, board[4]);
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_BISHOP, board[5]);
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_KNIGHT, board[6]);
  TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_ROOK, board[7]);
}

TEST_GROUP_RUNNER(fenTests) {
  RUN_TEST_CASE(fenTests, testStartingPosition);
  RUN_TEST_CASE(fenTests, testFenPiece);
  RUN_TEST_CASE(fenTests, testFenWhitespace);
  RUN_TEST_CASE(fenTests, testFenWhitespaceBadString);
  RUN_TEST_CASE(fenTests, fenActiveColor);
  RUN_TEST_CASE(fenTests, fenPopulateBoard);
}
