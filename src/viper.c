#include <stdio.h>
#include "bitboard.h"
#include "engine_types.h"

int main(void){
    initSlidingPiecesTables();
    bitboardPrint(getRookAttacks(C3, 0xFFFFFF000000FF00));
    return 0;
}
