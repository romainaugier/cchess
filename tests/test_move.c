#include "cchess/board.h"
#include "cchess/move.h"
#include "cchess/magic_bitboards.h"

#define ROMANO_ENABLE_PROFILING
#include "libromano/profiling.h"

#define DEBUG_MOVE_MASKS 1

#include <stdio.h>

void test_rook_moves(uint32_t square) 
{
    printf("Testing rook moves for square %d\n", square);
    
    const uint64_t mask = move_gen_rook_mask_special(square);
    board_debug_move_mask(mask, square);
}

int main(int argc, char** argv)
{
    CCHESS_ATEXIT_REGISTER(magic_bitboards_destroy, true);

    magic_bitboards_init();

    const char* start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board b = board_from_fen(start_fen);

#if DEBUG_MOVE_MASKS
    board_debug_move_masks(&b);
#endif /* DEBUG_MOVE_MASKS */

    SCOPED_PROFILE_START_MICROSECONDS(legal_moves);

    Move m;

    uint32_t plies = 1;

    uint64_t num_moves = 0;

    while(board_legal_moves_iterator(&b, &m, IteratorMoveType_All))
    {
        num_moves++;
    };

    SCOPED_PROFILE_END_MICROSECONDS(legal_moves);

    printf("Num moves found for %u plies: %llu\n", plies, num_moves);

    test_rook_moves(28);

    return 0;
}