#include "cchess/board.h"
#include "cchess/move.h"
#include "cchess/board_macros.h"

#include <stdio.h>

/*
    Uses verified hard-coded numbers to verify move generation is correct
*/

void pawns_moves(Board* b)
{

}

void knights_moves(Board* b)
{

}

void bishops_moves(Board* b)
{

}

void rooks_moves(Board* b)
{
    const uint64_t white_pieces = BOARD_PTR_GET_WHITE_PIECES(b);
    const uint64_t black_pieces = BOARD_PTR_GET_BLACK_PIECES(b);

    uint64_t rooks = board_get_rooks(b, SIDE_TO_PLAY_WHITE);

    const uint64_t rooks_moves_white[2] = {
        0x2021c02,
        0x13e0101000000,
    };

    uint64_t i = 0;

    while(rooks > 0)
    {
        const uint64_t square = ctz_u64(rooks);
        const uint64_t moves = move_gen_rook(square, 
                                             SIDE_TO_PLAY_WHITE, 
                                             white_pieces, 
                                             black_pieces);

        CCHESS_ASSERT(moves == rooks_moves_white[i] && "Invalid white rook moves");

        BOARD_UNSET_BIT(rooks, square);

        i++;
    }

    rooks = board_get_rooks(b, SIDE_TO_PLAY_BLACK);

    const uint64_t rooks_moves_black[2] = {
        0xe8,
        0x10ee101010100000,
    };

    i = 0;

    while(rooks > 0)
    {
        const uint64_t square = ctz_u64(rooks);
        const uint64_t moves = move_gen_rook(square, 
                                             SIDE_TO_PLAY_BLACK, 
                                             white_pieces, 
                                             black_pieces);

        CCHESS_ASSERT(moves == rooks_moves_black[i] && "Invalid black rook moves");

        BOARD_UNSET_BIT(rooks, square);

        i++;
    }
}

void queens_moves(Board* b)
{

}

void kings_moves(Board* b)
{

}

int main(int argc, char** argv)
{
    CCHESS_ATEXIT_REGISTER(move_gen_destroy, true);
    move_gen_init();

    Board b1 = board_from_fen("Kb1n4/1P2r3/R5Pp/3k4/pp5p/4Pp2/PR2pQn1/1b1Br3 w - - 0 1");

    board_debug(&b1);

    rooks_moves(&b1);

    return 0;
}