#ifndef guard_move_h
#define guard_move_h

#include <assert.h>
#include <stdint.h>

/// MOVE ENCODING
/// Viper implements a 16-bit move encoding in order to improve
/// cache efficiency, this is also done by some of the top chess engines.
///
/// The encoding looks like this:
/// Bits 0-5: Starting square for the move
/// Bits 6-11: Ending square for the move
/// Bits 12-15: Additional info encoding (move type, promotion, etc)
typedef uint16_t Move;

typedef struct SMoveList {
    Move moves[256];
    int size;
} MoveList;

#define MOVE_QUIET            0x00
#define MOVE_DOUBLE_PAWN_PUSH 0x01
#define MOVE_KING_CASTLE      0x04
#define MOVE_QUEEN_CASTLE     0x05
#define MOVE_CAPTURE          0x08
#define MOVE_EP_CAPTURE       0x09
#define MOVE_KNIGHT_PROMO     0x10
#define MOVE_BISHOP_PROMO     0x11
#define MOVE_ROOK_PROMO       0x14
#define MOVE_QUEEN_PROMO      0x15
#define MOVE_KNIGHT_PROMO_CAP 0x18
#define MOVE_BISHOP_PROMO_CAP 0x19
#define MOVE_ROOK_PROMO_CAP   0x1C
#define MOVE_QUEEN_PROMO_CAP  0x1D

#define MOVE_ENCODE(from, to, type)                                            \
    (Move)(((from)&0x3F) | (((to)&0x3F) << 6) | (((type)&0x03) << 12))

#define MOVE_GET_FROM(m) ((m)&0x3F)
#define MOVE_GET_TO(m)   (((m) >> 6) & 0x3F)
#define MOVE_GET_TYPE(m) (((m) >> 12) & 0x0F)

/// Clears the 'from' bits and sets the new value
#define MOVE_SET_FROM(m, from)                                                 \
    do {                                                                       \
        assert(SQUARE_IS_VALID(from));                                         \
        ((m) = ((m) & ~0x3F) | ((from)&0x3F));                                 \
    } while (0)

/// Clears the 'to' bits and sets the new value
#define MOVE_SET_TO(m, to)                                                     \
    do {                                                                       \
        assert(SQUARE_IS_VALID(to));                                           \
        ((m) = ((m) & ~(0x3F << 6)) | (((to)&0x3F) << 6));                     \
    } while (0)

/// Clears the 'type' bits and sets the new value
#define MOVE_SET_TYPE(m, type)                                                 \
    ((m) = ((m) & ~(0x0F << 14)) | (((type)&0x0F) << 14))

/// Generate a move from the respective fen notation
Move move_from_string(const char *str);

/// Returns the fen notation of the move
void move_to_string(Move m, char *buf);

#endif // guard_move_h
