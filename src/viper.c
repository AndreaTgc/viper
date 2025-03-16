#include <stdio.h>
#include "bitboard.h"
#include "engine_types.h"
#include "position.h"
#include "movegen.h"

int main(void){
    Position pos = { 0 };
    MoveList list = {
        .moves = { 0 },
        .size = 0,
    };
    positionSetInitial(&pos);
    positionPrint(&pos);
    genPseudoLegalMoves(&pos, &list);
    printf("pseudo legal moves = %d\n", list.size);
}
