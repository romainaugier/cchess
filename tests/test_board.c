#include "cchess/board.h"

#include <stdio.h>

int main(int argc, char** argv)
{
    CCHESS_ATEXIT_REGISTER(move_gen_destroy, true);
    move_gen_init();

    Board b = board_init();

    board_debug(&b);

    Board b_check = board_from_fen("rnbqkbnr/ppp1Qppp/8/3p4/4P3/8/PPPP1PPP/RNB1KBNR b KQkq - 0 3");

    board_debug(&b_check);

    board_has_check(&b_check);

    CCHESS_ASSERT(board_has_check(&b_check));

    return 0;
}