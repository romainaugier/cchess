#pragma once

#if !defined(__MOVE)
#define __MOVE

#include "cchess/cchess.h"
#include "cchess/board_macros.h"

CCHESS_FORCE_INLINE uint64_t move_gen_pawn_mask(const uint32_t index, const uint32_t side)
{
    const uint8_t rank = BOARD_RANK_FROM_POS(index);
    const uint8_t file = BOARD_FILE_FROM_POS(index);

    uint64_t mask = 0;

    if(side == 0)
    {
        mask |= RANKX(rank + 1);
        mask |= rank == 1 ? RANKX(rank + 2) : 0;
    }
    else
    {
        mask |= RANKX(rank - 1);
        mask |= rank == 6 ? RANKX(rank - 2) : 0;
    }

    mask &= FILEX(file);

    return mask;
}

CCHESS_FORCE_INLINE uint64_t move_gen_knight_mask(const uint32_t index, const uint32_t side)
{
    const uint8_t rank = BOARD_RANK_FROM_POS(index);
    const uint8_t file = BOARD_FILE_FROM_POS(index);

    const uint64_t knight_pos = (uint64_t)index;
    const uint64_t knight_mask = BOARD_BIT_FROM_FILE_AND_RANK(file, rank);

    uint64_t mask = 0;

    mask |= (knight_mask << 6) & FILEX(file - 2) & RANKX(rank + 1); 
    mask |= (knight_mask << 10) & FILEX(file + 2) & RANKX(rank + 1); 
    mask |= (knight_mask << 15) & FILEX(file - 1) & RANKX(rank + 2); 
    mask |= (knight_mask << 17) & FILEX(file + 1) & RANKX(rank + 2); 

    mask |= (knight_mask >> 6) & FILEX(file + 2) & RANKX(rank - 1); 
    mask |= (knight_mask >> 10) & FILEX(file - 2) & RANKX(rank - 1); 
    mask |= (knight_mask >> 15) & FILEX(file + 1) & RANKX(rank - 2); 
    mask |= (knight_mask >> 17) & FILEX(file - 1) & RANKX(rank - 2); 

    return mask;
}

CCHESS_FORCE_INLINE uint64_t move_gen_bishop_mask(const uint32_t index, const uint32_t side)
{
    const uint8_t rank = BOARD_RANK_FROM_POS(index);
    const uint8_t file = BOARD_FILE_FROM_POS(index);

    const uint64_t bishop_pos = (uint64_t)index;
    const uint64_t bishop_mask = BOARD_BIT_FROM_FILE_AND_RANK(file, rank);

    uint64_t mask = 0;

    mask |= BOARD_GET_MAIN_DIAGONAL(file, rank);
    mask |= BOARD_GET_ANTI_DIAGONAL(file, rank);

    mask &= ~bishop_mask;

    return mask;
}

#define BORDERS (FILEA | FILEH | RANK1 | RANK8)
#define INV_BORDERS ~BORDERS

CCHESS_FORCE_INLINE uint64_t move_gen_bishop_mask_special(const uint32_t index)
{
    const uint32_t rank = BOARD_RANK_FROM_POS(index);
    const uint32_t file = BOARD_FILE_FROM_POS(index);

    uint64_t mask = 0;

    for(int r = rank + 1, f = file + 1; r < 7 && f < 7; r++, f++) 
    {
        mask |= (1ULL << (r * 8 + f));
    }

    for(int r = rank + 1, f = file - 1; r < 7 && f > 0; r++, f--)
    {
        mask |= (1ULL << (r * 8 + f));
    }

    for(int r = rank - 1, f = file + 1; r > 0 && f < 7; r--, f++)
    {
        mask |= (1ULL << (r * 8 + f));
    }

    for(int r = rank - 1, f = file - 1; r > 0 && f > 0; r--, f--)
    {
        mask |= (1ULL << (r * 8 + f));
    }

    return mask;
}

CCHESS_FORCE_INLINE uint64_t move_gen_rook_mask(const uint32_t index, const uint32_t side)
{
    const uint8_t rank = BOARD_RANK_FROM_POS(index);
    const uint8_t file = BOARD_FILE_FROM_POS(index);

    const uint64_t rook_pos = (uint64_t)index;
    const uint64_t rook_mask = BOARD_BIT_FROM_FILE_AND_RANK(file, rank);

    uint64_t mask = 0;

    mask |= RANKX(rank);
    mask |= FILEX(file);

    mask &= ~rook_mask;

    return mask;
}

CCHESS_FORCE_INLINE uint64_t move_gen_rook_mask_special(const uint32_t index) 
{
    const uint32_t rank = BOARD_RANK_FROM_POS(index);
    const uint32_t file = BOARD_FILE_FROM_POS(index);

    uint64_t mask = 0ULL;
    
    for(int r = rank + 1; r < 7; r++)
    {
        mask |= (1ULL << (r * 8 + file));
    }

    for(int r = rank - 1; r > 0; r--) 
    {
        mask |= (1ULL << (r * 8 + file));
    }

    for(int f = file + 1; f < 7; f++) 
    {
        mask |= (1ULL << (rank * 8 + f));
    }

    for(int f = file - 1; f > 0; f--) 
    {
        mask |= (1ULL << (rank * 8 + f));
    }

    return mask;
}

CCHESS_FORCE_INLINE uint64_t move_gen_queen_mask(const uint32_t index, const uint32_t side)
{
    const uint8_t rank = BOARD_RANK_FROM_POS(index);
    const uint8_t file = BOARD_FILE_FROM_POS(index);

    const uint64_t queen_pos = (uint64_t)index;
    const uint64_t queen_mask = BOARD_BIT_FROM_FILE_AND_RANK(file, rank);

    uint64_t mask = 0;

    mask |= RANKX(rank);
    mask |= FILEX(file);
    mask |= BOARD_GET_MAIN_DIAGONAL(file, rank);
    mask |= BOARD_GET_ANTI_DIAGONAL(file, rank);

    mask &= ~queen_mask;

    return mask;
}

CCHESS_FORCE_INLINE uint64_t move_gen_king_mask(const uint32_t index, const uint32_t side)
{
    const uint8_t rank = BOARD_RANK_FROM_POS(index);
    const uint8_t file = BOARD_FILE_FROM_POS(index);

    const uint64_t king_pos = (uint64_t)index;
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

#endif /* !defined(__MOVE) */