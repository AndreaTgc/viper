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
    for(int i = 0; i < list.size; i++){
        char buf[6];
        moveToString(list.moves[i], buf);
        printf("Move at index %d = %s\n", i, buf);
    }
}
