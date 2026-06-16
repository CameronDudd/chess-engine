/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include <argparse.h>
#include <log.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "board.h"
#include "fen.h"
#include "perft.h"

#ifndef TARGET
#define TARGET "chess-engine"
#endif

typedef struct {
  const char* module;
  const char* description;
  int (*func)(const int, const char**);
} ModuleStruct;

int perftModule(int argc, const char** argv) {
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
    return 1;
  }

  Board board;
  initBoard(&board);
  fenPopulateBoard(FEN_STARTING_POSITION, &board);
  PerftResult result = perft(&board, depth, bulk);
  printf("depth %i | nodes %llu | captures %llu | e.p. %llu | castles %llu | promotions %llu | checks %llu | checkmates %llu \r\n", depth, result.nodes,
         result.captures, result.ep, result.castles, result.promotions, result.checks, result.checkmates);
  return 0;
}

static ModuleStruct modules[] = {
    {"perft", "run perft test", perftModule},
};

static void showUsage(void) {
  printf(TARGET " <module> [options]\n\nModules:\n");
  for (size_t i = 0; i < sizeof(modules) / sizeof(modules[0]); ++i) printf("  %s - %s\n", modules[i].module, modules[i].description);
}

int main(int argc, const char* argv[]) {
#ifdef DEBUG
  log_set_level(LOG_TRACE);
#else
  log_set_level(LOG_INFO);
#endif

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
  if (module) return module->func(argc - 1, argv + 1);

  showUsage();
  return 1;
}
