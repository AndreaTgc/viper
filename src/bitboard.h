#ifndef guard_bitboard_h
#define guard_bitboard_h

#include "engine_types.h"
#include <stdint.h>

typedef uint64_t Bitboard;

#define RANK_1 0x00000000000000FFULL
#define RANK_2 0x000000000000FF00ULL
#define RANK_3 0x0000000000FF0000ULL
#define RANK_4 0x00000000FF000000ULL
#define RANK_5 0x000000FF00000000ULL
#define RANK_6 0x0000FF0000000000ULL
#define RANK_7 0x00FF000000000000ULL
#define RANK_8 0xFF00000000000000ULL
#define FILE_A 0x0101010101010101ULL
#define FILE_B 0x0202020202020202ULL
#define FILE_C 0x0404040404040404ULL
#define FILE_D 0x0808080808080808ULL
#define FILE_E 0x1010101010101010ULL
#define FILE_F 0x2020202020202020ULL
#define FILE_G 0x4040404040404040ULL
#define FILE_H 0x8080808080808080ULL

#define BB_LSB(x)      (__builtin_ctzll(x))
#define BB_MSB(x)      (__builtin_clzll(x))
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

void bitboard_print(Bitboard x);

extern const Bitboard pawn_attacks[BOARDSQUARE_N * 2];
extern const Bitboard knight_attacks[BOARDSQUARE_N];
extern const Bitboard king_attacks[BOARDSQUARE_N];

typedef struct SMagic {
    Bitboard mask;
    uint64_t magic;
    int shift;
    int offset;
} Magic;

void init_magic_bitboards(void);

extern const Magic rook_magics[BOARDSQUARE_N];
extern const Magic bishop_magics[BOARDSQUARE_N];
extern Bitboard rook_attacks[102400];
extern Bitboard bishop_attacks[5248];

static inline Bitboard _generate_blocks(int idx, int n, Bitboard mask);
static inline Bitboard _generate_rook_attacks(BoardSquare sq, Bitboard blocks);
static inline Bitboard _generate_bishop_attacks(BoardSquare sq, Bitboard blocks);

Bitboard get_rook_attacks(BoardSquare sq, Bitboard blocks);
Bitboard get_bishop_attacks(BoardSquare sq, Bitboard blocks);
Bitboard get_queen_attacks(BoardSquare sq, Bitboard blocks);

#endif // guard_bitboard_h
