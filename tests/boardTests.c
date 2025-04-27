/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "board.h"

#include <unity.h>
#include <unity_fixture.h>

#include "fen.h"
#include "tests.h"

static Board testBoard;

TEST_GROUP(boardTests);

TEST_SETUP(boardTests) { fenPopulateBoard(FEN_STARTING_POSITION, testBoard); }

TEST_TEAR_DOWN(boardTests) { clearBoard(testBoard); }

TEST(boardTests, testMakeMove){SKIP}

TEST(boardTests, testClearBoard) {
  Board board;
  fenPopulateBoard(FEN_STARTING_POSITION, board);
  clearBoard(board);
  for (Position position = 0; position < NUM_POSITIONS; ++position) {
    TEST_ASSERT_EQUAL_UINT8(0, board[position]);
  }
}

TEST(boardTests, testStrMakeMove){SKIP}

TEST(boardTests, testInitBitBoards) {
  initBitBoards(testBoard);
  TEST_ASSERT_EQUAL_UINT64(0x0000000000000081, pieceBitBoards[BLACK_ROOK_KEY]);
  TEST_ASSERT_EQUAL_UINT64(0x0000000000000042,
                           pieceBitBoards[BLACK_KNIGHT_KEY]);
  TEST_ASSERT_EQUAL_UINT64(0x0000000000000024,
                           pieceBitBoards[BLACK_BISHOP_KEY]);
  TEST_ASSERT_EQUAL_UINT64(0x0000000000000008, pieceBitBoards[BLACK_QUEEN_KEY]);
  TEST_ASSERT_EQUAL_UINT64(0x0000000000000010, pieceBitBoards[BLACK_KING_KEY]);
  TEST_ASSERT_EQUAL_UINT64(0x000000000000FF00, pieceBitBoards[BLACK_PAWN_KEY]);
  TEST_ASSERT_EQUAL_UINT64(0x8100000000000000, pieceBitBoards[WHITE_ROOK_KEY]);
  TEST_ASSERT_EQUAL_UINT64(0x4200000000000000,
                           pieceBitBoards[WHITE_KNIGHT_KEY]);
  TEST_ASSERT_EQUAL_UINT64(0x2400000000000000,
                           pieceBitBoards[WHITE_BISHOP_KEY]);
  TEST_ASSERT_EQUAL_UINT64(0x0800000000000000, pieceBitBoards[WHITE_QUEEN_KEY]);
  TEST_ASSERT_EQUAL_UINT64(0x1000000000000000, pieceBitBoards[WHITE_KING_KEY]);
  TEST_ASSERT_EQUAL_UINT64(0x00FF000000000000, pieceBitBoards[WHITE_PAWN_KEY]);
}

TEST_GROUP_RUNNER(boardTests) {
  RUN_TEST_CASE(boardTests, testMakeMove);
  RUN_TEST_CASE(boardTests, testClearBoard);
  RUN_TEST_CASE(boardTests, testStrMakeMove);
  RUN_TEST_CASE(boardTests, testInitBitBoards);
}
