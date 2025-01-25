#include "cchess/board.h"
#include "cchess/move.h"
#include "cchess/board_macros.h"

#include <stdio.h>

/*
    Uses verified hard-coded numbers to verify move generation is correct
*/

#define PRINT_MOVE_MASKS_VALUES 0

void pawns_moves(Board* b)
{
    const uint64_t white_pieces = BOARD_PTR_GET_WHITE_PIECES(b);
    const uint64_t black_pieces = BOARD_PTR_GET_BLACK_PIECES(b);

    uint64_t pawns = board_get_pawns(b, SIDE_TO_PLAY_WHITE);

    const uint64_t pawns_moves_white[4] = {
        65536ULL,
        268435456ULL,
        18014398509481984ULL,
        0ULL,
    };

    uint64_t i = 0;

    while(pawns > 0)
    {
        const uint64_t square = ctz_u64(pawns);
        const uint64_t moves = move_gen_pawn(square, 
                                             SIDE_TO_PLAY_WHITE, 
                                             white_pieces, 
                                             black_pieces);

#if PRINT_MOVE_MASKS_VALUES
        printf("White pawn: %zu\n", moves);
#else
        CCHESS_ASSERT(moves == pawns_moves_white[i] && "Invalid white pawn moves");
#endif /* PRINT_MOVE_MASKS_VALUES */

        BOARD_UNSET_BIT(pawns, square);

        i++;
    }

    pawns = board_get_pawns(b, SIDE_TO_PLAY_BLACK);

    const uint64_t pawns_moves_black[6] = {
        8ULL,
        0ULL,
        65536ULL,
        131072ULL,
        8388608ULL,
        1649267441664ULL,
    };

    i = 0;

    while(pawns > 0)
    {
        const uint64_t square = ctz_u64(pawns);
        const uint64_t moves = move_gen_pawn(square, 
                                             SIDE_TO_PLAY_BLACK, 
                                             white_pieces, 
                                             black_pieces);

#if PRINT_MOVE_MASKS_VALUES
        printf("Black pawn: %zu\n", moves);
#else
        CCHESS_ASSERT(moves == pawns_moves_black[i] && "Invalid black pawn moves");
#endif /* PRINT_MOVE_MASKS_VALUES */

        BOARD_UNSET_BIT(pawns, square);

        i++;
    }
}

void knights_moves(Board* b)
{
    const uint64_t white_pieces = BOARD_PTR_GET_WHITE_PIECES(b);
    const uint64_t black_pieces = BOARD_PTR_GET_BLACK_PIECES(b);

    uint64_t knights = board_get_knights(b, SIDE_TO_PLAY_WHITE);

    const uint64_t knights_moves_white[0] = {};

    uint64_t i = 0;

    while(knights > 0)
    {
        const uint64_t square = ctz_u64(knights);
        const uint64_t moves = move_gen_knight(square, 
                                               SIDE_TO_PLAY_WHITE, 
                                               white_pieces, 
                                               black_pieces);

#if PRINT_MOVE_MASKS_VALUES
        printf("White knight: %zu\n", moves);
#else
        CCHESS_ASSERT(moves == knights_moves_white[i] && "Invalid white knight moves");
#endif /* PRINT_MOVE_MASKS_VALUES */

        BOARD_UNSET_BIT(knights, square);

        i++;
    }

    knights = board_get_knights(b, SIDE_TO_PLAY_BLACK);

    const uint64_t knights_moves_black[2] = {
        537919488ULL,
        9592139440717824ULL,
    };

    i = 0;

    while(knights > 0)
    {
        const uint64_t square = ctz_u64(knights);
        const uint64_t moves = move_gen_knight(square, 
                                               SIDE_TO_PLAY_BLACK, 
                                               white_pieces, 
                                               black_pieces);

#if PRINT_MOVE_MASKS_VALUES
        printf("Black knight: %zu\n", moves);
#else 
        CCHESS_ASSERT(moves == knights_moves_black[i] && "Invalid black knight moves");
#endif /* PRINT_MOVE_MASKS_VALUES */

        BOARD_UNSET_BIT(knights, square);

        i++;
    }
}

void bishops_moves(Board* b)
{
    const uint64_t white_pieces = BOARD_PTR_GET_WHITE_PIECES(b);
    const uint64_t black_pieces = BOARD_PTR_GET_BLACK_PIECES(b);

    uint64_t bishops = board_get_bishops(b, SIDE_TO_PLAY_WHITE);

    const uint64_t bishops_moves_white[1] = {
        16913408ULL,
    };

    uint64_t i = 0;

    while(bishops > 0)
    {
        const uint64_t square = ctz_u64(bishops);
        const uint64_t moves = move_gen_bishop(square, 
                                               SIDE_TO_PLAY_WHITE, 
                                               white_pieces, 
                                               black_pieces);

#if PRINT_MOVE_MASKS_VALUES
        printf("White bishop: %zu\n", moves);
#else
        CCHESS_ASSERT(moves == bishops_moves_white[i] && "Invalid white bishop moves");
#endif /* PRINT_MOVE_MASKS_VALUES */

        BOARD_UNSET_BIT(bishops, square);

        i++;
    }

    bishops = board_get_bishops(b, SIDE_TO_PLAY_BLACK);

    const uint64_t bishops_moves_black[2] = {
        70506452092160ULL,
        1416240237150208ULL,
    };

    i = 0;

    while(bishops > 0)
    {
        const uint64_t square = ctz_u64(bishops);
        const uint64_t moves = move_gen_bishop(square, 
                                               SIDE_TO_PLAY_BLACK, 
                                               white_pieces, 
                                               black_pieces);

#if PRINT_MOVE_MASKS_VALUES
        printf("Black bishop: %zu\n", moves);
#else
        CCHESS_ASSERT(moves == bishops_moves_black[i] && "Invalid black bishop moves");
#endif /* PRINT_MOVE_MASKS_VALUES */

        BOARD_UNSET_BIT(bishops, square);

        i++;
    }
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

#if PRINT_MOVE_MASKS_VALUES
        printf("White rooks: %zu\n", moves);
#else
        CCHESS_ASSERT(moves == rooks_moves_white[i] && "Invalid white rook moves");
#endif /* PRINT_MOVE_MASKS_VALUES */

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

#if PRINT_MOVE_MASKS_VALUES
        printf("Black rooks: %zu\n", moves);
#else
        CCHESS_ASSERT(moves == rooks_moves_black[i] && "Invalid black rook moves");
#endif /* PRINT_MOVE_MASKS_VALUES */

        BOARD_UNSET_BIT(rooks, square);

        i++;
    }
}

void queens_moves(Board* b)
{
    const uint64_t white_pieces = BOARD_PTR_GET_WHITE_PIECES(b);
    const uint64_t black_pieces = BOARD_PTR_GET_BLACK_PIECES(b);

    uint64_t queens = board_get_queens(b, SIDE_TO_PLAY_WHITE);

    const uint64_t queens_moves_white[1] = {
        2153795696ULL,
    };

    uint64_t i = 0;

    while(queens > 0)
    {
        const uint64_t square = ctz_u64(queens);
        const uint64_t moves = move_gen_queen(square, 
                                              SIDE_TO_PLAY_WHITE, 
                                              white_pieces, 
                                              black_pieces);

#if PRINT_MOVE_MASKS_VALUES
        printf("White queen: %zu\n", moves);
#else
        CCHESS_ASSERT(moves == queens_moves_white[i] && "Invalid white queen moves");
#endif /* PRINT_MOVE_MASKS_VALUES */

        BOARD_UNSET_BIT(queens, square);

        i++;
    }

    queens = board_get_queens(b, SIDE_TO_PLAY_BLACK);

    const uint64_t queens_moves_black[0] = {};

    i = 0;

    while(queens > 0)
    {
        const uint64_t square = ctz_u64(queens);
        const uint64_t moves = move_gen_queen(square, 
                                              SIDE_TO_PLAY_BLACK, 
                                              white_pieces, 
                                              black_pieces);

#if PRINT_MOVE_MASKS_VALUES
        printf("Black queen: %zu\n", moves);
#else
        CCHESS_ASSERT(moves == queens_moves_black[i] && "Invalid black queen moves");
#endif /* PRINT_MOVE_MASKS_VALUES */

        BOARD_UNSET_BIT(queens, square);

        i++;
    }
}

void kings_moves(Board* b)
{

    const uint64_t white_pieces = BOARD_PTR_GET_WHITE_PIECES(b);
    const uint64_t black_pieces = BOARD_PTR_GET_BLACK_PIECES(b);

    uint64_t king = board_get_king(b, SIDE_TO_PLAY_WHITE);

    const uint64_t kings_moves_white[1] = {
        9403938234414661632ULL,
    };

    uint64_t i = 0;

    while(king > 0)
    {
        const uint64_t square = ctz_u64(king);
        const uint64_t moves = move_gen_king(square, 
                                             SIDE_TO_PLAY_WHITE, 
                                             white_pieces, 
                                             black_pieces);

#if PRINT_MOVE_MASKS_VALUES
        printf("White king: %zu\n", moves);
#else
        CCHESS_ASSERT(moves == kings_moves_white[i] && "Invalid white king moves");
#endif /* PRINT_MOVE_MASKS_VALUES */

        BOARD_UNSET_BIT(king, square);

        i++;
    }

    king = board_get_king(b, SIDE_TO_PLAY_BLACK);

    const uint64_t kings_moves_black[1] = {
        30872694685696ULL,
    };

    i = 0;

    while(king > 0)
    {
        const uint64_t square = ctz_u64(king);
        const uint64_t moves = move_gen_king(square, 
                                             SIDE_TO_PLAY_BLACK, 
                                             white_pieces, 
                                             black_pieces);

#if PRINT_MOVE_MASKS_VALUES
        printf("Black king: %zu\n", moves);
#else
        CCHESS_ASSERT(moves == kings_moves_black[i] && "Invalid black king moves");
#endif /* PRINT_MOVE_MASKS_VALUES */

        BOARD_UNSET_BIT(king, square);

        i++;
    }
}

int main(int argc, char** argv)
{
    CCHESS_ATEXIT_REGISTER(move_gen_destroy, true);
    move_gen_init();

    Board b1 = board_from_fen("Kb1n4/1P2r3/R5Pp/3k4/pp5p/4Pp2/PR2pQn1/1b1Br3 w - - 0 1");

    board_debug(&b1);

    pawns_moves(&b1);
    knights_moves(&b1);
    bishops_moves(&b1);
    rooks_moves(&b1);
    queens_moves(&b1);
    kings_moves(&b1);

    return 0;
}