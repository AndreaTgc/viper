#ifndef guard_movegen_h
#define guard_movegen_h

#include "bitboard.h"
#include "position.h"
#include "engine_types.h"
#include "util.h"
#include "move.h"
#include "utils.h"

void genPseudoLegalMoves(Position* pos, Move* move_list);

FORCE_INLINE void genPawnMoves(Position* pos, Move* move_list);
FORCE_INLINE void genKnightMoves(Position* pos, Move* move_list);
FORCE_INLINE void genBishopMoves(Position* pos, Move* move_list);
FORCE_INLINE void genRookMoves(Position* pos, Move* move_list);
FORCE_INLINE void genQueenMoves(Position* pos, Move* move_list);
FORCE_INLINE void genKingMoves(Position* pos, Move* move_list);

#endif // guard_movegen_h
