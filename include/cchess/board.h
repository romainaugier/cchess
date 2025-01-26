#pragma once

#if !defined(__BOARD)
#define __BOARD

#include "cchess/cchess.h"
#include "cchess/move.h"
#include "cchess/bit_utils.h"

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

typedef enum 
{
    BoardState_WhiteQueenSideCastleAvailable = 0x1,
    BoardState_WhiteKingSideCastleAvailable = 0x2,
    BoardState_BlackQueenSideCastleAvailable = 0x4,
    BoardState_BlackKingSideCastleAvailable = 0x8,
    BoardState_EnPassantAvailable = 0x10,
    BoardState_WhiteToPlay = 0x20,
    BoardState_HasCheck = 0x40,
    BoardState_HasMate = 0x80,
} BoardState;

typedef struct Board 
{
    uint64_t pawns[2];
    uint64_t knights[2];
    uint64_t bishops[2];
    uint64_t rooks[2];
    uint64_t queens[2];
    uint64_t kings[2];

    uint64_t whites;
    uint64_t blacks;

    uint64_t white_attacks;
    uint64_t black_attacks;

    uint64_t all;

    uint32_t state;
} Board;

#define SIDE_TO_PLAY_WHITE 0
#define SIDE_TO_PLAY_BLACK 1

#define BOARD_MAX_MOVES 256

#define BOARD_GET_SIDE_TO_PLAY(board) ((board).state & BoardState_WhiteToPlay)
#define BOARD_TOGGLE_SIDE_TO_PLAY(board) ((board).state ^= BoardState_WhiteToPlay)
#define BOARD_SET_CHECK(board) ((board).state &= BoardState_HasCheck)
#define BOARD_SET_MATE(board) ((board).state &= BoardState_HasMate)
#define BOARD_HAS_CHECK(board) ((board).state & BoardState_HasCheck)
#define BOARD_HAS_MATE(board) ((board).state & BoardState_HasMate)

#define BOARD_PTR_GET_SIDE_TO_PLAY(board) (board->state & BoardState_WhiteToPlay)
#define BOARD_PTR_TOGGLE_SIDE_TO_PLAY(board) (board->state ^= BoardState_WhiteToPlay)
#define BOARD_PTR_SET_CHECK(board) (board->state &= BoardState_HasCheck)
#define BOARD_PTR_SET_MATE(board) (board->state &= BoardState_HasMate)
#define BOARD_PTR_HAS_CHECK(board) (board->state & BoardState_HasCheck)
#define BOARD_PTR_HAS_MATE(board) (board->state & BoardState_HasMate)

#define BOARD_GET_WHITE_PIECES(board) ((board).pawns[0] |   \
                                       (board).knights[0] | \
                                       (board).bishops[0] | \
                                       (board).rooks[0] |   \
                                       (board).queens[0] |  \
                                       (board).kings[0])

#define BOARD_PTR_GET_WHITE_PIECES(board) (board->pawns[0] |   \
                                           board->knights[0] | \
                                           board->bishops[0] | \
                                           board->rooks[0] |   \
                                           board->queens[0] |  \
                                           board->kings[0])

#define BOARD_GET_BLACK_PIECES(board) ((board).pawns[1] |   \
                                       (board).knights[1] | \
                                       (board).bishops[1] | \
                                       (board).rooks[1] |   \
                                       (board).queens[1] |  \
                                       (board).kings[1])

#define BOARD_PTR_GET_BLACK_PIECES(board) (board->pawns[1] |   \
                                           board->knights[1] | \
                                           board->bishops[1] | \
                                           board->rooks[1] |   \
                                           board->queens[1] |  \
                                           board->kings[1])

typedef enum
{
    BoardMoveError_PieceDoesNotExist = 1,
    BoardMoveError_PieceAlreadyOnThisSquare = 2,
    BoardMoveError_PieceCantMoveThere = 3,
    BoardMoveError_NoPieceToCapture = 4,
    BoardMoveError_PieceWrongSide= 5,
} BoardMoveError;


CCHESS_API Board board_init();

CCHESS_API Board board_from_fen(const char* fen);

CCHESS_FORCE_INLINE uint64_t board_get_pawns(Board* board, const uint64_t side)
{
    return board->pawns[side];
}

CCHESS_FORCE_INLINE uint64_t board_num_pawns(Board* board, const uint64_t side)
{
    return popcount_u64(board->pawns[side]);
}

CCHESS_FORCE_INLINE uint64_t board_get_knights(Board* board, const uint64_t side)
{
    return board->knights[side];
}

CCHESS_FORCE_INLINE uint64_t board_num_knights(Board* board, const uint64_t side)
{
    return popcount_u64(board->knights[side]);
}

CCHESS_FORCE_INLINE uint64_t board_get_bishops(Board* board, const uint64_t side)
{
    return board->bishops[side];
}

CCHESS_FORCE_INLINE uint64_t board_num_bishops(Board* board, const uint64_t side)
{
    return popcount_u64(board->bishops[side]);
}

CCHESS_FORCE_INLINE uint64_t board_get_rooks(Board* board, const uint64_t side)
{
    return board->rooks[side];
}

CCHESS_FORCE_INLINE uint64_t board_num_rooks(Board* board, const uint64_t side)
{
    return popcount_u64(board->rooks[side]);
}

CCHESS_FORCE_INLINE uint64_t board_get_queens(Board* board, const uint64_t side)
{
    return board->queens[side];
}

CCHESS_FORCE_INLINE uint64_t board_num_queens(Board* board, const uint64_t side)
{
    return popcount_u64(board->queens[side]);
}

CCHESS_FORCE_INLINE uint64_t board_get_king(Board* board, const uint64_t side)
{
    return board->kings[side];
}

CCHESS_FORCE_INLINE uint64_t board_get_white(Board* board)
{
    return board->whites;
}

CCHESS_FORCE_INLINE uint64_t board_get_black(Board* board)
{
    return board->blacks;
}

CCHESS_FORCE_INLINE uint64_t board_get_white_attacks(Board* board)
{
    return board->white_attacks;
}

CCHESS_FORCE_INLINE uint64_t board_get_black_attacks(Board* board)
{
    return board->black_attacks;
}

CCHESS_FORCE_INLINE uint64_t board_get_all(Board* board)
{
    return board->all;
}

CCHESS_FORCE_INLINE bool board_has_piece(Board* board,
                                         const uint32_t piece,
                                         const uint32_t square,
                                         const uint32_t side)
{
    const uint64_t piece_mask = 1ULL << square;

    uint64_t* board_as_ptr = (uint64_t*)board;

    return board_as_ptr[piece * 2UL + side] & piece_mask;
}

CCHESS_API void board_get_moves(Board* board, Move* moves, size_t* moves_count);

CCHESS_API bool board_move_is_legal(Board* board, const Move move);

CCHESS_API bool board_move_is_legal_algebraic(Board* board, const char* move);

CCHESS_API uint32_t board_make_move(Board* board, const Move move);

CCHESS_API bool board_make_move_algebraic(Board* board, const char* move);

CCHESS_API bool board_has_check_from_last_move(Board* board, const Move last_move);

CCHESS_API bool board_has_check(Board* board);

CCHESS_API bool board_has_mate_from_last_move(Board* board, const Move last_move);

CCHESS_API bool board_has_mate(Board* board);

typedef enum
{
    BoardMoveIteratorFlag_PieceWhite = 0x1,
    BoardMoveIteratorFlag_PieceBlack = 0x2,
} BoardMoveIteratorFlag;

typedef struct
{
    uint32_t piece_type;
    uint32_t side;
    uint32_t piece;
    uint32_t move;
    uint32_t flags;
} BoardMoveIterator;

CCHESS_API BoardMoveIterator board_move_iterator_init();

CCHESS_API bool board_legal_moves_iterator(Board* board, Move* move, BoardMoveIterator* it);

CCHESS_API uint64_t board_perft(Board* board, uint32_t num_plies);

CCHESS_API void board_debug(Board* board);

CCHESS_API void board_debug_move_masks(Board* board);

CCHESS_API void board_debug_mask(uint64_t mask);

CCHESS_API void board_debug_move_mask(uint64_t mask, uint64_t square);

#endif /* !defined(__BOARD) */