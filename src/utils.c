/*
 *   Copyright (c) 2024 Cameron Dudd
 *   All rights reserved.
 */

#include "utils.h"

const char *uint8t2str(uint8_t num) {
  static char str[9] = {'\0'};
  char *s = str;
  for (uint8_t i = 0; i < 8; ++i) {
    *s++ = (num & (1 << (7 - i))) ? '1' : '0';
  }
  *s++ = '\0';
  return str;
}
