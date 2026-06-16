/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include <unity_fixture.h>

#include "log.h"

static void allTests(void) {
  RUN_TEST_GROUP(testPerft);
}

int main(int argc, const char* argv[]) {
  log_set_quiet(true);
  return UnityMain(argc, argv, allTests);
}
