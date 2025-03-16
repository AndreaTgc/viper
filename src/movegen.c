#include "movegen.h"
#include "bitboard.h"
#include "engine_types.h"

void genPseudoLegalMoves(Position *pos, MoveList *moves) {
    int base = 6 * pos->player_to_move; // 0 if white, 6 if black;
    int starting_square, ending_square; // Temporary variables
    // Constants used to have cleaner code below
    const Bitboard total_occupancies =
        pos->occupancies[WHITE] | pos->occupancies[BLACK];
    const Bitboard allies = pos->occupancies[pos->player_to_move];
    const Bitboard enemies = pos->occupancies[OPPONENT(pos->player_to_move)];
    // Constants for pawn moves generation
    const Bitboard promotion_rank =
        pos->player_to_move == WHITE ? RANK_8 : RANK_1;
    const Bitboard starting_rank =
        pos->player_to_move == WHITE ? RANK_2 : RANK_7;
    const int single_push_offset = pos->player_to_move == WHITE ? 8 : -8;
    const int double_push_offset = pos->player_to_move == WHITE ? 16 : -16;
    Bitboard pawns = pos->pieces[base + 0];
    while (pawns) {
        BB_POP_LSB(pawns, starting_square);
        // Handle possible captures
        Bitboard attacks =
            pawn_attacks[starting_square + (64 * pos->player_to_move)] &
            enemies;
        while (attacks) {
            // Add the possible attacks to the move list
            BB_POP_LSB(attacks, ending_square);
            if (SQUARE_AS_BIT(ending_square) & promotion_rank) {
                // Add all the possible promotions to the move list
                moves->moves[moves->size++] = MOVE_ENCODE(
                    starting_square, ending_square, PROMO_KNIGHT, CAPTURE);
                moves->moves[moves->size++] = MOVE_ENCODE(
                    starting_square, ending_square, PROMO_BISHOP, CAPTURE);
                moves->moves[moves->size++] = MOVE_ENCODE(
                    starting_square, ending_square, PROMO_QUEEN, CAPTURE);
                moves->moves[moves->size++] = MOVE_ENCODE(
                    starting_square, ending_square, PROMO_ROOK, CAPTURE);
            } else
                moves->moves[moves->size++] =
                    MOVE_ENCODE(starting_square, ending_square, 0, CAPTURE);
        }
        // Handle pawn pushing
        if (SQUARE_AS_BIT(starting_square) & starting_rank) {
            // We can do a double push
            ending_square = starting_square + double_push_offset;
            if (!SQUARE_IS_VALID(ending_square))
                break;
            Bitboard dpush =
                SQUARE_AS_BIT(ending_square) & ~(total_occupancies);
            if (dpush & promotion_rank) {
                // Add all the possible promotions to the move list
                moves->moves[moves->size++] = MOVE_ENCODE(
                    starting_square, ending_square, PROMO_KNIGHT, CAPTURE);
                moves->moves[moves->size++] = MOVE_ENCODE(
                    starting_square, ending_square, PROMO_BISHOP, CAPTURE);
                moves->moves[moves->size++] = MOVE_ENCODE(
                    starting_square, ending_square, PROMO_QUEEN, CAPTURE);
                moves->moves[moves->size++] = MOVE_ENCODE(
                    starting_square, ending_square, PROMO_ROOK, CAPTURE);
            } else if (dpush) {
                moves->moves[moves->size++] =
                    MOVE_ENCODE(starting_square, ending_square, 0, CAPTURE);
            }
        }
        ending_square = starting_square + single_push_offset;
        if (SQUARE_IS_VALID(ending_square)) {
            Bitboard spush =
                SQUARE_AS_BIT(ending_square) & ~(total_occupancies);
            if (spush & promotion_rank) {
                // Add all the possible promotions to the move list
                moves->moves[moves->size++] = MOVE_ENCODE(
                    starting_square, ending_square, PROMO_KNIGHT, CAPTURE);
                moves->moves[moves->size++] = MOVE_ENCODE(
                    starting_square, ending_square, PROMO_BISHOP, CAPTURE);
                moves->moves[moves->size++] = MOVE_ENCODE(
                    starting_square, ending_square, PROMO_QUEEN, CAPTURE);
                moves->moves[moves->size++] = MOVE_ENCODE(
                    starting_square, ending_square, PROMO_ROOK, CAPTURE);
            } else if (spush) {
                moves->moves[moves->size++] =
                    MOVE_ENCODE(starting_square, ending_square, 0, CAPTURE);
            }
        }
    }
    Bitboard rooks = pos->pieces[base + 1];
    while (rooks) {
        BB_POP_LSB(rooks, starting_square);
        Bitboard attacks =
            getRookAttacks(starting_square, total_occupancies) & ~allies;
        while (attacks) {
            BB_POP_LSB(attacks, ending_square);
            if (SQUARE_AS_BIT(ending_square) & enemies)
                moves->moves[moves->size++] =
                    MOVE_ENCODE(starting_rank, ending_square, 0, CAPTURE);
            else
                moves->moves[moves->size++] =
                    MOVE_ENCODE(starting_rank, ending_square, 0, QUIET);
        }
    }
    Bitboard knights = pos->pieces[base + 2];
    while (knights) {
        BB_POP_LSB(knights, starting_square);
        Bitboard attacks = knight_attacks[starting_square] & ~allies;
        while (attacks) {
            BB_POP_LSB(attacks, ending_square);
            if (SQUARE_AS_BIT(ending_square) & enemies)
                moves->moves[moves->size++] =
                    MOVE_ENCODE(starting_square, ending_square, 0, CAPTURE);
            else
                moves->moves[moves->size++] =
                    MOVE_ENCODE(starting_square, ending_square, 0, QUIET);
        }
    }
    Bitboard bishops = pos->pieces[base + 3];
    while (bishops) {
        BB_POP_LSB(bishops, starting_square);
        Bitboard attacks =
            getBishopAttacks(starting_square, total_occupancies) & ~allies;
        while (attacks) {
            BB_POP_LSB(attacks, ending_square);
            if (SQUARE_AS_BIT(ending_square) & enemies)
                moves->moves[moves->size++] =
                    MOVE_ENCODE(starting_rank, ending_square, 0, CAPTURE);
            else
                moves->moves[moves->size++] =
                    MOVE_ENCODE(starting_rank, ending_square, 0, QUIET);
        }
    }
    Bitboard queens = pos->pieces[base + 4];
    while (queens) {
        BB_POP_LSB(queens, starting_square);
        Bitboard attacks =
            getQueenAttacks(starting_square, total_occupancies) & ~allies;
        while (attacks) {
            BB_POP_LSB(attacks, ending_square);
            if (SQUARE_AS_BIT(ending_square) & enemies)
                moves->moves[moves->size++] =
                    MOVE_ENCODE(starting_rank, ending_square, 0, CAPTURE);
            else
                moves->moves[moves->size++] =
                    MOVE_ENCODE(starting_rank, ending_square, 0, QUIET);
        }
    }
    Bitboard king = pos->pieces[base + 5];
    while (king) {
        BB_POP_LSB(king, starting_square);
        Bitboard attacks = king_attacks[starting_square] &
                           ~pos->occupancies[pos->player_to_move];
        while (attacks) {
            BB_POP_LSB(attacks, ending_square);
            if (SQUARE_AS_BIT(ending_square) &
                pos->occupancies[OPPONENT(pos->player_to_move)])
                moves->moves[moves->size++] =
                    MOVE_ENCODE(starting_square, ending_square, 0, CAPTURE);
            else
                moves->moves[moves->size++] =
                    MOVE_ENCODE(starting_square, ending_square, 0, QUIET);
        }
    }
}
