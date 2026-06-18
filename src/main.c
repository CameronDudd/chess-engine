/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include <argparse.h>
#include <log.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "board.h"
#include "defs.h"
#include "engine.h"
#include "fen.h"
#include "perft.h"

#ifndef TARGET
#define TARGET "chess-engine"
#endif

#define LOG_FILENAME_SIZE 128
#define ENGINE_SEARCH_DEPTH 3

static FILE* logFp = NULL;

typedef struct {
  const char* module;
  const char* description;
  void (*func)(const int, const char**);
} ModuleStruct;

static void perftModule(int argc, const char** argv) {
  int depth = -1;
  bool bulk = false;

  struct argparse_option options[] = {
      OPT_HELP(),
      OPT_INTEGER('d', "depth", &depth, "depth", NULL, 0, 0),
      OPT_BOOLEAN('b', "bulk", &bulk, "bulk", NULL, 0, 0),
      OPT_END(),
  };

  const char* const perftModuleUsages[] = {
      TARGET " perft",
      NULL,
  };

  struct argparse argparse;
  argparse_init(&argparse, options, perftModuleUsages, ARGPARSE_STOP_AT_NON_OPTION);
  argparse_describe(&argparse, "\ngenerate perft results at different depths", "");
  argc = argparse_parse(&argparse, argc, argv);
  if (argc != 0 || depth < 0) {
    argparse_usage(&argparse);
    return;
  }

  Board board;
  initBoard(&board);
  fenPopulateBoard(FEN_STARTING_POSITION, &board);
  PerftResult result = perft(&board, depth, bulk);
  printf("depth %i | nodes %llu | captures %llu | e.p. %llu | castles %llu | promotions %llu | checks %llu | checkmates %llu \r\n", depth, result.nodes,
         result.captures, result.ep, result.castles, result.promotions, result.checks, result.checkmates);
}

static void playModule(int argc, const char** argv) {
  int white = 0;
  int black = 0;

  struct argparse_option options[] = {
      OPT_HELP(),
      OPT_BOOLEAN('w', "white", &white, "play as white", NULL, 0, 0),
      OPT_BOOLEAN('b', "black", &black, "play as black", NULL, 0, 0),
      OPT_END(),
  };

  const char* const playModuleUsages[] = {
      TARGET " play",
      NULL,
  };

  struct argparse argparse;
  argparse_init(&argparse, options, playModuleUsages, ARGPARSE_STOP_AT_NON_OPTION);
  argparse_describe(&argparse, "\nplay against the engine", "");
  argc = argparse_parse(&argparse, argc, argv);
  if (argc != 0 || (white == 0 && black == 0)) {
    argparse_usage(&argparse);
    return;
  }

  Color playerColor = white ? WHITE : BLACK;

  Board board;
  initBoard(&board);
  fenPopulateBoard(FEN_STARTING_POSITION, &board);

  Move move = 0;
  UndoMove undo;
  int userQuit = 0;
  while (userQuit == 0) {
    displayBoard(&board);
    if (board.turn == playerColor) {  // NOLINT(bugprone-branch-clone)
      move = engineBestMove(&board);
    } else {
      move = engineBestMove(&board);
    }
    boardMakeMove(&board, move, &undo);
    userQuit = (getchar() == 'q') ? true : false;
  }
}

static ModuleStruct modules[] = {
    {"perft", "run perft test", perftModule},
    {"play", "play against the engine", playModule},
};

static void showUsage(void) {
  printf(TARGET " <module> [options]\n\nModules:\n");
  for (size_t i = 0; i < sizeof(modules) / sizeof(modules[0]); ++i) printf("  %s - %s\n", modules[i].module, modules[i].description);
}

static void initLogging(void) {
#ifdef DEBUG
  log_set_level(LOG_TRACE);
#else
  log_set_level(LOG_INFO);
#endif

  time_t now           = time(NULL);
  struct tm* localTime = localtime(&now);  // NOLINT(concurrency-mt-unsafe)
  if (localTime == NULL) {
    log_error("failed to obtain localtime");
    return;
  }

  char logFilename[LOG_FILENAME_SIZE];
  if (!strftime(logFilename, sizeof(logFilename), "%d%m%Y_%H%M%S_" TARGET ".log", localTime)) {
    log_error("failed to format time string");
    return;
  }
  logFp = fopen(logFilename, "w");
  if (logFp == NULL) {
    log_error("failed to open log file %s", logFilename);
    return;
  }

#ifdef DEBUG
  log_add_fp(logFp, LOG_TRACE);
#else
  log_add_fp(logFp, LOG_INFO);
#endif
}

int main(int argc, const char* argv[]) {
  initLogging();

  if (argc < 2 || !strcmp("-h", argv[1])) {
    showUsage();
    return 1;
  }

  ModuleStruct* module = NULL;
  for (unsigned long i = 0; i < sizeof(modules) / sizeof(modules[0]); ++i) {
    if (!strcmp(modules[i].module, argv[1])) {
      module = &modules[i];
    }
  }
  if (module) {
    module->func(argc - 1, argv + 1);
  } else {
    showUsage();
  }

  if (logFp && fclose(logFp)) {
    log_error("Failed to close log file");
  }

  return 1;
}
