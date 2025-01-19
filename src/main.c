#include "cchess/board.h"

int main(int argc, char** argv)
{
    Board b = board_init();
    board_debug(&b);

    return 0;
}