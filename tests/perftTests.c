/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

// perft first move results from stockfish
// perft results from https://www.chessprogramming.org/Perft_Results

#include <unity.h>
#include <unity_fixture.h>

#include "board.h"
#include "fen.h"
#include "perft.h"

#define FEN_A2A3_POSITION "rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1"
#define FEN_B2B3_POSITION "rnbqkbnr/pppppppp/8/8/8/1P6/P1PPPPPP/RNBQKBNR b KQkq - 0 1"
#define FEN_C2C3_POSITION "rnbqkbnr/pppppppp/8/8/8/2P5/PP1PPPPP/RNBQKBNR b KQkq - 0 1"
#define FEN_D2D3_POSITION "rnbqkbnr/pppppppp/8/8/8/3P4/PPP1PPPP/RNBQKBNR b KQkq - 0 1"
#define FEN_E2E3_POSITION "rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq - 0 1"
#define FEN_F2F3_POSITION "rnbqkbnr/pppppppp/8/8/8/5P2/PPPPP1PP/RNBQKBNR b KQkq - 0 1"
#define FEN_H2H3_POSITION "rnbqkbnr/pppppppp/8/8/8/7P/PPPPPPP1/RNBQKBNR b KQkq - 0 1"
#define FEN_A2A4_POSITION "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1"
#define FEN_B2B4_POSITION "rnbqkbnr/pppppppp/8/8/1P6/8/P1PPPPPP/RNBQKBNR b KQkq b3 0 1"
#define FEN_C2C4_POSITION "rnbqkbnr/pppppppp/8/8/2P5/8/PP1PPPPP/RNBQKBNR b KQkq c3 0 1"
#define FEN_D2D4_POSITION "rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d3 0 1"
#define FEN_E2E4_POSITION "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN_F2F4_POSITION "rnbqkbnr/pppppppp/8/8/5P2/8/PPPPP1PP/RNBQKBNR b KQkq f3 0 1"
#define FEN_G2G4_POSITION "rnbqkbnr/pppppppp/8/8/6P1/8/PPPPPP1P/RNBQKBNR b KQkq g3 0 1"
#define FEN_H2H4_POSITION "rnbqkbnr/pppppppp/8/8/7P/8/PPPPPPP1/RNBQKBNR b KQkq h3 0 1"
#define FEN_B1A3_POSITION "rnbqkbnr/pppppppp/8/8/8/N7/PPPPPPPP/R1BQKBNR b KQkq - 0 1"
#define FEN_B1C3_POSITION "rnbqkbnr/pppppppp/8/8/8/2N5/PPPPPPPP/R1BQKBNR b KQkq - 0 1"
#define FEN_G1F3_POSITION "rnbqkbnr/pppppppp/8/8/8/5N2/PPPPPPPP/RNBQKB1R b KQkq - 0 1"
#define FEN_G1H3_POSITION "rnbqkbnr/pppppppp/8/8/8/7N/PPPPPPPP/RNBQKB1R b KQkq - 0 1"

Board board;
Move previousMove;

TEST_GROUP(perftTests);

TEST_SETUP(perftTests) {
  previousMove = 0;
  fenPopulateBoard(FEN_STARTING_POSITION, board);
}

TEST_TEAR_DOWN(perftTests) {}

TEST(perftTests, perft0) {
  // A2A3
  fenPopulateBoard(FEN_A2A3_POSITION, board);
  struct PerftRes perft0a2a3 = perft(board, fenActiveColor(FEN_A2A3_POSITION), 0, MOVE_ENCODE(coordinate2pos(""), coordinate2pos(""), 0, 0));
  TEST_ASSERT_EQUAL_INT(1, perft0a2a3.nodes);

  // Basic test
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
  // FIXME (cameron): this causes a caught error
  // A2A3
  fenPopulateBoard(FEN_A2A3_POSITION, board);
  struct PerftRes perfta2a3 = perft(board, fenActiveColor(FEN_A2A3_POSITION), 3, MOVE_ENCODE(coordinate2pos("a2"), coordinate2pos("a3"), 0, 0));
  TEST_ASSERT_EQUAL_INT(1, perfta2a3.nodes);

  // Basic test
  struct PerftRes perft3 = perft(board, 1, 3, 0);
  TEST_ASSERT_EQUAL_INT(3, perft3.depth);
  TEST_ASSERT_EQUAL_UINT64(8902, perft3.nodes);
  TEST_ASSERT_EQUAL_UINT32(34, perft3.captures);
  TEST_ASSERT_EQUAL_UINT32(0, perft3.castles);
  TEST_ASSERT_EQUAL_UINT32(12, perft3.checks);
  TEST_ASSERT_EQUAL_UINT32(0, perft3.checkMates);
}

TEST(perftTests, perft4) {
  struct PerftRes perft4 = perft(board, 1, 4, 0);
  TEST_ASSERT_EQUAL_INT(4, perft4.depth);
  TEST_ASSERT_EQUAL_UINT64(197281, perft4.nodes);
  TEST_ASSERT_EQUAL_UINT32(1576, perft4.captures);
  TEST_ASSERT_EQUAL_UINT32(0, perft4.castles);
  TEST_ASSERT_EQUAL_UINT32(469, perft4.checks);
  TEST_ASSERT_EQUAL_UINT32(8, perft4.checkMates);
}

TEST(perftTests, perft5) {
  struct PerftRes perft5 = perft(board, 1, 5, 0);
  TEST_ASSERT_EQUAL_INT(5, perft5.depth);
  TEST_ASSERT_EQUAL_UINT64(4865609, perft5.nodes);
  TEST_ASSERT_EQUAL_UINT32(82719, perft5.captures);
  TEST_ASSERT_EQUAL_UINT32(0, perft5.castles);
  TEST_ASSERT_EQUAL_UINT32(27351, perft5.checks);
  TEST_ASSERT_EQUAL_UINT32(347, perft5.checkMates);
}

TEST_GROUP_RUNNER(perftTests) {
  RUN_TEST_CASE(perftTests, perft0);
  RUN_TEST_CASE(perftTests, perft1);
  RUN_TEST_CASE(perftTests, perft2);
  RUN_TEST_CASE(perftTests, perft3);
  RUN_TEST_CASE(perftTests, perft4);
  RUN_TEST_CASE(perftTests, perft5);
}
