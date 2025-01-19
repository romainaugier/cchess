#include "cchess/magic_bitboards.h"

#undef CCHESS_ENABLE_PROFILING
#include "cchess/profiling.h"

#define FIND_MAGIC_NUMBERS 0

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    CCHESS_ATEXIT_REGISTER(magic_bitboards_destroy, true);

    magic_bitboards_init();

#if FIND_MAGIC_NUMBERS
    magic_number_find_all();
#endif /* FIND_MAGIC_NUMBERS */
    return 0;
}