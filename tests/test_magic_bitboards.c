#include "cchess/magic_bitboards.h"

#include "libromano/logger.h"

#undef ROMANO_ENABLE_PROFILING
#include "libromano/profiling.h"

#define FIND_MAGIC_NUMBERS 1

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    CCHESS_ATEXIT_REGISTER(magic_bitboards_destroy, true);

    logger_init();

    logger_log(LogLevel_Info, "Starting magic_bitboards test");

    magic_bitboards_init();

#if FIND_MAGIC_NUMBERS
    logger_log(LogLevel_Info, "Starting magic numbers search");

    magic_number_find_all();
#endif /* FIND_MAGIC_NUMBERS */

    logger_release();

    return 0;
}