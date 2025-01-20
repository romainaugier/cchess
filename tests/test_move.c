#include "cchess/board.h"
#include "cchess/move.h"

#define ROMANO_ENABLE_PROFILING
#include "libromano/profiling.h"

#define DEBUG_MOVE_MASKS 1

#include <stdio.h>

int main(int argc, char** argv)
{
    CCHESS_ATEXIT_REGISTER(move_gen_destroy, true);

    move_gen_init();

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

    const uint64_t bishop_moves = move_gen_bishop(8, 0, 0, 0);
    board_debug_move_mask(bishop_moves, 8);

    return 0;
}