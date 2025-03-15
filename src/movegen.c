#include "movegen.h"
#include "bitboard.h"
#include "position.h"

void genPawnMoves(Position *pos, Move *move_list) {
    Bitboard pawns = pos->pieces[0 + (pos->player_to_move * 6)];
    BoardSquare square;
    int capt_bit;
    Bitboard init_rank = pos->player_to_move == WHITE ? RANK_2 : RANK_7;
    Bitboard total_occupancy =
        pos->occupancies[WHITE] | pos->occupancies[BLACK];
    while (pawns) {
        BB_POP_LSB(pawns, square); // Get squares one at a time
        // Handle pushes
        if (square & init_rank) {
            // Handle 2-square pushing
        }
        // Handle captures
        Bitboard captures = getPawnAttacks(square, pos->player_to_move);
        captures &= pos->occupancies[OPPONENT(pos->player_to_move)];
        while (captures) {
            BB_POP_LSB(captures, capt_bit);
            // Add the capture to the move list
        }
    }
}

void genKnightMoves(Position *pos, Move *move_list) {
    Bitboard knights = pos->pieces[2 + (pos->player_to_move * 6)];
    BoardSquare square;
    BoardSquare move_sq;
    while (knights) {
        BB_POP_LSB(knights, square);
        Bitboard moves =
            getKnightAttacks(square) & ~pos->occupancies[pos->player_to_move];
        while (moves) {
            BB_POP_LSB(moves, move_sq);
            // Add the moves to the move list
        }
    }
}

void genKingMoves(Position *pos, Move *move_list) {
    Bitboard king = pos->pieces[5 + (pos->player_to_move * 6)];
    BoardSquare square;
    BoardSquare move_sq;
    while (king) {
        BB_POP_LSB(king, square);
        Bitboard moves =
            getKingAttacks(square) & ~pos->occupancies[pos->player_to_move];
        while(moves){
            BB_POP_LSB(moves, move_sq);
            // Add the moves to the move list
        }
    }
}
