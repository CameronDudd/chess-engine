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
}

TEST_TEAR_DOWN(testPerft) {
}

TEST(testPerft, perft0) {
  initBoard(&board);
  fenPopulateBoard(FEN_STARTING_POSITION, &board);
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
  initBoard(&board);
  fenPopulateBoard(FEN_STARTING_POSITION, &board);
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
  initBoard(&board);
  fenPopulateBoard(FEN_STARTING_POSITION, &board);
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
  initBoard(&board);
  fenPopulateBoard(FEN_STARTING_POSITION, &board);
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
  initBoard(&board);
  fenPopulateBoard(FEN_STARTING_POSITION, &board);
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
  initBoard(&board);
  fenPopulateBoard(FEN_STARTING_POSITION, &board);
  PerftResult result = perft(&board, 5, false);
  TEST_ASSERT_EQUAL_UINT64(4865609, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(82719, result.captures);
  TEST_ASSERT_EQUAL_UINT64(258, result.ep);
  TEST_ASSERT_EQUAL_UINT64(0, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(27351, result.checks);
  TEST_ASSERT_EQUAL_UINT64(347, result.checkmates);
}

TEST(testPerft, pos2perft1) {
  initBoard(&board);
  fenPopulateBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", &board);
  PerftResult result = perft(&board, 1, false);
  TEST_ASSERT_EQUAL_UINT64(48, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(8, result.captures);
  TEST_ASSERT_EQUAL_UINT64(0, result.ep);
  TEST_ASSERT_EQUAL_UINT64(2, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(0, result.checks);
  TEST_ASSERT_EQUAL_UINT64(0, result.checkmates);
}

TEST(testPerft, pos2perft2) {
  initBoard(&board);
  fenPopulateBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", &board);
  PerftResult result = perft(&board, 2, false);
  TEST_ASSERT_EQUAL_UINT64(2039, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(351, result.captures);
  TEST_ASSERT_EQUAL_UINT64(1, result.ep);
  TEST_ASSERT_EQUAL_UINT64(91, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(3, result.checks);
  TEST_ASSERT_EQUAL_UINT64(0, result.checkmates);
}

TEST(testPerft, pos2perft3) {
  initBoard(&board);
  fenPopulateBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", &board);
  PerftResult result = perft(&board, 3, false);
  TEST_ASSERT_EQUAL_UINT64(97862, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(17102, result.captures);
  TEST_ASSERT_EQUAL_UINT64(45, result.ep);
  TEST_ASSERT_EQUAL_UINT64(3162, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(993, result.checks);
  TEST_ASSERT_EQUAL_UINT64(1, result.checkmates);
}

TEST(testPerft, pos2perft4) {
  initBoard(&board);
  fenPopulateBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", &board);
  PerftResult result = perft(&board, 4, false);
  TEST_ASSERT_EQUAL_UINT64(4085603, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(757163, result.captures);
  TEST_ASSERT_EQUAL_UINT64(1929, result.ep);
  TEST_ASSERT_EQUAL_UINT64(128013, result.castles);
  TEST_ASSERT_EQUAL_UINT64(15172, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(25523, result.checks);
  TEST_ASSERT_EQUAL_UINT64(43, result.checkmates);
}

TEST(testPerft, pos3perft1) {
  initBoard(&board);
  fenPopulateBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", &board);
  PerftResult result = perft(&board, 1, false);
  TEST_ASSERT_EQUAL_UINT64(14, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(1, result.captures);
  TEST_ASSERT_EQUAL_UINT64(0, result.ep);
  TEST_ASSERT_EQUAL_UINT64(0, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(2, result.checks);
  TEST_ASSERT_EQUAL_UINT64(0, result.checkmates);
}

TEST(testPerft, pos3perft2) {
  initBoard(&board);
  fenPopulateBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", &board);
  PerftResult result = perft(&board, 2, false);
  TEST_ASSERT_EQUAL_UINT64(191, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(14, result.captures);
  TEST_ASSERT_EQUAL_UINT64(0, result.ep);
  TEST_ASSERT_EQUAL_UINT64(0, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(10, result.checks);
  TEST_ASSERT_EQUAL_UINT64(0, result.checkmates);
}

TEST(testPerft, pos3perft3) {
  initBoard(&board);
  fenPopulateBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", &board);
  PerftResult result = perft(&board, 3, false);
  TEST_ASSERT_EQUAL_UINT64(2812, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(209, result.captures);
  TEST_ASSERT_EQUAL_UINT64(2, result.ep);
  TEST_ASSERT_EQUAL_UINT64(0, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(267, result.checks);
  TEST_ASSERT_EQUAL_UINT64(0, result.checkmates);
}

TEST(testPerft, pos3perft4) {
  initBoard(&board);
  fenPopulateBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", &board);
  PerftResult result = perft(&board, 4, false);
  TEST_ASSERT_EQUAL_UINT64(43238, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(3348, result.captures);
  TEST_ASSERT_EQUAL_UINT64(123, result.ep);
  TEST_ASSERT_EQUAL_UINT64(0, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(1680, result.checks);
  TEST_ASSERT_EQUAL_UINT64(17, result.checkmates);
}

TEST(testPerft, pos3perft5) {
  initBoard(&board);
  fenPopulateBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", &board);
  PerftResult result = perft(&board, 5, false);
  TEST_ASSERT_EQUAL_UINT64(674624, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(52051, result.captures);
  TEST_ASSERT_EQUAL_UINT64(1165, result.ep);
  TEST_ASSERT_EQUAL_UINT64(0, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(52950, result.checks);
  TEST_ASSERT_EQUAL_UINT64(0, result.checkmates);
}

TEST(testPerft, pos4perft1) {
  initBoard(&board);
  fenPopulateBoard("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", &board);
  PerftResult result = perft(&board, 1, false);
  TEST_ASSERT_EQUAL_UINT64(6, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(0, result.captures);
  TEST_ASSERT_EQUAL_UINT64(0, result.ep);
  TEST_ASSERT_EQUAL_UINT64(0, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(0, result.checks);
  TEST_ASSERT_EQUAL_UINT64(0, result.checkmates);
}

TEST(testPerft, pos4perft2) {
  initBoard(&board);
  fenPopulateBoard("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", &board);
  PerftResult result = perft(&board, 2, false);
  TEST_ASSERT_EQUAL_UINT64(264, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(87, result.captures);
  TEST_ASSERT_EQUAL_UINT64(0, result.ep);
  TEST_ASSERT_EQUAL_UINT64(6, result.castles);
  TEST_ASSERT_EQUAL_UINT64(48, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(10, result.checks);
  TEST_ASSERT_EQUAL_UINT64(0, result.checkmates);
}

TEST(testPerft, pos4perft3) {
  initBoard(&board);
  fenPopulateBoard("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", &board);
  PerftResult result = perft(&board, 3, false);
  TEST_ASSERT_EQUAL_UINT64(9467, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(1021, result.captures);
  TEST_ASSERT_EQUAL_UINT64(4, result.ep);
  TEST_ASSERT_EQUAL_UINT64(0, result.castles);
  TEST_ASSERT_EQUAL_UINT64(120, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(38, result.checks);
  TEST_ASSERT_EQUAL_UINT64(22, result.checkmates);
}

TEST(testPerft, pos4perft4) {
  initBoard(&board);
  fenPopulateBoard("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", &board);
  PerftResult result = perft(&board, 4, false);
  TEST_ASSERT_EQUAL_UINT64(422333, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(131393, result.captures);
  TEST_ASSERT_EQUAL_UINT64(0, result.ep);
  TEST_ASSERT_EQUAL_UINT64(7795, result.castles);
  TEST_ASSERT_EQUAL_UINT64(60032, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(15492, result.checks);
  TEST_ASSERT_EQUAL_UINT64(5, result.checkmates);
}

TEST(testPerft, pos4perft1Mirror) {
  initBoard(&board);
  fenPopulateBoard("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", &board);
  PerftResult result = perft(&board, 1, false);
  TEST_ASSERT_EQUAL_UINT64(6, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(0, result.captures);
  TEST_ASSERT_EQUAL_UINT64(0, result.ep);
  TEST_ASSERT_EQUAL_UINT64(0, result.castles);
  TEST_ASSERT_EQUAL_UINT64(0, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(0, result.checks);
  TEST_ASSERT_EQUAL_UINT64(0, result.checkmates);
}

TEST(testPerft, pos4perft2Mirror) {
  initBoard(&board);
  fenPopulateBoard("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", &board);
  PerftResult result = perft(&board, 2, false);
  TEST_ASSERT_EQUAL_UINT64(264, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(87, result.captures);
  TEST_ASSERT_EQUAL_UINT64(0, result.ep);
  TEST_ASSERT_EQUAL_UINT64(6, result.castles);
  TEST_ASSERT_EQUAL_UINT64(48, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(10, result.checks);
  TEST_ASSERT_EQUAL_UINT64(0, result.checkmates);
}

TEST(testPerft, pos4perft3Mirror) {
  initBoard(&board);
  fenPopulateBoard("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", &board);
  PerftResult result = perft(&board, 3, false);
  TEST_ASSERT_EQUAL_UINT64(9467, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(1021, result.captures);
  TEST_ASSERT_EQUAL_UINT64(4, result.ep);
  TEST_ASSERT_EQUAL_UINT64(0, result.castles);
  TEST_ASSERT_EQUAL_UINT64(120, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(38, result.checks);
  TEST_ASSERT_EQUAL_UINT64(22, result.checkmates);
}

TEST(testPerft, pos4perft4Mirror) {
  initBoard(&board);
  fenPopulateBoard("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", &board);
  PerftResult result = perft(&board, 4, false);
  TEST_ASSERT_EQUAL_UINT64(422333, result.nodes);
  TEST_ASSERT_EQUAL_UINT64(131393, result.captures);
  TEST_ASSERT_EQUAL_UINT64(0, result.ep);
  TEST_ASSERT_EQUAL_UINT64(7795, result.castles);
  TEST_ASSERT_EQUAL_UINT64(60032, result.promotions);
  TEST_ASSERT_EQUAL_UINT64(15492, result.checks);
  TEST_ASSERT_EQUAL_UINT64(5, result.checkmates);
}

TEST_GROUP_RUNNER(testPerft) {
  // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
  RUN_TEST_CASE(testPerft, perft0);
  RUN_TEST_CASE(testPerft, perft1);
  RUN_TEST_CASE(testPerft, perft2);
  RUN_TEST_CASE(testPerft, perft3);
  RUN_TEST_CASE(testPerft, perft4);
  RUN_TEST_CASE(testPerft, perft5);

  // r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -
  RUN_TEST_CASE(testPerft, pos2perft1);
  RUN_TEST_CASE(testPerft, pos2perft2);
  RUN_TEST_CASE(testPerft, pos2perft3);
  RUN_TEST_CASE(testPerft, pos2perft4);

  // 8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1
  RUN_TEST_CASE(testPerft, pos3perft1);
  RUN_TEST_CASE(testPerft, pos3perft2);
  RUN_TEST_CASE(testPerft, pos3perft3);
  RUN_TEST_CASE(testPerft, pos3perft4);
  RUN_TEST_CASE(testPerft, pos3perft5);

  // r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1
  RUN_TEST_CASE(testPerft, pos4perft1);
  RUN_TEST_CASE(testPerft, pos4perft2);
  RUN_TEST_CASE(testPerft, pos4perft3);
  RUN_TEST_CASE(testPerft, pos4perft4);

  // r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1
  RUN_TEST_CASE(testPerft, pos4perft1Mirror);
  RUN_TEST_CASE(testPerft, pos4perft2Mirror);
  RUN_TEST_CASE(testPerft, pos4perft3Mirror);
  RUN_TEST_CASE(testPerft, pos4perft4Mirror);
}
