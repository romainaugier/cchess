#pragma once

#if !defined(__BOARD)
#define __BOARD

#include "cchess/cchess.h"
#include "cchess/move.h"

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
#define PIECE_NONE 2

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

typedef enum
{
    BoardMoveError_PieceDoesNotExist = 1,
    BoardMoveError_PieceCantMoveThere = 2,
    BoardMoveError_PieceCantCapture = 3,
} BoardMoveError;

typedef enum
{
    IteratorMoveType_White = 1,
    IteratorMoveType_Black = 2,
    IteratorMoveType_All = 3
} IteratorMoveType;

CCHESS_API Board board_init();

CCHESS_API Board board_from_fen(const char* fen);

CCHESS_API uint32_t board_has_piece(Board* board,
                                    const uint32_t piece,
                                    const uint32_t square);

CCHESS_API uint32_t board_make_move(Board* board, Move move);

CCHESS_API void board_make_move_no_check(Board* board, Move move);

CCHESS_API bool board_legal_moves_iterator(Board* board, Move* move, IteratorMoveType move_type);

CCHESS_API uint64_t board_perft(Board* board, uint32_t num_plies);

CCHESS_API void board_debug(Board* board);

CCHESS_API void board_debug_move_masks(Board* board);

CCHESS_API void board_debug_move_mask(uint64_t mask, uint64_t square);

#endif /* !defined(__BOARD) */