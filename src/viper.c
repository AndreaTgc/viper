#include <stdio.h>
#include "bitboard.h"
#include "engine_types.h"

int main(void){
    for(int i = A1; i < BOARDSQUARE_N; i++){
        bitboardPrint(bishop_rays[i]);
    }
}
