#include "cchess/board.h"
#include "cchess/move.h"

#include "libromano/logger.h"

#define ROMANO_ENABLE_PROFILING
#include "libromano/profiling.h"

#define DEBUG_MOVE_MASKS 1

#include <stdio.h>

int main(int argc, char** argv)
{
    CCHESS_ATEXIT_REGISTER(move_gen_destroy, true);

    logger_init();

    move_gen_init();

    const char* start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board b = board_from_fen(start_fen);

#if DEBUG_MOVE_MASKS
    board_debug_move_masks(&b);
#endif /* DEBUG_MOVE_MASKS */

    SCOPED_PROFILE_START_MICROSECONDS(legal_moves);

    Move m;
    BoardMoveIterator move_it = board_move_iterator_init();

    uint32_t plies = 1;

    uint64_t num_moves = 0;

    while(board_legal_moves_iterator(&b, &m, &move_it))
    {
        num_moves++;
    };

    SCOPED_PROFILE_END_MICROSECONDS(legal_moves);

    printf("Num moves found for %u plies: %llu\n", plies, num_moves);

    logger_log(LogLevel_Info, "Starting perft test");

    for(uint32_t i = 1; i < 5; i++)
    {
        logger_log(LogLevel_Info, "Perft %u: %llu", i, board_perft(&b, i));
    }

    logger_release();

    return 0;
}