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

TEST(boardTests, testCoordinate2Pos) {
  // ROW 1
  TEST_ASSERT_EQUAL_INT(56, coordinate2pos("a1"));
  TEST_ASSERT_EQUAL_INT(57, coordinate2pos("b1"));
  TEST_ASSERT_EQUAL_INT(58, coordinate2pos("c1"));
  TEST_ASSERT_EQUAL_INT(59, coordinate2pos("d1"));
  TEST_ASSERT_EQUAL_INT(60, coordinate2pos("e1"));
  TEST_ASSERT_EQUAL_INT(61, coordinate2pos("f1"));
  TEST_ASSERT_EQUAL_INT(62, coordinate2pos("g1"));
  TEST_ASSERT_EQUAL_INT(63, coordinate2pos("h1"));

  // ROW 2
  TEST_ASSERT_EQUAL_INT(48, coordinate2pos("a2"));
  TEST_ASSERT_EQUAL_INT(49, coordinate2pos("b2"));
  TEST_ASSERT_EQUAL_INT(50, coordinate2pos("c2"));
  TEST_ASSERT_EQUAL_INT(51, coordinate2pos("d2"));
  TEST_ASSERT_EQUAL_INT(52, coordinate2pos("e2"));
  TEST_ASSERT_EQUAL_INT(53, coordinate2pos("f2"));
  TEST_ASSERT_EQUAL_INT(54, coordinate2pos("g2"));
  TEST_ASSERT_EQUAL_INT(55, coordinate2pos("h2"));

  // ROW 3
  TEST_ASSERT_EQUAL_INT(40, coordinate2pos("a3"));
  TEST_ASSERT_EQUAL_INT(41, coordinate2pos("b3"));
  TEST_ASSERT_EQUAL_INT(42, coordinate2pos("c3"));
  TEST_ASSERT_EQUAL_INT(43, coordinate2pos("d3"));
  TEST_ASSERT_EQUAL_INT(44, coordinate2pos("e3"));
  TEST_ASSERT_EQUAL_INT(45, coordinate2pos("f3"));
  TEST_ASSERT_EQUAL_INT(46, coordinate2pos("g3"));
  TEST_ASSERT_EQUAL_INT(47, coordinate2pos("h3"));

  // ROW 4
  TEST_ASSERT_EQUAL_INT(32, coordinate2pos("a4"));
  TEST_ASSERT_EQUAL_INT(33, coordinate2pos("b4"));
  TEST_ASSERT_EQUAL_INT(34, coordinate2pos("c4"));
  TEST_ASSERT_EQUAL_INT(35, coordinate2pos("d4"));
  TEST_ASSERT_EQUAL_INT(36, coordinate2pos("e4"));
  TEST_ASSERT_EQUAL_INT(37, coordinate2pos("f4"));
  TEST_ASSERT_EQUAL_INT(38, coordinate2pos("g4"));
  TEST_ASSERT_EQUAL_INT(39, coordinate2pos("h4"));

  // ROW 5
  TEST_ASSERT_EQUAL_INT(24, coordinate2pos("a5"));
  TEST_ASSERT_EQUAL_INT(25, coordinate2pos("b5"));
  TEST_ASSERT_EQUAL_INT(26, coordinate2pos("c5"));
  TEST_ASSERT_EQUAL_INT(27, coordinate2pos("d5"));
  TEST_ASSERT_EQUAL_INT(28, coordinate2pos("e5"));
  TEST_ASSERT_EQUAL_INT(29, coordinate2pos("f5"));
  TEST_ASSERT_EQUAL_INT(30, coordinate2pos("g5"));
  TEST_ASSERT_EQUAL_INT(31, coordinate2pos("h5"));

  // ROW 6
  TEST_ASSERT_EQUAL_INT(16, coordinate2pos("a6"));
  TEST_ASSERT_EQUAL_INT(17, coordinate2pos("b6"));
  TEST_ASSERT_EQUAL_INT(18, coordinate2pos("c6"));
  TEST_ASSERT_EQUAL_INT(19, coordinate2pos("d6"));
  TEST_ASSERT_EQUAL_INT(20, coordinate2pos("e6"));
  TEST_ASSERT_EQUAL_INT(21, coordinate2pos("f6"));
  TEST_ASSERT_EQUAL_INT(22, coordinate2pos("g6"));
  TEST_ASSERT_EQUAL_INT(23, coordinate2pos("h6"));

  // ROW 7
  TEST_ASSERT_EQUAL_INT(8, coordinate2pos("a7"));
  TEST_ASSERT_EQUAL_INT(9, coordinate2pos("b7"));
  TEST_ASSERT_EQUAL_INT(10, coordinate2pos("c7"));
  TEST_ASSERT_EQUAL_INT(11, coordinate2pos("d7"));
  TEST_ASSERT_EQUAL_INT(12, coordinate2pos("e7"));
  TEST_ASSERT_EQUAL_INT(13, coordinate2pos("f7"));
  TEST_ASSERT_EQUAL_INT(14, coordinate2pos("g7"));
  TEST_ASSERT_EQUAL_INT(15, coordinate2pos("h7"));

  // ROW 8
  TEST_ASSERT_EQUAL_INT(0, coordinate2pos("a8"));
  TEST_ASSERT_EQUAL_INT(1, coordinate2pos("b8"));
  TEST_ASSERT_EQUAL_INT(2, coordinate2pos("c8"));
  TEST_ASSERT_EQUAL_INT(3, coordinate2pos("d8"));
  TEST_ASSERT_EQUAL_INT(4, coordinate2pos("e8"));
  TEST_ASSERT_EQUAL_INT(5, coordinate2pos("f8"));
  TEST_ASSERT_EQUAL_INT(6, coordinate2pos("g8"));
  TEST_ASSERT_EQUAL_INT(7, coordinate2pos("h8"));
}

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
  RUN_TEST_CASE(boardTests, testCoordinate2Pos);
  RUN_TEST_CASE(boardTests, testMoveEncode);
  RUN_TEST_CASE(boardTests, testWhiteMove2Str);
  RUN_TEST_CASE(boardTests, testMakeMove);
  RUN_TEST_CASE(boardTests, testClearBoard);
  RUN_TEST_CASE(boardTests, testStrMakeMove);
  RUN_TEST_CASE(boardTests, testInitBitBoards);
}
