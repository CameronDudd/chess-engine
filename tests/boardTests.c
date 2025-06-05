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

TEST(boardTests, testMoveEncode){SKIP}

TEST(boardTests, testWhiteMove2Str) {
  Board board;

  fenPopulateBoard(FEN_STARTING_POSITION, board);
  Move pawnMove = MOVE_ENCODE(52, 36, 0);
  TEST_ASSERT_EQUAL_STRING("e4", move2str(pawnMove, board));

  fenPopulateBoard(
      "rnbqkb1r/ppp1pppp/5n2/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", board);
  Move pawnCaptureKnight = MOVE_ENCODE(28, 21, CAPTURE);
  TEST_ASSERT_EQUAL_STRING("exf6", move2str(pawnCaptureKnight, board));

  fenPopulateBoard(
      "r1bqkbnr/1ppp1ppp/p1n5/4p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 0 1",
      board);
  Move shortCastle = MOVE_ENCODE(60, 62, CASTLE);
  TEST_ASSERT_EQUAL_STRING("O-O", move2str(shortCastle, board));

  fenPopulateBoard(
      "rnbqk1nr/ppp1b1pp/3p4/4p1p1/4P1Q1/2NP4/PPP2PPP/2KR1BNR b kq - 0 1",
      board);
  Move longCastle = MOVE_ENCODE(60, 58, CASTLE);
  TEST_ASSERT_EQUAL_STRING("O-O-O", move2str(longCastle, board));
}

TEST(boardTests, testBlackMove2Str) {
  Board board;

  fenPopulateBoard(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1", board);
  Move pawnMove = MOVE_ENCODE(12, 28, 0);
  TEST_ASSERT_EQUAL_STRING("", move2str(pawnMove, board));

  // fenPopulateBoard("", board);
  // Move pawnCaptureKnight = MOVE_ENCODE(0, 0, 0);
  // TEST_ASSERT_EQUAL_STRING("", move2str(pawnCaptureKnight, board));

  // fenPopulateBoard("", board);
  // Move shortCastle = {.src = 0, .dst = 0, .flag = O_O};
  // TEST_ASSERT_EQUAL_STRING("", move2str(&shortCastle, board));

  // fenPopulateBoard("", board);
  // Move longCastle = {.src = 0, .dst = 0, .flag = O_O_O};
  // TEST_ASSERT_EQUAL_STRING("", move2str(&longCastle, board));
}

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
  RUN_TEST_CASE(boardTests, testMoveEncode);
  RUN_TEST_CASE(boardTests, testWhiteMove2Str);
  RUN_TEST_CASE(boardTests, testMakeMove);
  RUN_TEST_CASE(boardTests, testClearBoard);
  RUN_TEST_CASE(boardTests, testStrMakeMove);
  RUN_TEST_CASE(boardTests, testInitBitBoards);
}
