#include "cchess/board.h"
#include "cchess/bit_utils.h"
#include "cchess/char_utils.h"
#include "cchess/move.h"

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

    b.state |= State_WhiteKingSideCastleAvailable;
    b.state |= State_WhiteQueenSideCastleAvailable;
    b.state |= State_BlackKingSideCastleAvailable;
    b.state |= State_BlackQueenSideCastleAvailable;
    b.state |= State_WhiteToPlay;

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

            BOARD_SET_BIT(b_ptr[b_index], current_file, current_rank);
        }
        else if(is_digit(*s))
        {
            const uint8_t d = to_digit(*s) - 1;

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
                        SET_BIT(b.state, State_WhiteToPlay);
                        break;
                    case 'K':
                        SET_BIT(b.state, State_WhiteKingSideCastleAvailable);
                        break;
                    case 'Q':
                        SET_BIT(b.state, State_WhiteQueenSideCastleAvailable);
                        break;
                    case 'k':
                        SET_BIT(b.state, State_BlackKingSideCastleAvailable);
                        break;
                    case 'q':
                        SET_BIT(b.state, State_BlackQueenSideCastleAvailable);
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

move_gen_func __move_gen_funcs[6] = {
    move_gen_pawn,
    move_gen_knight,
    move_gen_bishop,
    move_gen_rook,
    move_gen_queen,
    move_gen_king
};

#define MOVE_GET_PIECE(m) (((m) >> 12) & 0x7)
#define MOVE_GET_IS_CAPTURING(m) ((m) & (0x8000))
#define MOVE_GET_FROM_FILE(m) (((m) >> 9) & 0x7)
#define MOVE_GET_FROM_RANK(m) (((m) >> 6) & 0x7)
#define MOVE_GET_TO_FILE(m) (((m) >> 3) & 0x7)
#define MOVE_GET_TO_RANK(m) ((m) & 0x7)

#define MOVE_SET_PIECE(m, p) ((m) = (((m) & ~0x7000) | (p << 12)))
#define MOVE_SET_IS_CAPTURING(m, c) ((m) = (((m) & ~0x8000) | ((c) << 15)))
#define MOVE_SET_FROM_FILE(m, f) ((m) = (((m) & ~0xE00) | ((f) << 9)))
#define MOVE_SET_FROM_RANK(m, r) ((m) = (((m) & ~0x1C0) | ((r) << 6)))
#define MOVE_SET_TO_FILE(m, f) ((m) = (((m) & ~0x38) | ((f) << 3)))
#define MOVE_SET_TO_RANK(m, r) ((m) = (((m) & ~0x7) | (r)))

static uint64_t _mit_i = 0;
static uint64_t _mit_j = 0;
static uint64_t _mit_k = 0;
static uint64_t _mit_l = 0;

bool board_legal_moves_iterator(Board* board, Move* move, IteratorMoveType move_type)
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

    while(_mit_i < 6)
    {
        while(_mit_j < 2)
        {
            const uint32_t b_index = (_mit_i + _mit_j) * 2 - _mit_j;

            uint64_t b = ((int64_t*)board)[b_index];

            const uint8_t piece = _mit_i;
            const uint8_t side = _mit_j;

            MOVE_SET_PIECE(*move, piece);

            const uint64_t num_pieces = popcount_u64(b);

            while(_mit_k < num_pieces)
            {
                const uint32_t piece_index = ctz_u64(b);
                const uint64_t move_mask = __move_gen_funcs[piece](piece_index,
                                                                    side,
                                                                    white_pieces,
                                                                    black_pieces);

                const uint64_t num_moves = popcount_u64(move_mask);

                while(_mit_l < num_moves)
                {
                    MOVE_SET_IS_CAPTURING(*move, 1);

                    _mit_l++;

                    return true;
                }

                UNSET_BIT(b, BIT64(piece_index));

                _mit_l = 0;
                _mit_k++;
            }

            _mit_k = 0;
            _mit_j++;
        }

        _mit_j = 0;
        _mit_i++;
    }

    _mit_i = 0;

    return false;
}

#define SIZEOF_DEBUG_BOARD (64 + 8 + 1)

#define PIECES_STRING ("PpNnBbRrQqKk")

void board_debug(Board* board)
{
    char res[SIZEOF_DEBUG_BOARD];
    memset(res, ' ', SIZEOF_DEBUG_BOARD * sizeof(char));
    res[SIZEOF_DEBUG_BOARD - 1] = '\0';

    printf("cchess board:\n");

    for(int64_t j = 7; j >= 0; j--)
    {
        for(uint64_t i = 0; i < 8; i++)
        {
            for(uint64_t k = 0; k < 12; k++)
            {
                const uint64_t b = ((uint64_t*)board)[k];

                if(BOARD_HAS_BIT(b, i, j))
                {
                    res[i + (7 - j) * 9] = PIECES_STRING[k];
                }
            }
        }
    }

    for(uint64_t i = 0; i < 8; i++)
    {
        res[8 + i * 9] = '\n';
    }

    printf(res);
    printf("\n");

    printf("State:\n");

    printf(" - White King side castling: %s\n", 
           board->state & State_WhiteKingSideCastleAvailable ? "Available" : "Unavailable");
    printf(" - White Queen side castling: %s\n", 
           board->state & State_WhiteQueenSideCastleAvailable ? "Available" : "Unavailable");

    printf(" - Black King side castling: %s\n", 
           board->state & State_BlackKingSideCastleAvailable ? "Available" : "Unavailable");
    printf(" - Black Queen side castling: %s\n", 
           board->state & State_BlackQueenSideCastleAvailable ? "Available" : "Unavailable");

    printf(" - %s to play\n",
           board->state & State_WhiteToPlay ? "White" : "Black");

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

                char res[SIZEOF_DEBUG_BOARD];
                memset(res, ' ', SIZEOF_DEBUG_BOARD * sizeof(char));
                res[SIZEOF_DEBUG_BOARD - 1] = '\0';

                const uint32_t file = BOARD_FILE_FROM_POS(piece_index);
                const uint32_t rank = BOARD_RANK_FROM_POS(piece_index);

                printf("Move mask for %s %s (%c%d, %llu):\n", 
                       SIDE_AS_STRING(side),
                       PIECE_AS_STRING(piece),
                       'a' + file,
                       1 + rank,
                       move_mask);

                printf("\n");

                for(int64_t jj = 7; jj >= 0; jj--)
                {
                    for(uint64_t ii = 0; ii < 8; ii++)
                    {
                        if(BOARD_HAS_BIT(move_mask, ii, jj))
                        {
                            res[ii + (7 - jj) * 9] = 'o';
                        }
                    }
                }

                // res[(file) + (7 - rank) * 9] = 'x';

                for(uint64_t ii = 0; ii < 8; ii++)
                {
                    res[8 + ii * 9] = '\n';
                }

                printf(res);
                printf("\n");

                UNSET_BIT(b, BIT64(piece_index));
            }
        }
    }
}

void board_debug_move_mask(uint64_t mask, uint64_t square)
{
    char res[SIZEOF_DEBUG_BOARD];
    memset(res, ' ', SIZEOF_DEBUG_BOARD * sizeof(char));
    res[SIZEOF_DEBUG_BOARD - 1] = '\0';

    const uint32_t file = BOARD_FILE_FROM_POS(square);
    const uint32_t rank = BOARD_RANK_FROM_POS(square);

    printf("\n");

    for(int64_t jj = 7; jj >= 0; jj--)
    {
        for(uint64_t ii = 0; ii < 8; ii++)
        {
            if(BOARD_HAS_BIT(mask, ii, jj))
            {
                res[ii + (7 - jj) * 9] = 'o';
            }
        }
    }

    res[(file) + (7 - rank) * 9] = 'x';

    for(uint64_t ii = 0; ii < 8; ii++)
    {
        res[8 + ii * 9] = '\n';
    }

    printf(res);
    printf("\n");
}