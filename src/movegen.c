#include "movegen.h"
#include "bitboard.h"
#include "engine_types.h"
#include <string.h>

void genPseudoLegalMoves(Position *pos, MoveList *moves) {
    int base = 6 * pos->player_to_move; // 0 if white, 6 if black;
    int starting_square, ending_square; // Temporary variables
    // Constants used to have cleaner code below
    const Bitboard all_pcs = pos->occupancies[WHITE] | pos->occupancies[BLACK];
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
            Bitboard dpush = SQUARE_AS_BIT(ending_square) & ~(all_pcs);
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
            Bitboard spush = SQUARE_AS_BIT(ending_square) & ~(all_pcs);
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
        Bitboard attacks = getRookAttacks(starting_square, all_pcs) & ~allies;
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
        Bitboard attacks = getBishopAttacks(starting_square, all_pcs) & ~allies;
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
        Bitboard attacks = getQueenAttacks(starting_square, all_pcs) & ~allies;
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

void filterNonLegalMoves(Position *pos, MoveList *list) {
    MoveList temp_list = {
        .size = 0,
        .moves = {0},
    };
    const Bitboard current_king = pos->pieces[5 + (pos->player_to_move * 6)];
    const Color oppo = OPPONENT(pos->player_to_move);
    for (int i = 0; i < list->size; i++) {
        // TODO: Apply move
        if (current_king & ~genFullAttacksMask(pos, oppo)) {
            // The move is legal, we can add it to the list
            temp_list.moves[temp_list.size++] = list->moves[i];
        }
        // TODO: Undo move
    }
    memcpy(list->moves, temp_list.moves, sizeof(Move) * temp_list.size);
}

Bitboard genFullAttacksMask(Position *pos, Color player) {
    const int player_offset = player * 6;
    const Bitboard all_pcs = pos->occupancies[WHITE] | pos->occupancies[BLACK];
    int starting_square;
    Bitboard attack_mask = 0ULL;
    // Get pawn attacks
    Bitboard piece = pos->pieces[0 + player_offset];
    while (piece) {
        BB_POP_LSB(piece, starting_square);
        attack_mask |= pawn_attacks[starting_square + (64 * player)];
    }
    // Get rook attacks
    piece = pos->pieces[1 + player_offset];
    while (piece) {
        BB_POP_LSB(piece, starting_square);
        attack_mask |= getRookAttacks(starting_square, all_pcs);
    }
    // Get knight attacks
    piece = pos->pieces[2 + player_offset];
    while (piece) {
        BB_POP_LSB(piece, starting_square);
        attack_mask |= knight_attacks[starting_square];
    }
    // Get bishop attacks
    piece = pos->pieces[3 + player_offset];
    while (piece) {
        BB_POP_LSB(piece, starting_square);
        attack_mask |= getBishopAttacks(starting_square, all_pcs);
    }
    // Get queen attacks
    piece = pos->pieces[4 + player_offset];
    while (piece) {
        BB_POP_LSB(piece, starting_square);
        attack_mask |= getQueenAttacks(starting_square, all_pcs);
    }
    // Get king attacks
    piece = pos->pieces[5 + player_offset];
    while (piece) {
        BB_POP_LSB(piece, starting_square);
        attack_mask |= king_attacks[starting_square];
    }
    return attack_mask;
}
