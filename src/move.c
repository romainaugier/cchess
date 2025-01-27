#include "cchess/move.h"
#include "cchess/board_macros.h"

#include "libromano/memory.h"
#include "libromano/bit.h"

#include <string.h>

CCHESS_FORCE_INLINE uint64_t move_gen_pawn_mask(const uint32_t square, const uint32_t side)
{
    const uint64_t rank = BOARD_RANK_FROM_POS(square);
    const uint64_t file = BOARD_FILE_FROM_POS(square);

    uint64_t mask = 0ULL;

    if(side == 0)
    {
        mask |= RANKX(rank + 1ULL);
        mask |= rank == 1ULL ? RANKX(rank + 2ULL) : 0ULL;
    }
    else
    {
        mask |= RANKX(rank - 1ULL);
        mask |= rank == 6ULL ? RANKX(rank - 2ULL) : 0ULL;
    }

    mask &= FILEX(file);

    return mask;
}

CCHESS_FORCE_INLINE uint64_t move_gen_knight_mask(const uint32_t square)
{
    const uint64_t rank = BOARD_RANK_FROM_POS(square);
    const uint64_t file = BOARD_FILE_FROM_POS(square);

    const uint64_t knight_pos = (uint64_t)square;
    const uint64_t knight_mask = BOARD_BIT_FROM_FILE_AND_RANK(file, rank);

    uint64_t mask = 0ULL;

    mask |= (knight_mask << 6ULL) & FILEX(file - 2ULL) & RANKX(rank + 1ULL); 
    mask |= (knight_mask << 10ULL) & FILEX(file + 2ULL) & RANKX(rank + 1ULL); 
    mask |= (knight_mask << 15ULL) & FILEX(file - 1ULL) & RANKX(rank + 2ULL); 
    mask |= (knight_mask << 17ULL) & FILEX(file + 1ULL) & RANKX(rank + 2ULL); 

    mask |= (knight_mask >> 6ULL) & FILEX(file + 2ULL) & RANKX(rank - 1ULL); 
    mask |= (knight_mask >> 10ULL) & FILEX(file - 2ULL) & RANKX(rank - 1ULL); 
    mask |= (knight_mask >> 15ULL) & FILEX(file + 1ULL) & RANKX(rank - 2ULL); 
    mask |= (knight_mask >> 17ULL) & FILEX(file - 1ULL) & RANKX(rank - 2ULL); 

    return mask;
}

CCHESS_FORCE_INLINE uint64_t move_gen_bishop_mask(const uint32_t square)
{
    const uint64_t rank = BOARD_RANK_FROM_POS(square);
    const uint64_t file = BOARD_FILE_FROM_POS(square);

    uint64_t mask = 0ULL;

    for (int r = rank + 1, f = file + 1; r < 7 && f < 7; r++, f++)
        mask |= 1ULL << (r * 8 + f);

    for (int r = rank - 1, f = file - 1; r > 0 && f > 0; r--, f--)
        mask |= 1ULL << (r * 8 + f);

    for (int r = rank + 1, f = file - 1; r < 7 && f > 0; r++, f--)
        mask |= 1ULL << (r * 8 + f);

    for (int r = rank - 1, f = file + 1; r > 0 && f < 7; r--, f++)
        mask |= 1ULL << (r * 8 + f);

    return mask;
}

CCHESS_FORCE_INLINE uint64_t move_gen_bishop_mask_with_blockers(const uint32_t square,
                                                                const uint64_t blockers)
{
    const uint64_t rank = BOARD_RANK_FROM_POS(square);
    const uint64_t file = BOARD_FILE_FROM_POS(square);

    uint64_t mask = 0ULL;

    for (int r = rank + 1, f = file + 1; r < 8 && f < 8; r++, f++)
    {
        const uint64_t pos = 1ULL << (r * 8 + f);
        mask |= pos;
        if (blockers & pos) break;
    }

    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--)
    {
        const uint64_t pos = 1ULL << (r * 8 + f);
        mask |= pos;
        if (blockers & pos) break;
    }

    for (int r = rank + 1, f = file - 1; r < 8 && f >= 0; r++, f--)
    {
        const uint64_t pos = 1ULL << (r * 8 + f);
        mask |= pos;
        if (blockers & pos) break;
    }

    for (int r = rank - 1, f = file + 1; r >= 0 && f < 8; r--, f++)
    {
        const uint64_t pos = 1ULL << (r * 8 + f);
        mask |= pos;
        if (blockers & pos) break;
    }

    return mask;
}

CCHESS_FORCE_INLINE uint64_t move_gen_rook_mask(const uint32_t square)
{
    const int32_t rank = BOARD_RANK_FROM_POS(square);
    const int32_t file = BOARD_FILE_FROM_POS(square);

    uint64_t mask = 0ULL;
    
    for(int32_t f = file + 1; f < 7; f++)
        mask |= 1ULL << (rank * 8 + f);

    for(int32_t f = file - 1; f > 0; f--)
        mask |= 1ULL << (rank * 8 + f);
        
    for(int32_t r = rank + 1; r < 7; r++)
        mask |= 1ULL << (r * 8 + file);

    for(int32_t r = rank - 1; r > 0; r--)
        mask |= 1ULL << (r * 8 + file);
        
    return mask;
}

CCHESS_FORCE_INLINE uint64_t move_gen_rook_mask_with_blockers(const uint32_t square, 
                                                              const uint64_t blockers)
{
    const int32_t rank = BOARD_RANK_FROM_POS(square);
    const int32_t file = BOARD_FILE_FROM_POS(square);

    uint64_t mask = 0ULL;
    
    for(int32_t f = file + 1; f < 8; f++)
    {
        const uint64_t pos = 1ULL << (rank * 8 + f);
        mask |= pos;

        if(blockers & pos) break;
    }

    for(int32_t f = file - 1; f >= 0; f--)
    {
        const uint64_t pos = 1ULL << (rank * 8 + f);
        mask |= pos;

        if(blockers & pos) break;
    }
        
    for(int32_t r = rank + 1; r < 8; r++)
    {
        const uint64_t pos = 1ULL << (r * 8 + file);
        mask |= pos;

        if(blockers & pos) break;
    }

    for(int32_t r = rank - 1; r >= 0; r--)
    {
        const uint64_t pos = 1ULL << (r * 8 + file);
        mask |= pos;

        if(blockers & pos) break;
    }
        
    return mask;
}

CCHESS_FORCE_INLINE uint64_t move_gen_queen_mask(const uint32_t square)
{
    return move_gen_rook_mask(square) | move_gen_bishop_mask(square);
}

CCHESS_FORCE_INLINE uint64_t move_gen_king_mask(const uint32_t square)
{
    const uint64_t rank = BOARD_RANK_FROM_POS(square);
    const uint64_t file = BOARD_FILE_FROM_POS(square);

    const uint64_t king_pos = (uint64_t)square;
    const uint64_t king_mask = BOARD_BIT_FROM_FILE_AND_RANK(file, rank);

    uint64_t mask = 0;

    mask |= king_mask << 1;
    mask |= king_mask >> 1;

    uint64_t mask_top = (mask | king_mask) << 8;
    uint64_t mask_bottom = (mask | king_mask) >> 8;

    mask |= mask_top;
    mask |= mask_bottom;

    return mask;
}

#define ROOK_RELEVANT_BITS (12)
#define BISHOP_RELEVANT_BITS (9)

#define ROOK_NUM_BLOCKERS (1 << ROOK_RELEVANT_BITS)
#define BISHOP_NUM_BLOCKERS (1 << BISHOP_RELEVANT_BITS)

#define ROOK_MOVES_LOOKUP_SIZE (64 + 64 * (1 << ROOK_RELEVANT_BITS))
#define BISHOP_MOVES_LOOKUP_SIZE (64 + 64 * (1 << BISHOP_RELEVANT_BITS))

static uint64_t* _rook_moves_lookup = NULL;
static uint64_t* _bishop_moves_lookup = NULL;

void gen_blockers(const uint64_t mask, uint64_t* blockers)
{
    uint64_t bits = popcount_u64(mask);
    uint64_t num_blockers = 1ULL << bits;

    for(uint32_t i = 0; i < num_blockers; i++)
    {
        blockers[i] = 0ULL;

        uint64_t temp_mask = mask;
        uint64_t bit_index = 0ULL;

        while(temp_mask)
        {
            uint64_t lsb = ctz_u64(temp_mask);

            if(i & (1ULL << bit_index))
            {
                blockers[i] |= (1ULL << lsb);
            }

            temp_mask = clsb_u64(temp_mask);
            bit_index++;
        }
    }
}

void move_gen_init(void)
{
    if(_rook_moves_lookup == NULL)
    {
        _rook_moves_lookup = (uint64_t*)calloc(ROOK_MOVES_LOOKUP_SIZE, sizeof(uint64_t));

        for(uint32_t i = 0; i < 64; i++)
        {
            const uint64_t mask = move_gen_rook_mask(i);

            _rook_moves_lookup[i] = mask;

            uint64_t blockers[ROOK_NUM_BLOCKERS];
            memset(blockers, 0, sizeof(blockers));
            gen_blockers(mask, blockers);

            for(uint32_t j = 0; j < ROOK_NUM_BLOCKERS; j++)
            {
                const uint32_t index = (uint32_t)pext_u64(blockers[j], mask);
                _rook_moves_lookup[64 + i * ROOK_NUM_BLOCKERS + index] = move_gen_rook_mask_with_blockers(i, blockers[j]);
            }
        }
    }

    if(_bishop_moves_lookup == NULL)
    {
        _bishop_moves_lookup = (uint64_t*)calloc(BISHOP_MOVES_LOOKUP_SIZE, sizeof(uint64_t));

        for(uint32_t i = 0; i < 64; i++)
        {
            const uint64_t mask = move_gen_bishop_mask(i);

            _bishop_moves_lookup[i] = mask;

            uint64_t blockers[BISHOP_NUM_BLOCKERS];
            memset(blockers, 0, sizeof(blockers));
            gen_blockers(mask, blockers);

            for(uint32_t j = 0; j < BISHOP_NUM_BLOCKERS; j++)
            {
                const uint32_t index = (uint32_t)pext_u64(blockers[j], mask);
                _bishop_moves_lookup[64 + i * BISHOP_NUM_BLOCKERS + index] = move_gen_bishop_mask_with_blockers(i, blockers[j]);
            }
        }
    }
}

void move_gen_destroy(void)
{
    if(_rook_moves_lookup != NULL)
    {
        free(_rook_moves_lookup);
    }

    if(_bishop_moves_lookup != NULL)
    {
        free(_bishop_moves_lookup);
    }
}

// uint64_t move_gen_pawn(const uint32_t square,
//                        const uint32_t side,
//                        const uint64_t blockers_white,
//                        const uint64_t blockers_black)
// {
//     uint64_t moves = move_gen_pawn_mask(square, side) & ~(blockers_white | blockers_black);

//     const uint64_t board_bit = BOARD_BIT_FROM_SQUARE(square);

//     if(side == 0)
//     {
//         moves |= (((board_bit << 7) | (board_bit << 9)) & blockers_black); 
//     }
//     else
//     {
//         moves |= (((board_bit >> 7) | (board_bit >> 9)) & blockers_white);        
//     }

//     return moves;
// }

uint64_t move_gen_pawn(const uint32_t square, 
                       const uint32_t side, 
                       const uint64_t whites, 
                       const uint64_t blacks) 
{
    uint64_t moves = 0;
    uint64_t all = whites | blacks;
    uint64_t pawn = 1ULL << square;
    uint64_t forward, double_forward, capture_left, capture_right;

    if (side == 0) 
    {
        forward = (pawn << 8) & ~all;
        double_forward = (forward && (square >= 8 && square <= 15)) ? (pawn << 16) & ~all : 0;
        capture_left = (pawn << 7) & blacks & ~RANK8 & ~FILEA;
        capture_right = (pawn << 9) & blacks & ~RANK8 & ~FILEH;
    } 
    else 
    {
        forward = (pawn >> 8) & ~all;
        double_forward = (forward && (square >= 48 && square <= 55)) ? (pawn >> 16) & ~all : 0;
        capture_left = (pawn >> 7) & whites & ~RANK1 & ~FILEH;
        capture_right = (pawn >> 9) & whites & ~RANK1 & ~FILEA;
    }

    moves = forward | double_forward | capture_left | capture_right;

    return moves;
}

uint64_t move_gen_knight(const uint32_t square,
                         const uint32_t side,
                         const uint64_t blockers_white,
                         const uint64_t blockers_black)
{
    return move_gen_knight_mask(square) & (side == 0 ? ~blockers_white : ~blockers_black);
}

uint64_t move_gen_bishop(const uint32_t square,
                         const uint32_t side,
                         const uint64_t blockers_white,
                         const uint64_t blockers_black)
{
    CCHESS_ASSERT(_bishop_moves_lookup != NULL && "_bishop_moves_lookup has not been initialized");

    const uint64_t mask = _bishop_moves_lookup[square];
    const uint64_t blockers_mask = mask & (blockers_white | blockers_black);
    const uint64_t index = pext_u64(blockers_mask, mask);

    return _bishop_moves_lookup[64 + square * BISHOP_NUM_BLOCKERS + index] & (side == 0 ? ~blockers_white : ~blockers_black) ;
}

uint64_t move_gen_rook(const uint32_t square,
                       const uint32_t side,
                       const uint64_t blockers_white,
                       const uint64_t blockers_black)
{
    CCHESS_ASSERT(_rook_moves_lookup != NULL && "_rook_moves_lookup has not been initialized");

    const uint64_t mask = _rook_moves_lookup[square];
    const uint64_t blockers_mask = mask & (blockers_white | blockers_black);
    const uint64_t index = pext_u64(blockers_mask, mask);

    return _rook_moves_lookup[64 + square * ROOK_NUM_BLOCKERS + index] & (side == 0 ? ~blockers_white : ~blockers_black);
}

uint64_t move_gen_queen(const uint32_t square,
                        const uint32_t side,
                        const uint64_t blockers_white,
                        const uint64_t blockers_black)
{
    CCHESS_ASSERT(_bishop_moves_lookup != NULL && "_bishop_moves_lookup has not been initialized");
    CCHESS_ASSERT(_rook_moves_lookup != NULL && "_rook_moves_lookup has not been initialized");

    return move_gen_bishop(square, side, blockers_white, blockers_black) | 
           move_gen_rook(square, side, blockers_white, blockers_black);
}

uint64_t move_gen_king(const uint32_t square,
                       const uint32_t side,
                       const uint64_t blockers_white,
                       const uint64_t blockers_black)
{
    return move_gen_king_mask(square) & ((side == 0) ? ~blockers_white : 
                                                       ~blockers_black);
}