#ifndef guard_movegen_h
#define guard_movegen_h

#include "bitboard.h"
#include "engine_types.h"
#include "move.h"
#include "position.h"
#include "util.h"
#include "utils.h"

/// Function that takes a given position and generates all the
/// pseudo-legal moves possible for the player who has to make a move
/// and adds them in the MoveList passed as parameter
void genPseudoLegalMoves(Position *pos, MoveList *moves);

/// Takes as input a position and a list of pseudo-legal moves and filters
/// out the ones that are not legal (e.g. moves that lead to the player's own
/// king being in check)
void filterNonLegalMoves(Position *pos, MoveList *list);

#endif // guard_movegen_h
