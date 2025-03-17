#ifndef guard_position_h
#define guard_position_h

#include "engine_types.h"
#include "bitboard.h"
#include "move.h"

typedef struct SPosition {
    Bitboard pieces[PIECETYPE_N * COLOR_N];
    Bitboard occupancies[COLOR_N];
    Color player_to_move;
    BoardSquare en_passant;
    uint16_t ply;
    uint8_t half_move_clock;
} Position;

/// Pretty prints the current position on stdout
void positionPrint(Position* pos);

/// Sets up the initial position (can act as reset)
void positionSetInitial(Position* pos);

/// Loads a position encoded in a fen string
void positionFromFen(Position* pos, char* fen);

/// Applies a move to the current state of the position
/// TODO: add a way to track moves for undos
void positionApplyMove(Position* pos, Move move);

#endif // guard_position_h
