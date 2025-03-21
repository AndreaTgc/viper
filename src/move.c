#include "move.h"
#include <assert.h>
#include <string.h>

#define I_TO_CHAR(i) ('1' + i)
#define I_TO_FILE(i) ('A' + i)

void move_to_string(Move m, char *buf) {
  assert(strlen(buf) >= 6);
  int l = 0;
  int fsq = MOVE_GET_FROM(m);
  int tsq = MOVE_GET_TO(m);
  int type = MOVE_GET_TYPE(m);
  buf[l++] = I_TO_FILE(fsq % 8);
  buf[l++] = I_TO_CHAR(fsq / 8);
  if (type & MOVE_CAPTURE){
    buf[l++] = 'X';
  }
  buf[l++] = I_TO_FILE(tsq % 8);
  buf[l++] = I_TO_CHAR(tsq / 8);
  if (type & 0x10) {
    // Move is a promotion
    buf[l++] = 'P';
  }
  buf[l++] = '\0';
}
