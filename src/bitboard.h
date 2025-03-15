#ifndef guard_bitboard_h
#define guard_bitboard_h

#include "engine_types.h"
#include <stdint.h>

typedef uint64_t Bitboard;

#define BB_LSB(x) (__builtin_ctzll(x));
#define BB_MSB(x) (__builtin_clzll(x));
#define BB_BITCOUNT(x) (__builtin_popcountll(x));

void bitboardPrint(Bitboard x);

extern const Bitboard pawn_attacks[BOARDSQUARE_N * 2];
extern const Bitboard knight_attacks[BOARDSQUARE_N];
extern const Bitboard king_attacks[BOARDSQUARE_N];
extern const Bitboard rook_rays[BOARDSQUARE_N];
extern const Bitboard bishop_rays[BOARDSQUARE_N];

typedef struct SMagic {
  Bitboard mask;
  uint64_t magic;
  int shift;
  int offset;
} Magic;

extern const Magic rook_magics[BOARDSQUARE_N];
extern const Magic bishop_magics[BOARDSQUARE_N];

#endif // guard_bitboard_h
