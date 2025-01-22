#include "cchess/board.h"
#include "cchess/bit_utils.h"
#include "cchess/char_utils.h"
#include "cchess/board_macros.h"

#include <stdio.h>
#include <string.h>

Board board_init()
{
    Board b;
    memset(&b, 0, sizeof(b));

    b.pawns[0] |= RANK2;
    b.pawns[1] |= RANK7;

    b.knights[0] |= INITIAL_WHITE_KNIGHTS;
    b.knights[1] |= INITIAL_BLACK_KNIGHTS;

    b.bishops[0] |= INITIAL_WHITE_BISHOPS;
    b.bishops[1] |= INITIAL_BLACK_BISHOPS;

    b.rooks[0] |= INITIAL_WHITE_ROOKS;
    b.rooks[1] |= INITIAL_BLACK_ROOKS;

    b.queens[0] |= INITIAL_WHITE_QUEEN;
    b.queens[1] |= INITIAL_BLACK_QUEEN;

    b.kings[0] |= INITIAL_WHITE_KING;
    b.kings[1] |= INITIAL_BLACK_KING;

    b.state |= BoardState_WhiteKingSideCastleAvailable;
    b.state |= BoardState_WhiteQueenSideCastleAvailable;
    b.state |= BoardState_BlackKingSideCastleAvailable;
    b.state |= BoardState_BlackQueenSideCastleAvailable;
    b.state |= BoardState_WhiteToPlay;

    return b;
}

#define ASCII_OFFSET 65  
#define PIECE_LOOKUP_SIZE 57

const int8_t __fen_to_piece[PIECE_LOOKUP_SIZE] = {
    ['P' - ASCII_OFFSET] = (Piece_Pawn + 1),
    ['N' - ASCII_OFFSET] = (Piece_Knight + 1),
    ['B' - ASCII_OFFSET] = (Piece_Bishop + 1),
    ['R' - ASCII_OFFSET] = (Piece_Rook + 1),
    ['Q' - ASCII_OFFSET] = (Piece_Queen + 1),
    ['K' - ASCII_OFFSET] = (Piece_King + 1),
    ['p' - ASCII_OFFSET] = -(Piece_Pawn + 1),
    ['n' - ASCII_OFFSET] = -(Piece_Knight + 1),
    ['b' - ASCII_OFFSET] = -(Piece_Bishop + 1),
    ['r' - ASCII_OFFSET] = -(Piece_Rook + 1),
    ['q' - ASCII_OFFSET] = -(Piece_Queen + 1),
    ['k' - ASCII_OFFSET] = -(Piece_King + 1),
};

Board board_from_fen(const char* fen)
{
    Board b;
    memset(&b, 0, sizeof(b));

    uint64_t* b_ptr = (uint64_t*)&b;

    char* s = (char*)fen;

    int32_t current_rank = 7;
    int32_t current_file = 0;

    while(*s != '\0')
    {
        if(*s == '/')
        {
            current_rank--;
            current_file = 0;

            goto end;
        }
        else if(is_letter(*s))
        {
            const int8_t piece = __fen_to_piece[*s - ASCII_OFFSET];
            const int8_t side = (piece < 0 ? PIECE_BLACK : PIECE_WHITE);
            const uint8_t piece_num = abs_u8(piece) - 1;
            const uint32_t b_index = (piece_num + side) * 2 - side;

            BOARD_SET_BIT_FROM_FILE_RANK(b_ptr[b_index], current_file, current_rank);
        }
        else if(is_digit(*s))
        {
            const uint8_t d = to_digit(*s);

            current_file += d;

            goto end;
        }
        else if(*s == ' ')
        {
            while(*s != '\0')
            {
                switch (*s)
                {
                    case 'w':
                        SET_BIT(b.state, BoardState_WhiteToPlay);
                        break;
                    case 'K':
                        SET_BIT(b.state, BoardState_WhiteKingSideCastleAvailable);
                        break;
                    case 'Q':
                        SET_BIT(b.state, BoardState_WhiteQueenSideCastleAvailable);
                        break;
                    case 'k':
                        SET_BIT(b.state, BoardState_BlackKingSideCastleAvailable);
                        break;
                    case 'q':
                        SET_BIT(b.state, BoardState_BlackQueenSideCastleAvailable);
                        break;

                    // TODO: en passant
                    
                    default:
                        break;
                }

                s++;
            }

            break;
        }
        else
        {
            break;
        }

        current_file++;

end:
        s++;
    }

    return b;
}

/* Moves */

move_gen_func __move_gen_funcs[6] = {
    move_gen_pawn,
    move_gen_knight,
    move_gen_bishop,
    move_gen_rook,
    move_gen_queen,
    move_gen_king
};

uint64_t board_get_move_mask_all_pieces(Board* board, const uint32_t side)
{
    const uint64_t pieces_white = BOARD_PTR_GET_WHITE_PIECES(board);
    const uint64_t pieces_black = BOARD_PTR_GET_BLACK_PIECES(board);

    uint64_t mask = 0;

    uint64_t* board_as_ptr = (uint64_t*)board;

    for(uint32_t i = 0; i < 6; i++)
    {
        uint64_t pieces = board_as_ptr[i * 2 + side];

        while(pieces > 0)
        {
            const uint32_t square = ctz_u64(pieces);

            mask |= __move_gen_funcs[i](square, side, pieces_white, pieces_black);

            UNSET_BIT(pieces, BIT64(square));
        }
    }

    return mask;
}

uint64_t board_get_pinned_pieces(Board* board, const uint32_t side)
{
    return 0;
}

uint32_t board_make_move(Board* board, const Move move)
{
    const uint32_t piece = MOVE_GET_PIECE(move);
    const uint32_t from_square = MOVE_GET_FROM_SQUARE(move);
    const uint32_t to_square = MOVE_GET_TO_SQUARE(move);
    const uint32_t side = (uint32_t)!(board->state & BoardState_WhiteToPlay);

    const bool has_piece = board_has_piece(board, piece, from_square, side);

    if(!has_piece)
    {
        return BoardMoveError_PieceDoesNotExist;
    }

    const bool has_piece_target = board_has_piece(board, piece, to_square, side);

    if(has_piece_target)
    {
        return BoardMoveError_PieceAlreadyOnThisSquare;
    }

    const uint64_t from_piece_mask = ~(1 << from_square);
    const uint64_t to_piece_mask = 1 << to_square;

    uint64_t* board_as_ptr = (uint64_t*)board;

    board_as_ptr[piece * 2UL + side] &= from_piece_mask;
    board_as_ptr[piece * 2UL + side] |= to_piece_mask;

    const bool is_capturing = (bool)MOVE_GET_IS_CAPTURING(move);

    if(is_capturing)
    {
        bool has_captured = false;

        for(uint32_t i = 0; i < 6; i++)
        {
            board_as_ptr[i * 2UL + !side] &= ~to_piece_mask;

            if(board_as_ptr[i * 2UL + !side] & to_piece_mask)
            {
                has_captured = true;
                break;
            }
        }

        if(!has_captured)
        {
            return BoardMoveError_NoPieceToCapture;
        }
    }

    BOARD_TOGGLE_SIDE_TO_PLAY(*board);

    return 0;
}

void board_make_move_unsafe(Board* board, const Move move)
{
    const uint32_t piece = MOVE_GET_PIECE(move);
    const uint32_t from_square = MOVE_GET_FROM_SQUARE(move);
    const uint32_t to_square = MOVE_GET_TO_SQUARE(move);
    const uint64_t from_piece_mask = 1ULL << from_square;
    const uint64_t to_piece_mask = 1ULL << to_square;
    const uint32_t side = (uint32_t)!(board->state & BoardState_WhiteToPlay);

    uint64_t* board_as_ptr = (uint64_t*)board;

    CCHESS_ASSERT((board_as_ptr[piece * 2UL + side] & from_piece_mask) && 
                  "Piece does not exist on this square of the board");

    CCHESS_ASSERT(!(board_as_ptr[piece * 2UL + side] & to_piece_mask) && 
                  "There is already a piece on this square");

    board_as_ptr[piece * 2 + side] &= ~from_piece_mask;
    board_as_ptr[piece * 2 + side] |= to_piece_mask;

    const bool is_capturing = (bool)MOVE_GET_IS_CAPTURING(move);

    if(is_capturing)
    {
        for(uint32_t i = 0; i < 6; i++)
        {
            board_as_ptr[i * 2UL + !side] &= to_piece_mask;
        }
    }
}

uint32_t board_make_move_algebraic(Board* board, const char* move)
{
    return 0;
}

bool board_has_check_from_last_move(Board* board, Move last_move)
{
    const uint64_t white_pieces = BOARD_PTR_GET_WHITE_PIECES(board);
    const uint64_t black_pieces = BOARD_PTR_GET_BLACK_PIECES(board);

    const uint32_t to_square = MOVE_GET_TO_SQUARE(last_move);
    const uint32_t piece = MOVE_GET_PIECE(last_move);
    const uint32_t side = BOARD_GET_SIDE_TO_PLAY(*board);

    const uint64_t new_move_mask = __move_gen_funcs[piece](to_square,
                                                           side,
                                                           white_pieces,
                                                           black_pieces);

    const uint64_t king_mask = board->kings[!side];

    return new_move_mask & king_mask;
}

bool board_has_check(Board* board)
{
    const uint32_t side = BOARD_GET_SIDE_TO_PLAY(*board);
    const uint64_t king_mask = board->kings[!side];
    const uint64_t attack_mask = board_get_move_mask_all_pieces(board, side);

    return king_mask & attack_mask;
}

bool board_has_mate_from_last_move(Board* board, Move last_move)
{
    const uint64_t white_pieces = BOARD_PTR_GET_WHITE_PIECES(board);
    const uint64_t black_pieces = BOARD_PTR_GET_BLACK_PIECES(board);

    const uint32_t to_square = MOVE_GET_TO_SQUARE(last_move);
    const uint32_t piece = MOVE_GET_PIECE(last_move);
    const uint32_t side = BOARD_GET_SIDE_TO_PLAY(*board);
    return false;
}

bool board_has_mate(Board* board)
{
    return false;
}

BoardMoveIterator board_move_iterator_init()
{
    BoardMoveIterator it;
    memset(&it, 0, sizeof(BoardMoveIterator));

    return it;
}

bool board_legal_moves_iterator(Board* board, Move* move, BoardMoveIterator* it)
{
    const uint64_t white_pieces = BOARD_PTR_GET_WHITE_PIECES(board);
    const uint64_t black_pieces = BOARD_PTR_GET_BLACK_PIECES(board);

    const uint64_t pieces[2] = {
        white_pieces,
        black_pieces,
    };

    const uint64_t inv_pieces[2] = {
        ~white_pieces,
        ~black_pieces,
    };

    const uint64_t all_pieces = white_pieces | black_pieces;
    const uint64_t inv_all_pieces = inv_pieces[0] | inv_pieces[1];

    while(it->piece_type < 6)
    {
        while(it->side < 2)
        {
            const uint32_t b_index = it->piece_type * 2 + it->side;

            uint64_t b = ((int64_t*)board)[b_index];

            const uint8_t piece = it->piece_type;
            const uint8_t side = it->side;

            MOVE_SET_PIECE(*move, piece);

            const uint64_t num_pieces = popcount_u64(b);

            while(it->piece < num_pieces)
            {
                const uint32_t piece_index = ctz_u64(b);
                const uint64_t move_mask = __move_gen_funcs[piece](piece_index,
                                                                    side,
                                                                    white_pieces,
                                                                    black_pieces);

                const uint64_t num_moves = popcount_u64(move_mask);

                while(it->move < num_moves)
                {
                    MOVE_SET_IS_CAPTURING(*move, 1);

                    it->move++;

                    return true;
                }

                UNSET_BIT(b, BIT64(piece_index));

                it->move = 0;
                it->piece++;
            }

            it->piece = 0;
            it->side++;
        }

        it->side = 0;
        it->piece_type++;
    }

    it->piece_type = 0;

    return false;
}

uint64_t board_perft_recurse(Board* board, uint32_t depth, uint32_t max_depth)
{
    if(depth == max_depth)
    {
        return 0;
    }

    uint64_t total_moves = 0;

    const uint32_t side = !(board->state & BoardState_WhiteToPlay);

    const uint64_t white_pieces = board->pawns[0] |
                                  board->knights[0] |
                                  board->bishops[0] |
                                  board->rooks[0] |
                                  board->queens[0] |
                                  board->kings[0];

    const uint64_t black_pieces = board->pawns[1] |
                                  board->knights[1] |
                                  board->bishops[1] |
                                  board->rooks[1] |
                                  board->queens[1] |
                                  board->kings[1];

    const uint64_t pieces[2] = {
        white_pieces,
        black_pieces,
    };

    const uint64_t inv_pieces[2] = {
        ~white_pieces,
        ~black_pieces,
    };

    const uint64_t all_pieces = white_pieces | black_pieces;
    const uint64_t inv_all_pieces = inv_pieces[0] | inv_pieces[1];

    for(uint32_t i = 0; i < 6; i++)
    {
        const uint32_t b_index = i * 2 + side;

        uint64_t b = ((int64_t*)board)[b_index];

        const uint8_t piece = (uint8_t)i;

        Move m;

        MOVE_SET_PIECE(m, piece);

        const uint64_t num_pieces = popcount_u64(b);

        for(uint32_t k = 0; k < num_pieces; k++)
        {
            const uint32_t from_square = ctz_u64(b);
            uint64_t move_mask = __move_gen_funcs[piece](from_square,
                                                         side,
                                                         white_pieces,
                                                         black_pieces);

            const uint64_t num_moves = popcount_u64(move_mask);

            MOVE_SET_FROM_SQUARE(m, from_square);

            for(uint32_t l = 0; l < num_moves; l++)
            {
                const uint32_t to_square = ctz_u64(move_mask);
                MOVE_SET_TO_SQUARE(m, to_square);

                total_moves++;

                Board new_board = *board;
                board_make_move_unsafe(&new_board, m);

                total_moves += board_perft_recurse(&new_board, depth + 1, max_depth);

                UNSET_BIT(move_mask, BIT64(to_square));
            }

            UNSET_BIT(b, BIT64(from_square));
        }
    }

    return total_moves;
}

uint64_t board_perft(Board* board, uint32_t num_plies)
{
    return board_perft_recurse(board, 0, num_plies);
}

/* Debug */

#define PIECES_STRING ("PpNnBbRrQqKk")

void board_debug(Board* board)
{
    char res[SIZEOF_DEBUG_BOARD];
    memset(res, ' ', SIZEOF_DEBUG_BOARD - 1);
    res[SIZEOF_DEBUG_BOARD - 1] = '\0';

    for (int i = 0; i < 10; i++) {
        res[i * 22 + 21] = '\n';
    }

    const char *files = "abcdefgh";

    for(int i = 0; i < 8; i++) 
    {
        res[3 + i * 2] = files[i];
        res[9 * 22 + 3 + i * 2] = files[i];
    }

    for(int line = 1; line <= 8; line++) 
    {
        int rank = 9 - line;
        char rank_char = '0' + rank;
        int line_start = line * 22;
        res[line_start] = rank_char;
        res[line_start + 20] = rank_char;
    }

    for(int64_t jj = 7; jj >= 0; jj--) 
    {
        int line = 1 + (7 - jj);
        int line_start = line * 22;

        for(uint64_t ii = 0; ii < 8; ii++) 
        {
            for(uint64_t k = 0; k < 12; k++)
            {
                const uint64_t b = ((uint64_t*)board)[k];

                if(BOARD_HAS_BIT_FROM_FILE_RANK(b, ii, jj))
                {
                    int pos = line_start + 3 + ii * 2;
                    res[pos] = PIECES_STRING[k];
                }
            }
        }
    }

    printf(res);
    printf("\n");

    printf("State:\n");

    printf(" - White King side castling: %s\n", 
           board->state & BoardState_WhiteKingSideCastleAvailable ? "Available" : "Unavailable");
    printf(" - White Queen side castling: %s\n", 
           board->state & BoardState_WhiteQueenSideCastleAvailable ? "Available" : "Unavailable");

    printf(" - Black King side castling: %s\n", 
           board->state & BoardState_BlackKingSideCastleAvailable ? "Available" : "Unavailable");
    printf(" - Black Queen side castling: %s\n", 
           board->state & BoardState_BlackQueenSideCastleAvailable ? "Available" : "Unavailable");

    printf(" - %s to play\n",
           board->state & BoardState_WhiteToPlay ? "White" : "Black");

    printf("\n");
}

void board_debug_move_masks(Board* board)
{
    const uint64_t white_pieces = board->pawns[0] |
                                  board->knights[0] |
                                  board->bishops[0] |
                                  board->rooks[0] |
                                  board->queens[0] |
                                  board->kings[0];

    const uint64_t black_pieces = board->pawns[1] |
                                  board->knights[1] |
                                  board->bishops[1] |
                                  board->rooks[1] |
                                  board->queens[1] |
                                  board->kings[1];

    const uint64_t pieces[2] = {
        white_pieces,
        black_pieces,
    };

    const uint64_t inv_pieces[2] = {
        ~white_pieces,
        ~black_pieces,
    };

    const uint64_t all_pieces = white_pieces | black_pieces;
    const uint64_t inv_all_pieces = inv_pieces[0] | inv_pieces[1];

    for(size_t i = 0; i < 6; i++)
    {
        for(size_t j = 0; j < 2; j++)
        {
            const uint32_t b_index = (i + j) * 2 - j;

            uint64_t b = ((int64_t*)board)[b_index];

            const uint8_t piece = i;
            const uint8_t side = j;

            const uint64_t num_pieces = popcount_u64(b);

            for(size_t k = 0; k < num_pieces; k++)
            {
                const uint32_t piece_index = ctz_u64(b);
                const uint64_t move_mask = __move_gen_funcs[piece](piece_index,
                                                                    side,
                                                                    white_pieces,
                                                                    black_pieces); 

                const uint32_t file = BOARD_FILE_FROM_POS(piece_index);
                const uint32_t rank = BOARD_RANK_FROM_POS(piece_index);

                printf("Move mask for %s %s (%c%d, %llu):\n", 
                       SIDE_AS_STRING(side),
                       PIECE_AS_STRING(piece),
                       'a' + file,
                       1 + rank,
                       move_mask);

                printf("\n");

                board_debug_mask(move_mask);

                UNSET_BIT(b, BIT64(piece_index));
            }
        }
    }
}

void board_debug_move_mask(uint64_t mask, uint64_t square)
{
    char res[SIZEOF_DEBUG_BOARD];
    memset(res, ' ', SIZEOF_DEBUG_BOARD - 1);
    res[SIZEOF_DEBUG_BOARD - 1] = '\0';

    for (int i = 0; i < 10; i++) {
        res[i * 22 + 21] = '\n';
    }

    const char *files = "abcdefgh";

    for(int i = 0; i < 8; i++) 
    {
        res[3 + i * 2] = files[i];
        res[9 * 22 + 3 + i * 2] = files[i];
    }

    for(int line = 1; line <= 8; line++) 
    {
        int rank = 9 - line;
        char rank_char = '0' + rank;
        int line_start = line * 22;
        res[line_start] = rank_char;
        res[line_start + 20] = rank_char;
    }

    for(int64_t jj = 7; jj >= 0; jj--) 
    {
        int line = 1 + (7 - jj);
        int line_start = line * 22;

        for(uint64_t ii = 0; ii < 8; ii++) 
        {
            if(BOARD_HAS_BIT_FROM_FILE_RANK(mask, ii, jj))
            {
                int pos = line_start + 3 + ii * 2;
                res[pos] = 'o';
            }
        }
    }

    const uint32_t file = BOARD_FILE_FROM_POS(square);
    const uint32_t rank = BOARD_RANK_FROM_POS(square);

    const int pos = (8 - rank) * 22 + 3 + file * 2;
    res[pos] = 'x';

    printf("\n%s\n", res);
}