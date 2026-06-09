/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include "magics.h"

#include <stdbool.h>
#include <stdio.h>

const uint64_t RooksMagics[64] = {
    0x48000908A204000,  0x40400010002000,   0xB00100900402000,  0x8880045800100080, 0xA0002005160080C,  0x3280020080090400, 0x300010000C43200,
    0x200128102002044,  0x8028800040018222, 0x112C01002200041,  0x4105004100102006, 0x3801001004201900, 0x111000800243100,  0x1001001400584700,
    0x8004004408102241, 0x2800080015500,    0x18800A4000502000, 0x20424004601002,   0x8201818050002000, 0x4838010008800,    0xA1011000801000C,
    0x1101000E289400,   0x8104004E081003,   0x1140220023864104, 0x20862080004001,   0x128700810020400A, 0x100C500880200080, 0x10004040080403,
    0x5104001480180080, 0x40080800200,      0x28400502801,      0x140008200040041,  0x100804000800020,  0x810002001400441,  0x9005885000802000,
    0x1000205001003900, 0x580080800400,     0x4002008002802400, 0x12002802004411,   0x82008406000041,   0x840AA280C0108000, 0x201000404000,
    0x210002804022000,  0x1401090150010020, 0x42848000C008080,  0x108A404C10080120, 0x2002100218040001, 0x21401C0084620001, 0x4000400028801180,
    0x800620081004600,  0x20182A000100480,  0x100103A20900,     0x3A0010A8208600,   0x9002002824100200, 0x2840110830420400, 0x2050248400450A00,
    0x400410200A48412,  0x400110820804005,  0xA20010408102041,  0x804100014182101,  0x2000D08201002,    0x1000400280211,    0x4880100206880124,
    0x4660041020810842,
};

const uint64_t BishopsMagics[64] = {
    0x4004220801002804, 0x202111400800040,  0x8008480004000,    0x4002018000000010, 0x220044113101C,    0x2008900400380200, 0x4004010A42008008,
    0x4462004202012080, 0x1080081010AA0006, 0x1100080104440000, 0x2220210C60001,    0x4A001080C0101414, 0x300520802100D05,  0x8220241018000400,
    0x2190A4200000,     0x41008898480082,   0x1000008850700000, 0x8204481089011,    0x510008D101048000, 0x1100013200204082, 0x800000208008000,
    0x4000010422880020, 0x8018113085100001, 0x1000000042120000, 0x10800040C1240,    0x40031070001,      0x4100010888000,    0x800011002000800,
    0x1000010020200005, 0x4AA04020004,      0x1C020840C2B10002, 0xA415060910218030, 0x4000104800040144, 0x2013080200030,    0x4002841C00008007,
    0x21020801808000,   0x200002204900800,  0x1092008020030005, 0x4210008C000,      0x20004008000A802,  0x8282208020000,    0xC44000988A020001,
    0x5A8003E120A00000, 0xE20000149000001,  0x81004040022,      0x440084010200,     0x4100010162400000, 0x20400428416,      0x4030048420180210,
    0x4009404020A03,    0x400080400881101,  0x8000000000480002, 0x8290028404040200, 0x688260C000,       0x201101002008120,  0x12C0028410408028,
    0x4002090041000,    0xC00400084092064,  0x1580814002820200, 0x20000200000B800,  0x104001000825A80,  0x22040000093A8800, 0x10000818030412,
    0x180020093010A08,
};

// https://en.wikipedia.org/wiki/Xorshift
uint64_t xorshift64() {
  static uint64_t seed = 8392615925280821472ULL;
  seed ^= seed << 13;
  seed ^= seed >> 7;
  seed ^= seed << 17;
  return seed;
}

uint64_t sparsexorshift64(void) {
  return xorshift64() & xorshift64() & xorshift64();
}

uint64_t occupancyMask(int index, uint64_t mask) {
  uint64_t occupancy = 0ULL;
  int bits           = 0;
  for (int pos = 0; pos < 64; ++pos) {
    if (mask & (1ULL << pos)) {
      if (index & (1 << bits)) {
        occupancy |= (1ULL << pos);
      }
      ++bits;
    }
  }
  return occupancy;
}

uint64_t rookMask(int rank, int file) {
  uint64_t mask = 0ULL;
  for (int r = rank - 1; r > 0; --r) mask |= (1ULL << ((r * 8) + file));
  for (int c = file + 1; c < 7; ++c) mask |= (1ULL << ((rank * 8) + c));
  for (int r = rank + 1; r < 7; ++r) mask |= (1ULL << ((r * 8) + file));
  for (int c = file - 1; c > 0; --c) mask |= (1ULL << ((rank * 8) + c));
  return mask;
}

uint64_t rookLegalMoves(int rank, int file, uint64_t occupancyMask) {
  uint64_t mask = 0ULL;

  for (int r = rank - 1; r >= 0; --r) {
    int pos = (r * 8) + file;
    mask |= (1ULL << pos);
    if ((1ULL << pos) & occupancyMask) break;
  }

  for (int c = file + 1; c < 8; ++c) {
    int pos = (rank * 8) + c;
    mask |= (1ULL << pos);
    if ((1ULL << pos) & occupancyMask) break;
  }

  for (int r = rank + 1; r < 8; ++r) {
    int pos = (r * 8) + file;
    mask |= (1ULL << pos);
    if ((1ULL << pos) & occupancyMask) break;
  }

  for (int c = file - 1; c >= 0; --c) {
    int pos = ((rank * 8) + c);
    mask |= (1ULL << pos);
    if ((1ULL << pos) & occupancyMask) break;
  }

  return mask;
}

void rookMagicNumbers(void) {
  printf("const uint64_t RooksMagics[64] = {");
  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      uint64_t mask     = rookMask(rank, file);
      int bitsCount     = __builtin_popcountll(mask);
      int numVariations = 1 << bitsCount;

      uint64_t occupancies[4096];
      uint64_t legalMoves[4096];

      for (int v = 0; v < numVariations; ++v) {
        occupancies[v] = occupancyMask(v, mask);
        legalMoves[v]  = rookLegalMoves(rank, file, occupancies[v]);
      }

      uint64_t tmp[4096];
      bool fail;

      while (1) {
        uint64_t candidate = sparsexorshift64();
        for (int t = 0; t < numVariations; ++t) tmp[t] = 0ULL;
        fail = false;
        for (int v = 0; v < numVariations; ++v) {
          int magicIdx = (occupancies[v] * candidate) >> (64 - bitsCount);
          if (tmp[magicIdx] == 0ULL) {
            tmp[magicIdx] = legalMoves[v];
          } else if (tmp[magicIdx] != legalMoves[v]) {
            fail = true;
            break;
          }
        }
        if (!fail) {
          printf("0x%llX, ", (unsigned long long)candidate);
          break;
        }
      }
    }
  }
  printf("};\r\n");
}

uint64_t bishopMask(int rank, int file) {
  uint64_t mask = 0ULL;
  for (int c = file + 1, r = rank + 1; c < 7 && r < 7; ++c, ++r) mask |= (1ULL << ((r * 8) + c));
  for (int c = file + 1, r = rank - 1; c < 7 && r > 0; ++c, --r) mask |= (1ULL << ((r * 8) + c));
  for (int c = file - 1, r = rank - 1; c > 0 && r > 0; --c, --r) mask |= (1ULL << ((r * 8) + c));
  for (int c = file - 1, r = rank + 1; c > 0 && r < 7; --c, ++r) mask |= (1ULL << ((r * 8) + c));
  return mask;
}

uint64_t bishopLegalMoves(int rank, int file, uint64_t occupancyMask) {
  uint64_t mask = 0ULL;

  for (int c = file + 1, r = rank + 1; c < 7 && r < 7; ++c, ++r) {
    int pos = ((r * 8) + c);
    mask |= (1ULL << pos);
    if ((1ULL << pos) & occupancyMask) break;
  }

  for (int c = file + 1, r = rank - 1; c < 7 && r > 0; ++c, --r) {
    int pos = ((r * 8) + c);
    mask |= (1ULL << pos);
    if ((1ULL << pos) & occupancyMask) break;
  }

  for (int c = file - 1, r = rank - 1; c > 0 && r > 0; --c, --r) {
    int pos = ((r * 8) + c);
    mask |= (1ULL << pos);
    if ((1ULL << pos) & occupancyMask) break;
  }

  for (int c = file - 1, r = rank + 1; c > 0 && r < 7; --c, ++r) {
    int pos = ((r * 8) + c);
    mask |= (1ULL << pos);
    if ((1ULL << pos) & occupancyMask) break;
  }

  return mask;
}

void bishopMagicNumbers(void) {
  printf("const uint64_t BishopsMagics[64] = {");
  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      uint64_t mask     = bishopMask(rank, file);
      int bitsCount     = __builtin_popcountll(mask);
      int numVariations = 1 << bitsCount;

      uint64_t occupancies[4096];
      uint64_t legalMoves[4096];

      for (int v = 0; v < numVariations; ++v) {
        occupancies[v] = occupancyMask(v, mask);
        legalMoves[v]  = bishopLegalMoves(rank, file, occupancies[v]);
      }

      uint64_t tmp[4096];
      bool fail;

      while (1) {
        uint64_t candidate = sparsexorshift64();
        for (int t = 0; t < numVariations; ++t) tmp[t] = 0ULL;
        fail = false;
        for (int v = 0; v < numVariations; ++v) {
          int magicIdx = (occupancies[v] * candidate) >> (64 - bitsCount);
          if (tmp[magicIdx] == 0ULL) {
            tmp[magicIdx] = legalMoves[v];
          } else if (tmp[magicIdx] != legalMoves[v]) {
            fail = true;
            break;
          }
        }
        if (!fail) {
          printf("0x%llX, ", (unsigned long long)candidate);
          break;
        }
      }
    }
  }
  printf("};\r\n");
}
