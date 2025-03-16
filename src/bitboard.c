#include "bitboard.h"
#include "engine_types.h"
#include "utils.h"
#include <stdio.h>

void bitboardPrint(Bitboard x) {
    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            putchar((1ULL << (i * 8 + j)) & x ? 'x' : '.');
            putchar(' ');
        }
        putchar('\n');
    }
    putchar('\n');
}

void initSlidingPiecesTables(void) {
    for (BoardSquare sq = A1; sq <= H8; sq++) {
        int bits = BOARDSQUARE_N - rook_magics[sq].shift;
        for (int i = 0; i < (1 << bits); i++) {
            Bitboard blk = generateBlocks(i, bits, rook_magics[sq].mask);
            Bitboard hash =
                ((blk & rook_magics[sq].mask) * rook_magics[sq].magic) >>
                rook_magics[sq].shift;
            rook_attacks[hash + rook_magics[sq].offset] =
                generateRookAttack(sq, blk);
        }
        bits = BOARDSQUARE_N - bishop_magics[sq].shift;
        for (int i = 0; i < (1 << bits); i++) {
            Bitboard blk = generateBlocks(i, bits, bishop_magics[sq].mask);
            Bitboard hash =
                ((blk & bishop_magics[sq].mask) * bishop_magics[sq].magic) >>
                bishop_magics[sq].shift;
            bishop_attacks[hash + bishop_magics[sq].offset] =
                generateBishopAttack(sq, blk);
        }
    }
}

static inline Bitboard generateBlocks(int idx, int n, Bitboard mask) {
    Bitboard blk = 0ULL;
    BoardSquare lsb;
    for (int i = 0; i < n; i++) {
        BB_POP_LSB(mask, lsb);
        if (idx & (1 << i))
            blk |= SQUARE_AS_BIT(lsb);
    }
    return blk;
}

static inline Bitboard generateRookAttack(BoardSquare sq, Bitboard blocks) {
    Bitboard attacks = 0;
    int rank = sq / 8, file = sq % 8;
    for (int r = rank + 1; r < 8; r++) {
        attacks |= (1ULL << (r * 8 + file));
        if (blocks & (1ULL << (r * 8 + file)))
            break;
    }
    for (int r = rank - 1; r >= 0; r--) {
        attacks |= (1ULL << (r * 8 + file));
        if (blocks & (1ULL << (r * 8 + file)))
            break;
    }
    for (int f = file + 1; f < 8; f++) {
        attacks |= (1ULL << (rank * 8 + f));
        if (blocks & (1ULL << (rank * 8 + f)))
            break;
    }
    for (int f = file - 1; f >= 0; f--) {
        attacks |= (1ULL << (rank * 8 + f));
        if (blocks & (1ULL << (rank * 8 + f)))
            break;
    }
    return attacks;
}

static inline Bitboard generateBishopAttack(BoardSquare sq, Bitboard blocks) {
    Bitboard attacks = 0;
    int rank = sq / 8, file = sq % 8;
    for (int r = rank + 1, f = file + 1; r < 8 && f < 8; r++, f++) {
        attacks |= (1ULL << (r * 8 + f));
        if (blocks & (1ULL << (r * 8 + f)))
            break;
    }
    for (int r = rank + 1, f = file - 1; r < 8 && f >= 0; r++, f--) {
        attacks |= (1ULL << (r * 8 + f));
        if (blocks & (1ULL << (r * 8 + f)))
            break;
    }
    for (int r = rank - 1, f = file + 1; r >= 0 && f < 8; r--, f++) {
        attacks |= (1ULL << (r * 8 + f));
        if (blocks & (1ULL << (r * 8 + f)))
            break;
    }
    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--) {
        attacks |= (1ULL << (r * 8 + f));
        if (blocks & (1ULL << (r * 8 + f)))
            break;
    }
    return attacks;
}

FORCE_INLINE Bitboard getRookAttacks(BoardSquare sq, Bitboard blocks) {
    Bitboard hash = blocks & rook_magics[sq].mask;
    hash = (hash * rook_magics[sq].magic) >> rook_magics[sq].shift;
    return rook_attacks[hash + rook_magics[sq].offset];
}

FORCE_INLINE Bitboard getBishopAttacks(BoardSquare sq, Bitboard blocks) {
    Bitboard hash = blocks & bishop_magics[sq].mask;
    hash = (hash * bishop_magics[sq].magic) >> bishop_magics[sq].shift;
    return bishop_attacks[hash + bishop_magics[sq].offset];
}

FORCE_INLINE Bitboard getQueenAttacks(BoardSquare sq, Bitboard blocks) {
    return getBishopAttacks(sq, blocks) | getRookAttacks(sq, blocks);
}

Bitboard rook_attacks[102400];
Bitboard bishop_attacks[5248];

const Bitboard pawn_attacks[BOARDSQUARE_N * 2] = {
    0x0000000000000200, 0x0000000000000500, 0x0000000000000a00,
    0x0000000000001400, 0x0000000000002800, 0x0000000000005000,
    0x000000000000a000, 0x0000000000004000, 0x0000000000020000,
    0x0000000000050000, 0x00000000000a0000, 0x0000000000140000,
    0x0000000000280000, 0x0000000000500000, 0x0000000000a00000,
    0x0000000000400000, 0x0000000002000000, 0x0000000005000000,
    0x000000000a000000, 0x0000000014000000, 0x0000000028000000,
    0x0000000050000000, 0x00000000a0000000, 0x0000000040000000,
    0x0000000200000000, 0x0000000500000000, 0x0000000a00000000,
    0x0000001400000000, 0x0000002800000000, 0x0000005000000000,
    0x000000a000000000, 0x0000004000000000, 0x0000020000000000,
    0x0000050000000000, 0x00000a0000000000, 0x0000140000000000,
    0x0000280000000000, 0x0000500000000000, 0x0000a00000000000,
    0x0000400000000000, 0x0002000000000000, 0x0005000000000000,
    0x000a000000000000, 0x0014000000000000, 0x0028000000000000,
    0x0050000000000000, 0x00a0000000000000, 0x0040000000000000,
    0x0200000000000000, 0x0500000000000000, 0x0a00000000000000,
    0x1400000000000000, 0x2800000000000000, 0x5000000000000000,
    0xa000000000000000, 0x4000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000002, 0x0000000000000005, 0x000000000000000a,
    0x0000000000000014, 0x0000000000000028, 0x0000000000000050,
    0x00000000000000a0, 0x0000000000000040, 0x0000000000000200,
    0x0000000000000500, 0x0000000000000a00, 0x0000000000001400,
    0x0000000000002800, 0x0000000000005000, 0x000000000000a000,
    0x0000000000004000, 0x0000000000020000, 0x0000000000050000,
    0x00000000000a0000, 0x0000000000140000, 0x0000000000280000,
    0x0000000000500000, 0x0000000000a00000, 0x0000000000400000,
    0x0000000002000000, 0x0000000005000000, 0x000000000a000000,
    0x0000000014000000, 0x0000000028000000, 0x0000000050000000,
    0x00000000a0000000, 0x0000000040000000, 0x0000000200000000,
    0x0000000500000000, 0x0000000a00000000, 0x0000001400000000,
    0x0000002800000000, 0x0000005000000000, 0x000000a000000000,
    0x0000004000000000, 0x0000020000000000, 0x0000050000000000,
    0x00000a0000000000, 0x0000140000000000, 0x0000280000000000,
    0x0000500000000000, 0x0000a00000000000, 0x0000400000000000,
    0x0002000000000000, 0x0005000000000000, 0x000a000000000000,
    0x0014000000000000, 0x0028000000000000, 0x0050000000000000,
    0x00a0000000000000, 0x0040000000000000,
};

const Bitboard knight_attacks[BOARDSQUARE_N] = {
    0x0000000000020400, 0x0000000000050800, 0x00000000000a1100,
    0x0000000000142200, 0x0000000000284400, 0x0000000000508800,
    0x0000000000a01000, 0x0000000000402000, 0x0000000002040004,
    0x0000000005080008, 0x000000000a110011, 0x0000000014220022,
    0x0000000028440044, 0x0000000050880088, 0x00000000a0100010,
    0x0000000040200020, 0x0000000204000402, 0x0000000508000805,
    0x0000000a1100110a, 0x0000001422002214, 0x0000002844004428,
    0x0000005088008850, 0x000000a0100010a0, 0x0000004020002040,
    0x0000020400040200, 0x0000050800080500, 0x00000a1100110a00,
    0x0000142200221400, 0x0000284400442800, 0x0000508800885000,
    0x0000a0100010a000, 0x0000402000204000, 0x0002040004020000,
    0x0005080008050000, 0x000a1100110a0000, 0x0014220022140000,
    0x0028440044280000, 0x0050880088500000, 0x00a0100010a00000,
    0x0040200020400000, 0x0204000402000000, 0x0508000805000000,
    0x0a1100110a000000, 0x1422002214000000, 0x2844004428000000,
    0x5088008850000000, 0xa0100010a0000000, 0x4020002040000000,
    0x0400040200000000, 0x0800080500000000, 0x1100110a00000000,
    0x2200221400000000, 0x4400442800000000, 0x8800885000000000,
    0x100010a000000000, 0x2000204000000000, 0x0004020000000000,
    0x0008050000000000, 0x00110a0000000000, 0x0022140000000000,
    0x0044280000000000, 0x0088500000000000, 0x0010a00000000000,
    0x0020400000000000,
};

const Bitboard king_attacks[BOARDSQUARE_N] = {
    0x0000000000000302, 0x0000000000000705, 0x0000000000000e0a,
    0x0000000000001c14, 0x0000000000003828, 0x0000000000007050,
    0x000000000000e0a0, 0x000000000000c040, 0x0000000000030203,
    0x0000000000070507, 0x00000000000e0a0e, 0x00000000001c141c,
    0x0000000000382838, 0x0000000000705070, 0x0000000000e0a0e0,
    0x0000000000c040c0, 0x0000000003020300, 0x0000000007050700,
    0x000000000e0a0e00, 0x000000001c141c00, 0x0000000038283800,
    0x0000000070507000, 0x00000000e0a0e000, 0x00000000c040c000,
    0x0000000302030000, 0x0000000705070000, 0x0000000e0a0e0000,
    0x0000001c141c0000, 0x0000003828380000, 0x0000007050700000,
    0x000000e0a0e00000, 0x000000c040c00000, 0x0000030203000000,
    0x0000070507000000, 0x00000e0a0e000000, 0x00001c141c000000,
    0x0000382838000000, 0x0000705070000000, 0x0000e0a0e0000000,
    0x0000c040c0000000, 0x0003020300000000, 0x0007050700000000,
    0x000e0a0e00000000, 0x001c141c00000000, 0x0038283800000000,
    0x0070507000000000, 0x00e0a0e000000000, 0x00c040c000000000,
    0x0302030000000000, 0x0705070000000000, 0x0e0a0e0000000000,
    0x1c141c0000000000, 0x3828380000000000, 0x7050700000000000,
    0xe0a0e00000000000, 0xc040c00000000000, 0x0203000000000000,
    0x0507000000000000, 0x0a0e000000000000, 0x141c000000000000,
    0x2838000000000000, 0x5070000000000000, 0xa0e0000000000000,
    0x40c0000000000000,
};

const Magic rook_magics[BOARDSQUARE_N] = {
    // clang-format off
    { .mask = 0x000101010101017E, .magic = 0x1080018022704002, .shift = 52, .offset = 0     },
    { .mask = 0x000202020202027C, .magic = 0x8B4000A005900840, .shift = 53, .offset = 4096  },
    { .mask = 0x000404040404047A, .magic = 0x09000A4100200012, .shift = 53, .offset = 6144  },
    { .mask = 0x0008080808080876, .magic = 0x0200100408402200, .shift = 53, .offset = 8192  },
    { .mask = 0x001010101010106E, .magic = 0x0600020020083064, .shift = 53, .offset = 10240 },
    { .mask = 0x002020202020205E, .magic = 0x0100020100040008, .shift = 53, .offset = 12288 },
    { .mask = 0x004040404040403E, .magic = 0x0600010450881200, .shift = 53, .offset = 14336 },
    { .mask = 0x008080808080807E, .magic = 0x8100028208412100, .shift = 52, .offset = 16384 },
    { .mask = 0x0001010101017E00, .magic = 0x2001800040008021, .shift = 53, .offset = 20480 },
    { .mask = 0x0002020202027C00, .magic = 0x0000C02010004001, .shift = 54, .offset = 22528 },
    { .mask = 0x0004040404047A00, .magic = 0x0001002000401104, .shift = 54, .offset = 23552 },
    { .mask = 0x0008080808087600, .magic = 0x1201002008100104, .shift = 54, .offset = 24576 },
    { .mask = 0x0010101010106E00, .magic = 0x0904808008000400, .shift = 54, .offset = 25600 },
    { .mask = 0x0020202020205E00, .magic = 0x4004800400020080, .shift = 54, .offset = 26624 },
    { .mask = 0x0040404040403E00, .magic = 0x040A0002000801C4, .shift = 54, .offset = 27648 },
    { .mask = 0x0080808080807E00, .magic = 0x1D020001084408A2, .shift = 53, .offset = 28672 },
    { .mask = 0x00010101017E0100, .magic = 0x41400080088A4620, .shift = 53, .offset = 30720 },
    { .mask = 0x00020202027C0200, .magic = 0x0C50004000200040, .shift = 54, .offset = 32768 },
    { .mask = 0x00040404047A0400, .magic = 0x0042020010804020, .shift = 54, .offset = 33792 },
    { .mask = 0x0008080808760800, .magic = 0x0400210008100104, .shift = 54, .offset = 34816 },
    { .mask = 0x00101010106E1000, .magic = 0x0828808004000800, .shift = 54, .offset = 35840 },
    { .mask = 0x00202020205E2000, .magic = 0x0600808002000400, .shift = 54, .offset = 36864 },
    { .mask = 0x00404040403E4000, .magic = 0x42010C0010090208, .shift = 54, .offset = 37888 },
    { .mask = 0x00808080807E8000, .magic = 0x0208020000408924, .shift = 53, .offset = 38912 },
    { .mask = 0x000101017E010100, .magic = 0x0040400080009020, .shift = 53, .offset = 40960 },
    { .mask = 0x000202027C020200, .magic = 0x1040008080200042, .shift = 54, .offset = 43008 },
    { .mask = 0x000404047A040400, .magic = 0x000A820200122240, .shift = 54, .offset = 44032 },
    { .mask = 0x0008080876080800, .magic = 0x0010040040080040, .shift = 54, .offset = 45056 },
    { .mask = 0x001010106E101000, .magic = 0x0208008080040008, .shift = 54, .offset = 46080 },
    { .mask = 0x002020205E202000, .magic = 0x5800200801041040, .shift = 54, .offset = 47104 },
    { .mask = 0x004040403E404000, .magic = 0x0806004200410804, .shift = 54, .offset = 48128 },
    { .mask = 0x008080807E808000, .magic = 0x4812004200041081, .shift = 53, .offset = 49152 },
    { .mask = 0x0001017E01010100, .magic = 0x2040204000800080, .shift = 53, .offset = 51200 },
    { .mask = 0x0002027C02020200, .magic = 0x20400C4081002501, .shift = 54, .offset = 53248 },
    { .mask = 0x0004047A04040400, .magic = 0x0010002000801881, .shift = 54, .offset = 54272 },
    { .mask = 0x0008087608080800, .magic = 0x1000801000800800, .shift = 54, .offset = 55296 },
    { .mask = 0x0010106E10101000, .magic = 0x8004800800800401, .shift = 54, .offset = 56320 },
    { .mask = 0x0020205E20202000, .magic = 0x1800800400800200, .shift = 54, .offset = 57344 },
    { .mask = 0x0040403E40404000, .magic = 0x8219000401000200, .shift = 54, .offset = 58368 },
    { .mask = 0x0080807E80808000, .magic = 0x000008410200008C, .shift = 53, .offset = 59392 },
    { .mask = 0x00017E0101010100, .magic = 0x0640802840008000, .shift = 53, .offset = 61440 },
    { .mask = 0x00027C0202020200, .magic = 0x3000201000404000, .shift = 54, .offset = 63488 },
    { .mask = 0x00047A0404040400, .magic = 0x9020220088420011, .shift = 54, .offset = 64512 },
    { .mask = 0x0008760808080800, .magic = 0x20920041110A0020, .shift = 54, .offset = 65536 },
    { .mask = 0x00106E1010101000, .magic = 0x0A04000800808006, .shift = 54, .offset = 66560 },
    { .mask = 0x00205E2020202000, .magic = 0x8400020004008080, .shift = 54, .offset = 67584 },
    { .mask = 0x00403E4040404000, .magic = 0x0089081002040081, .shift = 54, .offset = 68608 },
    { .mask = 0x00807E8080808000, .magic = 0x0401008044020001, .shift = 53, .offset = 69632 },
    { .mask = 0x007E010101010100, .magic = 0x0400800040002080, .shift = 53, .offset = 71680 },
    { .mask = 0x007C020202020200, .magic = 0x82400042211A8100, .shift = 54, .offset = 73728 },
    { .mask = 0x007A040404040400, .magic = 0x0410008010200080, .shift = 54, .offset = 74752 },
    { .mask = 0x0076080808080800, .magic = 0x3080080010008080, .shift = 54, .offset = 75776 },
    { .mask = 0x006E101010101000, .magic = 0x0080080004008080, .shift = 54, .offset = 76800 },
    { .mask = 0x005E202020202000, .magic = 0x0282020080040080, .shift = 54, .offset = 77824 },
    { .mask = 0x003E404040404000, .magic = 0x0000100108820400, .shift = 54, .offset = 78848 },
    { .mask = 0x007E808080808000, .magic = 0x0091340080410200, .shift = 53, .offset = 79872 },
    { .mask = 0x7E01010101010100, .magic = 0x0082800020430013, .shift = 52, .offset = 81920 },
    { .mask = 0x7C02020202020200, .magic = 0x0208408012002102, .shift = 53, .offset = 86016 },
    { .mask = 0x7A04040404040400, .magic = 0x0464220008401282, .shift = 53, .offset = 88064 },
    { .mask = 0x7608080808080800, .magic = 0x2100041000082101, .shift = 53, .offset = 90112 },
    { .mask = 0x6E10101010101000, .magic = 0x0402000409605002, .shift = 53, .offset = 92160 },
    { .mask = 0x5E20202020202000, .magic = 0x2005004882040001, .shift = 53, .offset = 94208 },
    { .mask = 0x3E40404040404000, .magic = 0x1040409008410204, .shift = 53, .offset = 96256 },
    { .mask = 0x7E80808080808000, .magic = 0x0800408849040022, .shift = 52, .offset = 98304 },
    // clang-format on
};

const Magic bishop_magics[64] = {
    // clang-format off
    { .mask = 0x0040201008040200, .magic = 0x4041620809010010, .shift = 58, .offset = 0    },
    { .mask = 0x0000402010080400, .magic = 0x0010500101202102, .shift = 59, .offset = 64   },
    { .mask = 0x0000004020100A00, .magic = 0x024202004108001A, .shift = 59, .offset = 96   },
    { .mask = 0x0000000040221400, .magic = 0x0108260040011000, .shift = 59, .offset = 128  },
    { .mask = 0x0000000002442800, .magic = 0x0401104000000000, .shift = 59, .offset = 160  },
    { .mask = 0x0000000204085000, .magic = 0x0822011008062000, .shift = 59, .offset = 192  },
    { .mask = 0x0000020408102000, .magic = 0x10311808020A0002, .shift = 59, .offset = 224  },
    { .mask = 0x0002040810204000, .magic = 0x0030420201014100, .shift = 58, .offset = 256  },
    { .mask = 0x0020100804020000, .magic = 0x0852041082280110, .shift = 59, .offset = 320  },
    { .mask = 0x0040201008040000, .magic = 0x0000A00214011020, .shift = 59, .offset = 352  },
    { .mask = 0x00004020100A0000, .magic = 0x084012241400A450, .shift = 59, .offset = 384  },
    { .mask = 0x0000004022140000, .magic = 0x1300822082000805, .shift = 59, .offset = 416  },
    { .mask = 0x0000000244280000, .magic = 0x01241C10284008C0, .shift = 59, .offset = 448  },
    { .mask = 0x0000020408500000, .magic = 0x00100A2804C02224, .shift = 59, .offset = 480  },
    { .mask = 0x0002040810200000, .magic = 0x00040202411C40A0, .shift = 59, .offset = 512  },
    { .mask = 0x0004081020400000, .magic = 0x0006620202010410, .shift = 59, .offset = 544  },
    { .mask = 0x0010080402000200, .magic = 0x201080A020429090, .shift = 59, .offset = 576  },
    { .mask = 0x0020100804000400, .magic = 0x0015010208280300, .shift = 59, .offset = 608  },
    { .mask = 0x004020100A000A00, .magic = 0x0422040102020202, .shift = 57, .offset = 640  },
    { .mask = 0x0000402214001400, .magic = 0x0208020082830080, .shift = 57, .offset = 768  },
    { .mask = 0x0000024428002800, .magic = 0x0804200202010014, .shift = 57, .offset = 896  },
    { .mask = 0x0002040850005000, .magic = 0x1110200110101000, .shift = 57, .offset = 1024 },
    { .mask = 0x0004081020002000, .magic = 0x0011000401015000, .shift = 59, .offset = 1152 },
    { .mask = 0x0008102040004000, .magic = 0x2001000024010410, .shift = 59, .offset = 1184 },
    { .mask = 0x0008040200020400, .magic = 0x00084000880208C8, .shift = 59, .offset = 1216 },
    { .mask = 0x0010080400040800, .magic = 0x2402280802080820, .shift = 59, .offset = 1248 },
    { .mask = 0x0020100A000A1000, .magic = 0x02080C0006002602, .shift = 57, .offset = 1280 },
    { .mask = 0x0040221400142200, .magic = 0x0002008008008102, .shift = 55, .offset = 1408 },
    { .mask = 0x0002442800284400, .magic = 0x0803010000444000, .shift = 55, .offset = 1920 },
    { .mask = 0x0004085000500800, .magic = 0x0201020008405004, .shift = 57, .offset = 2432 },
    { .mask = 0x0008102000201000, .magic = 0x2844204084821088, .shift = 59, .offset = 2560 },
    { .mask = 0x0010204000402000, .magic = 0x00C0484021010800, .shift = 59, .offset = 2592 },
    { .mask = 0x0004020002040800, .magic = 0x020C10480014A024, .shift = 59, .offset = 2624 },
    { .mask = 0x0008040004081000, .magic = 0x0024040400031040, .shift = 59, .offset = 2656 },
    { .mask = 0x00100A000A102000, .magic = 0x2009040210010804, .shift = 57, .offset = 2688 },
    { .mask = 0x0022140014224000, .magic = 0x8000200800090106, .shift = 55, .offset = 2816 },
    { .mask = 0x0044280028440200, .magic = 0x1811010401420020, .shift = 55, .offset = 3328 },
    { .mask = 0x0008500050080400, .magic = 0x1E05080121020200, .shift = 57, .offset = 3840 },
    { .mask = 0x0010200020100800, .magic = 0x0028820880040894, .shift = 59, .offset = 3968 },
    { .mask = 0x0020400040201000, .magic = 0x010C108200023500, .shift = 59, .offset = 4000 },
    { .mask = 0x0002000204081000, .magic = 0x0050900808C22011, .shift = 59, .offset = 4032 },
    { .mask = 0x0004000408102000, .magic = 0x6520881110110901, .shift = 59, .offset = 4064 },
    { .mask = 0x000A000A10204000, .magic = 0x0007010801080200, .shift = 57, .offset = 4096 },
    { .mask = 0x0014001422400000, .magic = 0x20B0012018043100, .shift = 57, .offset = 4224 },
    { .mask = 0x0028002844020000, .magic = 0x1446082100404405, .shift = 57, .offset = 4352 },
    { .mask = 0x0050005008040200, .magic = 0x4C81020081001200, .shift = 57, .offset = 4480 },
    { .mask = 0x0020002010080400, .magic = 0x1888C80800800044, .shift = 59, .offset = 4608 },
    { .mask = 0x0040004020100800, .magic = 0x0510010101080020, .shift = 59, .offset = 4640 },
    { .mask = 0x0000020408102000, .magic = 0x500400A290100000, .shift = 59, .offset = 4672 },
    { .mask = 0x0000040810204000, .magic = 0xC001006110080008, .shift = 59, .offset = 4704 },
    { .mask = 0x00000A1020400000, .magic = 0x0000020100886140, .shift = 59, .offset = 4736 },
    { .mask = 0x0000142240000000, .magic = 0x0080002042020000, .shift = 59, .offset = 4768 },
    { .mask = 0x0000284402000000, .magic = 0xA000101002088044, .shift = 59, .offset = 4800 },
    { .mask = 0x0000500804020000, .magic = 0x8311100210010302, .shift = 59, .offset = 4832 },
    { .mask = 0x0000201008040200, .magic = 0xC804084204040040, .shift = 59, .offset = 4864 },
    { .mask = 0x0000402010080400, .magic = 0x0009010102160188, .shift = 59, .offset = 4896 },
    { .mask = 0x0002040810204000, .magic = 0x400621004A304004, .shift = 58, .offset = 4928 },
    { .mask = 0x0004081020400000, .magic = 0x8000044C02011000, .shift = 59, .offset = 4992 },
    { .mask = 0x000A102040000000, .magic = 0x1004090080480800, .shift = 59, .offset = 5024 },
    { .mask = 0x0014224000000000, .magic = 0x0001808008460810, .shift = 59, .offset = 5056 },
    { .mask = 0x0028440200000000, .magic = 0xA489500090320880, .shift = 59, .offset = 5088 },
    { .mask = 0x0050080402000000, .magic = 0x8002000450820200, .shift = 59, .offset = 5120 },
    { .mask = 0x0020100804020000, .magic = 0x0200850404280200, .shift = 59, .offset = 5152 },
    { .mask = 0x0040201008040200, .magic = 0x0602040414084202, .shift = 58, .offset = 5184 },
    // clang-format on
};
