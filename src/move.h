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
/// Bits 12-13: Move promotion encoding (rook/knight/bishop/queen)
/// Bits 14-15: Move type encoding (Promotion/EnPassant/Castling)
typedef uint16_t Move;

typedef struct SMoveList {
    Move moves[256];
    int size;
} MoveList;

typedef enum EMoveType {
    QUIET = 0,
    PROMOTION,
    CAPTURE,
    EN_PASSANT,
} MoveType;

#define MOVE_ENCODE(from, to, flag, type)                                      \
    (Move)(((from)&0x3F) | (((to)&0x3F) << 6) | (((flag)&0x03) << 12) |        \
           (((type)&0x03) << 14))

#define MOVE_GET_FROM(m) ((m)&0x3F)
#define MOVE_GET_TO(m)   (((m) >> 6) & 0x3F)
#define MOVE_GET_FLAG(m) (((m) >> 12) & 0x03)
#define MOVE_GET_TYPE(m) (((m) >> 14) & 0x03)

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

/// Clears the 'flag' bits and sets the new value
#define MOVE_SET_FLAG(m, promotion)                                            \
    ((m) = ((m) & ~(0x03 << 12)) | (((promotion)&0x03) << 12))

/// Clears the 'type' bits and sets the new value
#define MOVE_SET_TYPE(m, type)                                                 \
    ((m) = ((m) & ~(0x03 << 14)) | (((type)&0x03) << 14))

/// Generate a move from the respective fen notation
Move moveFromString(const char *str);

/// Returns the fen notation of the move
void moveToString(Move m, char* buf);

#endif // guard_move_h
