#ifndef guard_position_h
#define guard_position_h

#include "bitboard.h"
#include "engine_types.h"
#include "move.h"

typedef struct SPosition {
    Bitboard pieces[PIECETYPE_N * COLOR_N];
    Bitboard occupancies[COLOR_N];
    Color player_to_move;
    BoardSquare en_passant;
    uint16_t ply;
    uint8_t half_move_clock;
} Position;

/// Simple macro that recomputes the occupancies
#define POS_RECOMPUTE_OCCUPANCIES(pos)                                         \
    do {                                                                       \
        pos->occupancies[WHITE] = pos->pieces[0] | pos->pieces[1] |            \
                                  pos->pieces[2] | pos->pieces[3] |            \
                                  pos->pieces[4] | pos->pieces[5];             \
        pos->occupancies[BLACK] = pos->pieces[6] | pos->pieces[7] |            \
                                  pos->pieces[8] | pos->pieces[9] |            \
                                  pos->pieces[10] | pos->pieces[11];           \
    } while (0)

/// Pretty prints the current position on stdout
void position_print(Position *pos);

/// Sets up the initial position (can act as reset)
void position_set_initial(Position *pos);

/// Loads a position encoded in a fen string
void position_from_fen(Position *pos, char *fen);

/// Applies a move to the current state of the position
/// TODO: add a way to track moves for undos
void position_apply_move(Position *pos, Move move);

/// Undo a move
void position_undo_move(Position* pos, Move move);

#endif // guard_position_h
