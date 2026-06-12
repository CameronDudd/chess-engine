/*
 *   Copyright (c) 2026 Cameron Dudd
 *   All rights reserved.
 */

#include "magics.h"

#include <stdbool.h>
#include <stdio.h>

#include "defs.h"

const BitBoard RookMagics[NUM_POSITIONS] = {
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

const BitBoard BishopMagics[NUM_POSITIONS] = {
    0x84100408441040,   0x820994A00830002,  0x104042420400080,  0x8111040484200025, 0x4009104000001C40, 0x842080445610040,  0x1828840568C00020,
    0x400120882201000,  0xA0408100C088400,  0x80000C0104050208, 0x400800810A0412,   0x402080A00218030,  0x208C040420500000, 0xB1086010C202400,
    0x9820090805108988, 0x20212021201,      0x84003060120410,   0x108080308080090,  0x6C010018201C0,    0x4402021040104000, 0x9001000490400202,
    0x8102040508010402, 0x44000200840C01,   0x2009102610420,    0xA88200040640180,  0x42008640820A0,    0x241028004400,     0x2002080004024098,
    0x1010020504008,    0x188224006004201,  0x344004401011000,  0x2004012210805,    0xC100842042004,    0x621031000085040,  0x20108401071000C2,
    0x10C28081080200,   0x1801A4001CC100,   0x20010041121800,   0x8012044040210800, 0x204052208002180,  0x92212028102004,   0x16080218101280,
    0x1800C0406010C00,  0x100004207805800,  0x884288100408C04,  0x4004008802001110, 0x45102C01024340,   0x50013103000120,   0x20030105104088C0,
    0x1821040A200000,   0x400116084A081040, 0x1940441108000,    0x1300000420820431, 0x10801030100324B0, 0x6829220802140310, 0x20220441006004,
    0x8110140401080829, 0x100408208010424,  0x2000C00042009080, 0x40080380841400,   0x401080012020A00,  0x204011010068820,  0x100102012088200,
    0x10400400A2004101,
};

BitBoard rookMasks[NUM_POSITIONS];
BitBoard bishopMasks[NUM_POSITIONS];

int rookNumBits[NUM_POSITIONS];
int bishopNumBits[NUM_POSITIONS];

BitBoard kingAttacks[NUM_POSITIONS];
BitBoard rookAttacks[NUM_POSITIONS][ROOK_VARIATIONS];
BitBoard bishopAttacks[NUM_POSITIONS][BISHOP_VARIATIONS];
BitBoard knightAttacks[NUM_POSITIONS];
BitBoard pawnAttacks[2][NUM_POSITIONS];
BitBoard pawnSingleMoves[2][NUM_POSITIONS];
BitBoard pawnDoubleMoves[2][NUM_POSITIONS];

// https://en.wikipedia.org/wiki/Xorshift
BitBoard xorshift64() {
  static BitBoard seed = (BitBoard)8392615925280821472;
  seed ^= seed << 13;
  seed ^= seed >> 7;
  seed ^= seed << 17;
  return seed;
}

BitBoard sparsexorshift64(void) {
  return xorshift64() & xorshift64() & xorshift64();
}

BitBoard occupancyMask(int index, BitBoard mask) {
  BitBoard occupancy = (BitBoard)0;
  int bits           = 0;
  for (PositionIndex pos = 0; pos < NUM_POSITIONS; ++pos) {
    if (mask & BIT_SQUARE(pos)) {
      if (index & (1 << bits)) {
        occupancy |= BIT_SQUARE(pos);
      }
      ++bits;
    }
  }
  return occupancy;
}

BitBoard rookMask(int rank, int file) {
  BitBoard mask = (BitBoard)0;
  for (int r = rank - 1; r > 0; --r) mask |= BIT_SQUARE((r * 8) + file);
  for (int c = file + 1; c < 7; ++c) mask |= BIT_SQUARE((rank * 8) + c);
  for (int r = rank + 1; r < 7; ++r) mask |= BIT_SQUARE((r * 8) + file);
  for (int c = file - 1; c > 0; --c) mask |= BIT_SQUARE((rank * 8) + c);
  return mask;
}

BitBoard rookLegalMoves(int rank, int file, BitBoard occupancyMask) {
  BitBoard mask = (BitBoard)0;

  for (int r = rank - 1; r >= 0; --r) {
    int pos = (r * 8) + file;
    mask |= BIT_SQUARE(pos);
    if (BIT_SQUARE(pos) & occupancyMask) break;
  }

  for (int c = file + 1; c < 8; ++c) {
    int pos = (rank * 8) + c;
    mask |= BIT_SQUARE(pos);
    if (BIT_SQUARE(pos) & occupancyMask) break;
  }

  for (int r = rank + 1; r < 8; ++r) {
    int pos = (r * 8) + file;
    mask |= BIT_SQUARE(pos);
    if (BIT_SQUARE(pos) & occupancyMask) break;
  }

  for (int c = file - 1; c >= 0; --c) {
    int pos = ((rank * 8) + c);
    mask |= BIT_SQUARE(pos);
    if (BIT_SQUARE(pos) & occupancyMask) break;
  }

  return mask;
}

void rookMagicNumbers(void) {
  printf("const BitBoard RookMagics[NUM_POSITIONS] = {");
  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      BitBoard mask     = rookMask(rank, file);
      int bitsCount     = __builtin_popcountll(mask);
      int numVariations = 1 << bitsCount;

      BitBoard occupancies[4096];
      BitBoard legalMoves[4096];

      for (int v = 0; v < numVariations; ++v) {
        occupancies[v] = occupancyMask(v, mask);
        legalMoves[v]  = rookLegalMoves(rank, file, occupancies[v]);
      }

      BitBoard tmp[4096];
      bool fail;

      while (1) {
        BitBoard candidate = sparsexorshift64();
        for (int t = 0; t < numVariations; ++t) tmp[t] = (BitBoard)0;
        fail = false;
        for (int v = 0; v < numVariations; ++v) {
          int magicIdx = (occupancies[v] * candidate) >> (NUM_POSITIONS - bitsCount);
          if (tmp[magicIdx] == (BitBoard)0) {
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

BitBoard bishopMask(int rank, int file) {
  BitBoard mask = (BitBoard)0;
  for (int c = file + 1, r = rank + 1; c < 7 && r < 7; ++c, ++r) mask |= BIT_SQUARE((r * 8) + c);
  for (int c = file + 1, r = rank - 1; c < 7 && r > 0; ++c, --r) mask |= BIT_SQUARE((r * 8) + c);
  for (int c = file - 1, r = rank - 1; c > 0 && r > 0; --c, --r) mask |= BIT_SQUARE((r * 8) + c);
  for (int c = file - 1, r = rank + 1; c > 0 && r < 7; --c, ++r) mask |= BIT_SQUARE((r * 8) + c);
  return mask;
}

BitBoard bishopLegalMoves(int rank, int file, BitBoard occupancyMask) {
  BitBoard mask = (BitBoard)0;

  for (int c = file + 1, r = rank + 1; c < 8 && r < 8; ++c, ++r) {
    int pos = ((r * 8) + c);
    mask |= BIT_SQUARE(pos);
    if (BIT_SQUARE(pos) & occupancyMask) break;
  }

  for (int c = file + 1, r = rank - 1; c < 8 && r >= 0; ++c, --r) {
    int pos = ((r * 8) + c);
    mask |= BIT_SQUARE(pos);
    if (BIT_SQUARE(pos) & occupancyMask) break;
  }

  for (int c = file - 1, r = rank - 1; c >= 0 && r >= 0; --c, --r) {
    int pos = ((r * 8) + c);
    mask |= BIT_SQUARE(pos);
    if (BIT_SQUARE(pos) & occupancyMask) break;
  }

  for (int c = file - 1, r = rank + 1; c >= 0 && r < 8; --c, ++r) {
    int pos = ((r * 8) + c);
    mask |= BIT_SQUARE(pos);
    if (BIT_SQUARE(pos) & occupancyMask) break;
  }

  return mask;
}

void bishopMagicNumbers(void) {
  printf("const BitBoard BishopMagics[NUM_POSITIONS] = {");
  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      BitBoard mask     = bishopMask(rank, file);
      int bitsCount     = __builtin_popcountll(mask);
      int numVariations = 1 << bitsCount;

      BitBoard occupancies[512];
      BitBoard legalMoves[512];

      for (int v = 0; v < numVariations; ++v) {
        occupancies[v] = occupancyMask(v, mask);
        legalMoves[v]  = bishopLegalMoves(rank, file, occupancies[v]);
      }

      BitBoard tmp[512];
      bool fail;

      while (1) {
        BitBoard candidate = sparsexorshift64();
        for (int t = 0; t < numVariations; ++t) tmp[t] = (BitBoard)0;
        fail = false;
        for (int v = 0; v < numVariations; ++v) {
          int magicIdx = (occupancies[v] * candidate) >> (NUM_POSITIONS - bitsCount);
          if (tmp[magicIdx] == (BitBoard)0) {
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

void initMagicAttacks(void) {
  int bits;
  int variations;
  BitBoard attackMask;

  for (PositionIndex pos = 0; pos < NUM_POSITIONS; ++pos) {
    int rank = INDEX_RANK(pos);
    int file = INDEX_FILE(pos);

    // KINGS
    BitBoard k = (BitBoard)0;
    for (int dr = -1; dr <= 1; ++dr) {
      for (int df = -1; df <= 1; ++df) {
        if (dr == 0 && df == 0) continue;
        int r = rank + dr;
        int f = file + df;
        if (r < 0 || r > 7 || f < 0 || f > 7) continue;
        k |= BIT_SQUARE(POS_INDEX(r, f));
      }
    }
    kingAttacks[pos] = k;

    // ROOKS
    attackMask       = rookMask(rank, file);
    rookMasks[pos]   = attackMask;
    bits             = __builtin_popcountll(attackMask);
    rookNumBits[pos] = bits;
    variations       = 1 << bits;
    for (int v = 0; v < variations; ++v) {
      BitBoard occupancy      = occupancyMask(v, attackMask);
      BitBoard moves          = rookLegalMoves(rank, file, occupancy);
      int magic               = (occupancy * RookMagics[pos]) >> (NUM_POSITIONS - bits);
      rookAttacks[pos][magic] = moves;
    }

    // BISHOPS
    attackMask         = bishopMask(rank, file);
    bishopMasks[pos]   = attackMask;
    bits               = __builtin_popcountll(attackMask);
    bishopNumBits[pos] = bits;
    variations         = 1 << bits;
    for (int v = 0; v < variations; ++v) {
      BitBoard occupancy        = occupancyMask(v, attackMask);
      BitBoard moves            = bishopLegalMoves(rank, file, occupancy);
      int magic                 = (occupancy * BishopMagics[pos]) >> (NUM_POSITIONS - bits);
      bishopAttacks[pos][magic] = moves;
    }

    // KNIGHT
    BitBoard n      = (BitBoard)0;
    const int dr[8] = {2, 2, 1, 1, -1, -1, -2, -2};
    const int df[8] = {-1, 1, -2, 2, -2, 2, -1, 1};
    for (int i = 0; i < 8; ++i) {
      int r = rank + dr[i];
      int f = file + df[i];
      if ((0 <= r) && (r < 8) && (0 <= f) && (f < 8)) {
        n |= BIT_SQUARE(POS_INDEX(r, f));
      }
    }
    knightAttacks[pos] = n;

    // WHITE PAWN
    if (rank == 1) pawnDoubleMoves[WHITE][pos] = BIT_SQUARE(POS_INDEX(rank + 2, file));
    if (rank < 7) pawnSingleMoves[WHITE][pos] = BIT_SQUARE(POS_INDEX(rank + 1, file));

    BitBoard wp = (BitBoard)0;
    if (rank < 7) {
      if (file > 0) wp |= BIT_SQUARE(POS_INDEX(rank + 1, file - 1));
      if (file < 7) wp |= BIT_SQUARE(POS_INDEX(rank + 1, file + 1));
    }
    pawnAttacks[WHITE][pos] = wp;

    // BLACK PAWN
    if (rank == 6) pawnDoubleMoves[BLACK][pos] = BIT_SQUARE(POS_INDEX(rank - 2, file));
    if (rank > 0) pawnSingleMoves[BLACK][pos] = BIT_SQUARE(POS_INDEX(rank - 1, file));

    BitBoard bp = (BitBoard)0;
    if (rank > 0) {
      if (file > 0) bp |= BIT_SQUARE(POS_INDEX(rank - 1, file - 1));
      if (file < 7) bp |= BIT_SQUARE(POS_INDEX(rank - 1, file + 1));
    }
    pawnAttacks[BLACK][pos] = bp;
  }
}
