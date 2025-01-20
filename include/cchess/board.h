#pragma once

#if !defined(__BOARD)
#define __BOARD

#include "cchess/cchess.h"

typedef enum 
{
    Piece_Pawn = 0,
    Piece_Knight = 1,
    Piece_Bishop = 2,
    Piece_Rook = 3,
    Piece_Queen = 4,
    Piece_King = 5

} Piece;

#define PIECE_WHITE 0
#define PIECE_BLACK 1

#define CAPTURING_MOVE 1

typedef enum 
{
    State_WhiteQueenSideCastleAvailable = 0x1,
    State_WhiteKingSideCastleAvailable = 0x2,
    State_BlackQueenSideCastleAvailable = 0x4,
    State_BlackKingSideCastleAvailable = 0x8,
    State_EnPassantAvailable = 0x10,
    State_WhiteToPlay = 0x20
} State;

typedef struct Board 
{
    uint64_t pawns[2];
    uint64_t knights[2];
    uint64_t bishops[2];
    uint64_t rooks[2];
    uint64_t queens[2];
    uint64_t kings[2];

    uint8_t state;
} Board;

/* 
    A move is represented using 16 bits

    0  000  000000  000000
    c   P    from     to

    c stands for capturing or not
    P stands for piece
    from is the file and rank from where we move
    to is the file and rank where we move
*/

typedef uint16_t Move;

typedef enum
{
    IteratorMoveType_White = 1,
    IteratorMoveType_Black = 2,
    IteratorMoveType_All = 3
} IteratorMoveType;

CCHESS_API Board board_init();

CCHESS_API Board board_from_fen(const char* fen);

CCHESS_API bool board_legal_moves_iterator(Board* board, Move* move, IteratorMoveType move_type);

CCHESS_API void board_debug(Board* board);

CCHESS_API void board_debug_move_masks(Board* board);

CCHESS_API void board_debug_move_mask(uint64_t mask, uint64_t square);

#endif /* !defined(__BOARD) */