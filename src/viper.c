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
    bitboardPrint(pos.occupancies[WHITE]);
    printf("[MASK OF THE SQUARES ATTACKED BY WHITE]");
    bitboardPrint(genFullAttacksMask(&pos, pos.player_to_move));
}
