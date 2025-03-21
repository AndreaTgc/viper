#include "position.h"
#include "engine_types.h"
#include "move.h"
#include <ctype.h>
#include <stdio.h>

#define CTOI(c) (int)(c - '0')

void position_print(Position *pos) {
    puts("[POSITION PRETTY PRINTING]");
    printf("[TOTAL MOVES] %d\n", pos->ply);
    printf("[HALF CLOCK MOVES] %d\n", pos->half_move_clock);
    printf("[PLAYER TO MOVE] %c\n", pos->player_to_move == WHITE ? 'W' : 'C');
    const char *pieces_chars = "PRNBQKprnbqk";
    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            Bitboard bit = SQUARE_AS_BIT(i * 8 + j);
            int found = -1;
            for (int k = 0; k <= COLOR_N * PIECETYPE_N; k++) {
                if (bit & pos->pieces[k]) {
                    printf("%c ", pieces_chars[k]);
                    found = k;
                    break;
                }
            }
            if (found == -1)
                printf(". ");
        }
        putchar('\n');
    }
}

void position_set_initial(Position *pos) {
    pos->ply = 0;
    pos->half_move_clock = 0;
    pos->player_to_move = WHITE;
    pos->en_passant = NO_SQUARE;
    pos->pieces[0] = RANK_2;
    pos->pieces[1] = 0ULL | SQUARE_AS_BIT(A1) | SQUARE_AS_BIT(H1);
    pos->pieces[2] = 0ULL | SQUARE_AS_BIT(B1) | SQUARE_AS_BIT(G1);
    pos->pieces[3] = 0ULL | SQUARE_AS_BIT(C1) | SQUARE_AS_BIT(F1);
    pos->pieces[4] = 0ULL | SQUARE_AS_BIT(D1);
    pos->pieces[5] = 0ULL | SQUARE_AS_BIT(E1);
    pos->pieces[6] = RANK_7;
    pos->pieces[7] = 0ULL | SQUARE_AS_BIT(A8) | SQUARE_AS_BIT(H8);
    pos->pieces[8] = 0ULL | SQUARE_AS_BIT(B8) | SQUARE_AS_BIT(G8);
    pos->pieces[9] = 0ULL | SQUARE_AS_BIT(C8) | SQUARE_AS_BIT(F8);
    pos->pieces[10] = 0ULL | SQUARE_AS_BIT(D8);
    pos->pieces[11] = 0ULL | SQUARE_AS_BIT(E8);
    pos->occupancies[WHITE] = 0ULL;
    pos->occupancies[BLACK] = 0ULL;
    POS_RECOMPUTE_OCCUPANCIES(pos);
}

void position_from_fen(Position *pos, char *fen) {
    assert(pos != NULL && fen != NULL);
    char *c = fen;
    int file = 0, rank = 7;
    // Loops that set the initial state of the bitboards
    for (; *c; c++) {
        if (*c == ' ') {
            break;
        } else if (*c == '/') {
            rank--;
            file = 0;
        } else if (isdigit(*c)) {
            file += CTOI(*c);
        } else {
            const char *pc = "PRNBQKprnbqk";
            for (int i = 0; i < 12; i++) {
                if (*c == pc[i]) {
                    pos->pieces[i] |= SQUARE_AS_BIT(rank * 8 + file);
                    file++;
                    break;
                }
            }
        }
    }
    POS_RECOMPUTE_OCCUPANCIES(pos);
}
