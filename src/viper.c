#include <stdio.h>
#include "bitboard.h"
#include "engine_types.h"
#include "position.h"
#include "movegen.h"

int main(void){
    Position pos = { 0 };
    MoveList list = { 0 };
    position_set_initial(&pos);
    generate_pseudo_legal_moves(&pos, &list);
    for(int i = 0; i < list.size; i++){
        char buf[10];
        move_to_string(list.moves[i], buf);
        printf("%s\n", buf);
    }
}
