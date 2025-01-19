#include "cchess/board_macros.h"

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