#include "cchess/board.h"
#include "cchess/move.h"

#include "libromano/logger.h"

#define ROMANO_ENABLE_PROFILING
#include "libromano/profiling.h"

#define DEBUG_MOVE_MASKS 1

#if CCHESS_DEBUG
#define PERFT 6
#else
#define PERFT 7
#endif /* CCHESS_DEBUG */

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

    logger_log(LogLevel_Info, "Starting perft test");

    const uint64_t perft_result[7] = {
        20ULL,
        400ULL,
        8902ULL,
        197281ULL,
        4865609ULL,
        119060324ULL,
        3195901860ULL,
    };

    for(uint32_t i = 1; i < PERFT; i++)
    {
        logger_log(LogLevel_Info, "Perft %u: %llu", i, board_perft(&b, i));
    }

    logger_release();

    return 0;
}