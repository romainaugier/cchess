#include "cchess/board_macros.h"
#include "cchess/bit_utils.h"

#include <stdio.h>
#include <string.h>

static const char* const _piece_as_string[6] = {
    "Pawn",
    "Knight",
    "Bishop",
    "Rook",
    "Queen",
    "King",
};

const char* __piece_as_string(const uint32_t piece)
{
    return _piece_as_string[piece];
}

static const uint64_t _files[8] = {
    FILEA,
    FILEB,
    FILEC,
    FILED,
    FILEE,
    FILEF,
    FILEG,
    FILEH,
};

uint64_t __files(const uint32_t file)
{
    return _files[file];
}

static const uint64_t _main_diagonals[15] = {
    (uint64_t)0x1,                
    (uint64_t)0x102,              
    (uint64_t)0x10204,            
    (uint64_t)0x1020408,          
    (uint64_t)0x102040810,        
    (uint64_t)0x10204081020,      
    (uint64_t)0x1020408102040,    
    (uint64_t)0x102040810204080,  
    (uint64_t)0x204081020408000,  
    (uint64_t)0x408102040800000,  
    (uint64_t)0x810204080000000,  
    (uint64_t)0x1020408000000000, 
    (uint64_t)0x2040800000000000, 
    (uint64_t)0x4080000000000000, 
    (uint64_t)0x8000000000000000  
};

static const uint64_t _anti_diagonals[15] = {
    (uint64_t)0x80,                
    (uint64_t)0x8040,              
    (uint64_t)0x804020,            
    (uint64_t)0x80402010,          
    (uint64_t)0x8040201008,        
    (uint64_t)0x804020100804,      
    (uint64_t)0x80402010080402,    
    (uint64_t)0x8040201008040201,  
    (uint64_t)0x4020100804020100,  
    (uint64_t)0x2010080402010000,  
    (uint64_t)0x1008040201000000,  
    (uint64_t)0x804020100000000,   
    (uint64_t)0x402010000000000,   
    (uint64_t)0x201000000000000,   
    (uint64_t)0x100000000000000    
};

uint64_t __main_diagonals(const uint32_t file, const uint32_t rank)
{
    return _main_diagonals[file + rank];
}

uint64_t __anti_diagonals(const uint32_t file, const uint32_t rank)
{
    return _anti_diagonals[(7 - file) + rank];
}

void board_debug_mask(const uint64_t mask)
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

    printf("\n%s\n", res);
}
