#include "cchess/board.h"
#include "cchess/move.h"
#include "cchess/magic_bitboards.h"

#define CCHESS_ENABLE_PROFILING
#include "cchess/profiling.h"

#define DEBUG_MOVE_MASKS 1

#include <stdio.h>

void test_rook_moves(uint32_t square) 
{
    printf("Testing rook moves for square %d\n", square);
    
    // First, let's see the mask
    uint64_t mask = move_gen_rook_mask_special(square);
    printf("Special rook mask:\n");
    board_debug_move_mask(mask, square);
    
    // Let's try with no blockers
    uint64_t no_blockers = 0ULL;
    printf("Index for no blockers: %u\n", magic_bitboards_get_rook_index(square, no_blockers & mask));
    
    uint64_t moves = magic_bitboards_get_rook(square, no_blockers & mask);
    printf("Moves with no blockers:\n");
    board_debug_move_mask(moves, square);
    
    // Try with one blocker
    uint64_t one_blocker = 1ULL << (square + 8); // one square up
    printf("Index for one blocker: %u\n", magic_bitboards_get_rook_index(square, one_blocker & mask));
    
    moves = magic_bitboards_get_rook(square, one_blocker & mask);
    printf("Moves with one blocker:\n");
    board_debug_move_mask(moves, square);
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