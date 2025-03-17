#include "move.h"
#include <assert.h>
#include <string.h>

void moveToString(Move m, char *buf) {
    assert(strlen(buf) >= 6);
    int from = MOVE_GET_FROM(m);
    int to = MOVE_GET_TO(m);
    int flag = MOVE_GET_FLAG(m);
    buf[0] = 'a' + (from % 8);
    buf[1] = '1' + (from / 8);
    buf[2] = 'a' + (to % 8);
    buf[3] = '1' + (to / 8);
    int len = 4;
    if (flag) {
        // Handle promotion flag
        buf[4] = " nbrq"[flag];
        len = 5;
    }
    buf[len] = '\0';
}
