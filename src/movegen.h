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

/// Generates a bitboard that contains all the squares that can be attacked
/// By a player
/// NOTE: This mask is generated without taking into account the fact that some
/// moves may not be valid since an ally piece is blocking that square
/// (Expect for sliding pieces, they have to take it into account anyway)
Bitboard genFullAttacksMask(Position* pos, Color player);

#endif // guard_movegen_h
