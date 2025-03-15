#include "position.h"
#include "engine_types.h"
#include <stdio.h>

void positionPrint(Position *pos) {
    puts("[POSITION PRETTY PRINTING]");
    printf("[TOTAL MOVES] %d\n", pos->ply);
    printf("[HALF CLOCK MOVES] %d\n", pos->half_move_clock);
    printf("[PLAYER TO MOVE] %c\n", pos->player_to_move == WHITE ? 'W' : 'C');
    const char *pieces_chars = "prnbqkPRNBQK";
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
