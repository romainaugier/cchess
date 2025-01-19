#include "cchess/board.h"

#include <stdio.h>

int main(int argc, char** argv)
{
    const char* start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board b = board_from_fen(start_fen);

    printf("FEN: %s\n", start_fen);
    board_debug(&b);

    return 0;
}