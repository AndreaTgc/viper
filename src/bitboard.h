#ifndef guard_bitboard_h
#define guard_bitboard_h

#include "engine_types.h"
#include <stdint.h>

typedef uint64_t Bitboard;

#define BB_LSB(x) (__builtin_ctzll(x))
#define BB_MSB(x) (__builtin_clzll(x))
#define BB_BITCOUNT(x) (__builtin_popcountll(x))

#define BB_POP_LSB(x, b)                                                       \
    do {                                                                       \
        b = BB_LSB(x);                                                         \
        x &= ~(1ULL << b);                                                     \
    } while (0)

#define BB_POP_MSB(x, b)                                                       \
    do {                                                                       \
        b = BB_MSB(x);                                                         \
        x &= ~(1ULL << b);                                                     \
    } while (0)

void bitboardPrint(Bitboard x);

extern const Bitboard pawn_attacks[BOARDSQUARE_N * 2];
extern const Bitboard knight_attacks[BOARDSQUARE_N];
extern const Bitboard king_attacks[BOARDSQUARE_N];

typedef struct SMagic {
    Bitboard mask;
    uint64_t magic;
    int shift;
    int offset;
} Magic;

void initSlidingPiecesTables(void);

extern const Magic rook_magics[BOARDSQUARE_N];
extern const Magic bishop_magics[BOARDSQUARE_N];
extern Bitboard rook_attacks[102400];
extern Bitboard bishop_attacks[5248];

Bitboard generateBlocks(int idx, int n, Bitboard mask);
Bitboard generateRookAttack(BoardSquare sq, Bitboard blocks);
Bitboard generateBishopAttack(BoardSquare sq, Bitboard blocks);

Bitboard getPawnAttacks(BoardSquare sq, Color c);
Bitboard getKnightAttacks(BoardSquare sq);
Bitboard getKingAttacs(BoardSquare sq);
Bitboard getRookAttacks(BoardSquare sq, Bitboard blocks);
Bitboard getBishopAttacks(BoardSquare sq, Bitboard blocks);
Bitboard getQueenAttacks(BoardSquare sq, Bitboard blocks);

#endif // guard_bitboard_h
