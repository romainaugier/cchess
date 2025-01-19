#include "cchess/magic_bitboards.h"
#include "cchess/move.h"
#include "cchess/bit_utils.h"

#include "libromano/random.h"
#include "libromano/memory.h"

// #define ROMANO_ENABLE_PROFILING
#include "libromano/profiling.h"

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string.h>

#define DEBUG_MAGIC_NUMBERS_ITERATIONS 1

CCHESS_FORCE_INLINE uint64_t random_sparse_64()
{
    return random_lehmer_64() & random_lehmer_64() & random_lehmer_64();
}

uint64_t generate_occupancy(uint32_t index, uint32_t bits_in_mask, uint64_t mask) 
{
    uint64_t occupancy = 0ULL;

    for(uint32_t count = 0; count < bits_in_mask; count++) 
    {
        uint32_t bit_position = ctz_u64(mask);

        if(index & (1 << count))
        {
            occupancy |= (1ULL << bit_position);
        }

        mask &= (mask - 1);
    }

    return occupancy;
}

uint64_t get_rook_attacks(const uint32_t square, const uint64_t block) 
{
    uint64_t attacks = 0ULL;
    const uint32_t rank = square / 8;
    const uint32_t file = square % 8;
    
    for(uint32_t r = rank + 1; r <= 7; r++) 
    {
        attacks |= (1ULL << (r * 8 + file));

        if(block & (1ULL << (r * 8 + file))) 
        {
            break;
        }
    }

    for(uint32_t r = rank - 1; r >= 0; r--) 
    {
        attacks |= (1ULL << (r * 8 + file));

        if(block & (1ULL << (r * 8 + file))) 
        {
            break;
        }
    }

    for (uint32_t f = file + 1; f <= 7; f++) 
    {
        attacks |= (1ULL << (rank * 8 + f));

        if(block & (1ULL << (rank * 8 + f))) 
        {
            break;
        }
    }

    for (uint32_t f = file - 1; f >= 0; f--) 
    {
        attacks |= (1ULL << (rank * 8 + f));

        if(block & (1ULL << (rank * 8 + f))) 
        {
            break;
        }
    }
    
    return attacks;
}

uint64_t get_bishop_attacks(const uint32_t square, const uint64_t block) 
{
    uint64_t attacks = 0ULL;
    const uint32_t rank = square / 8;
    const uint32_t file = square % 8;
    
    for(uint32_t r = rank + 1, f = file + 1; r <= 7 && f <= 7; r++, f++) 
    {
        attacks |= (1ULL << (r * 8 + f));

        if(block & (1ULL << (r * 8 + f))) 
        {
            break;
        }
    }

    for(uint32_t r = rank + 1, f = file - 1; r <= 7 && f >= 0; r++, f--) 
    {
        attacks |= (1ULL << (r * 8 + f));

        if(block & (1ULL << (r * 8 + f))) 
        {
            break;
        }
    }

    for(uint32_t r = rank - 1, f = file + 1; r >= 0 && f <= 7; r--, f++) 
    {
        attacks |= (1ULL << (r * 8 + f));

        if(block & (1ULL << (r * 8 + f))) 
        {
            break;
        }
    }

    for(uint32_t r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--) 
    {
        attacks |= (1ULL << (r * 8 + f));

        if(block & (1ULL << (r * 8 + f))) 
        {
            break;
        }
    }
    
    return attacks;
}

uint64_t magic_number_find_best(const uint64_t mask,
                                const uint32_t square, 
                                const uint32_t relevant_bits,
                                const uint32_t num_iterations,
                                const bool is_bishop) 
{
#if DEBUG_MAGIC_NUMBERS_ITERATIONS
    printf("Searching magic number for square: %d\n", square);
#endif /* DEBUG_MAGIC_NUMBERS_ITERATIONS */

    uint64_t attacks[4096];
    uint64_t occupancies[4096];
    uint64_t table[4096];

    uint32_t n = popcount_u64(mask);
    uint32_t num_occupancies = 1 << n;

#if DEBUG_MAGIC_NUMBERS_ITERATIONS
    printf("Generating %d occupancies\n", num_occupancies);
#endif /* DEBUG_MAGIC_NUMBERS_ITERATIONS */

    for(uint32_t i = 0; i < num_occupancies; i++)
    {
        occupancies[i] = generate_occupancy(i, n, mask);

        attacks[i] = is_bishop ? get_bishop_attacks(square, occupancies[i]) :
                                 get_rook_attacks(square, occupancies[i]);
    }

#if DEBUG_MAGIC_NUMBERS_ITERATIONS
    printf("Generated %d occupancies\n", num_occupancies);
#endif /* DEBUG_MAGIC_NUMBERS_ITERATIONS */

    for(uint32_t attempt = 0; attempt < num_iterations; ++attempt) 
    {
        const uint64_t magic = random_sparse_64();

        if(popcount_u64((mask * magic) & 0xFF00000000000000ULL) < 6) 
        {
            continue;
        }

        memset(table, 0, sizeof(table));

        bool failed = false;

        for(uint32_t i = 0; !failed && i < num_occupancies; i++)
        {
            uint32_t j = (uint32_t)((occupancies[i] * magic) >> (64 - relevant_bits));

            if(table[j] == 0)
            {
                table[j] = attacks[i];
            }
            else if(table[j] != attacks[i])
            {
                failed = true;
            }
        }

#if DEBUG_MAGIC_NUMBERS_ITERATIONS
        printf("\r[%u/%u] Square: %u | 0x%llX                              ",
               attempt,
               num_iterations,
               square,
               magic);
#endif /* DEBUG_MAGIC_NUMBERS_ITERATIONS */

        if(!failed)
        {
            printf("Square: %d | 0x%llX", square, magic);
            return magic;
        }
    }

    printf("Square: %d failed", square);

    return 0;
}

uint64_t magic_number_find_best_rook(const uint32_t square,
                                     const uint32_t relevant_bits,
                                     const uint32_t num_iterations)
{
    const uint64_t mask = move_gen_rook_mask_special(square);

    const uint64_t magic = magic_number_find_best(mask, 
                                                  square,
                                                  relevant_bits,
                                                  num_iterations,
                                                  false);

    // printf("Perfect magic number for square %u for rook: 0x%llx\n", square, magic);

    return magic;
}

uint64_t magic_number_find_best_bishop(const uint32_t square,
                                       const uint32_t relevant_bits,
                                       const uint32_t num_iterations)
{
    const uint64_t mask = move_gen_bishop_mask_special(square);

    const uint64_t magic = magic_number_find_best(mask,
                                                  square,
                                                  relevant_bits,
                                                  num_iterations,
                                                  true);

    // printf("Perfect magic number for square %u for bishop: 0x%llx\n", square, magic);

    return magic;
}

void magic_number_find_all()
{
    // #pragma omp parallel for
    for(uint32_t i = 0; i < 64; i++)
    {
        SCOPED_PROFILE_START_MILLISECONDS(magic_number_find);

        uint64_t magic_rook = magic_number_find_best_rook(i,
                                                          ROOK_RELEVANT_BITS,
                                                          10000000);

        SCOPED_PROFILE_END_MILLISECONDS(magic_number_find);
    }

    // #pragma omp parallel for
    for(uint32_t i = 0; i < 64; i++)
    {
        SCOPED_PROFILE_START_MILLISECONDS(magic_number_find);

        uint64_t magic_bishop = magic_number_find_best_bishop(i,
                                                              BISHOP_RELEVANT_BITS,
                                                              10000000);

        SCOPED_PROFILE_END_MILLISECONDS(magic_number_find);
    }
}


static uint64_t _magic_numbers_rook[64] = {
    0x880025020824000,
    0x40100020004000,
    0xc008004010024c1,
    0x4200040208144009,
    0x204040020020800,
    0x220120200010080,
    0x8080020800a84104,
    0x48000b080004100,
    0x4022081020040040,
    0x80042128011000,
    0xa200005000900,
    0x200408002200,
    0x4040220024080006,
    0x101000100020400,
    0x6000922000044,
    0x800080002100,
    0x1100108000844000,
    0x400403100a102000,
    0x8200040420090800,
    0x14a022082006400,
    0x3000450080040010,
    0xc00010008008440,
    0x6000010008420004,
    0x20081000080,
    0x9000400080208008,
    0x920004040000c,
    0x8c0102010040,
    0x420050001040,
    0x20800240010040,
    0x2c000082104002,
    0x20010a038040,
    0x100050c01010002,
    0x2000608000410040,
    0x40001902840030,
    0x200e880018104004,
    0x100220010100004,
    0x8200109081080,
    0x2000040028044200,
    0x600080810200,
    0x440044086001401,
    0x8000200100840800,
    0x1040004a008400,
    0x80080042000,
    0xa1000202200,
    0x8020081001001,
    0x2000040010222108,
    0x800890400820,
    0x40084020011008,
    0x1001028000481100,
    0x440000810002050,
    0x6080481000044040,
    0x202000204008288,
    0x202201008010200,
    0x8020004000080,
    0x219000810004090,
    0x101000200420820,
    0x2001002048108001,
    0x1080110028804001,
    0x5052000110243,
    0x120004084102,
    0x4a001140202,
    0x880090408100082,
    0x4082000401,
    0x43084024102,
};

static uint16_t _magic_offsets_rook[64] = {
    4096,
    2048,
    2048,
    2048,
    2048,
    2048,
    2048,
    4096,
    2048,
    1024,
    1024,
    1024,
    1024,
    1024,
    1024,
    2048,
    2048,
    1024,
    1024,
    1024,
    1024,
    1024,
    1024,
    2048,
    2048,
    1024,
    1024,
    1024,
    1024,
    1024,
    1024,
    2048,
    2048,
    1024,
    1024,
    1024,
    1024,
    1024,
    1024,
    2048,
    2048,
    1024,
    1024,
    1024,
    1024,
    1024,
    1024,
    2048,
    2048,
    1024,
    1024,
    1024,
    1024,
    1024,
    1024,
    2048,
    4096,
    2048,
    2048,
    2048,
    2048,
    2048,
    2048,
    4096,
};

static uint64_t _magic_numbers_bishop[64] = {
    0x2120080080880820,
    0x800400200a100a00,
    0x4010031000000,
    0x4000240060400001,
    0x241010240000,
    0x2000802088000400,
    0x20888400400000,
    0x5004840804042,
    0x50010042064020,
    0x2008081000a10,
    0x2000045100a0400,
    0x28a00400000,
    0x100405004004,
    0x200011020200008,
    0x100808044400,
    0x20008200110404,
    0x11000502028300,
    0x81010881020f014,
    0x140188000c0008,
    0x2000300900200,
    0x820800500230000,
    0x1000080010421008,
    0x40000a001001,
    0x8108c04024001880,
    0xc00044808800,
    0x1810040000401040,
    0x20d00202008004,
    0x2080004004218,
    0x9010000104000,
    0x208008002c02042,
    0x108000406400,
    0x2402100700c00980,
    0x123000c00400,
    0x2000140440008440,
    0x401000004420,
    0x8000200800050050,
    0x440400004100,
    0x4008080024001000,
    0x301000444000cc00,
    0x1154090010200,
    0x2300044008220,
    0x4005101000800200,
    0x4004040008800100,
    0x1044000020,
    0x48081500200,
    0x42004011000020,
    0x220021000008,
    0x802006004040,
    0x2400042023400000,
    0x4208200400448a20,
    0x200020404040000,
    0x1120180000,
    0x8000000401001000,
    0x8190084000,
    0x8082800281000,
    0x20011044a0000,
    0x200802120101488,
    0x4020000200120100,
    0x10000044020800,
    0x100040600,
    0x20220080440,
    0x4000000220040020,
    0x288210020010,
    0x510900082001020,
};

static uint16_t _magic_offsets_bishop[64] = {
    64,
    32,
    32,
    32,
    32,
    32,
    32,
    64,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    128,
    128,
    128,
    128,
    32,
    32,
    32,
    32,
    128,
    512,
    512,
    128,
    32,
    32,
    32,
    32,
    128,
    512,
    512,
    128,
    32,
    32,
    32,
    32,
    128,
    128,
    128,
    128,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    64,
    32,
    32,
    32,
    32,
    32,
    32,
    64,
};

static uint64_t* _magic_bitboards_rook = NULL;
static uint64_t* _magic_bitboards_bishop = NULL;

void magic_bitboards_init()
{
    return;
}

void magic_bitboards_destroy()
{
    if(_magic_bitboards_rook != NULL)
    {
        free(_magic_bitboards_rook);
        _magic_bitboards_rook = NULL;
    }

    if(_magic_bitboards_bishop != NULL)
    {
        free(_magic_bitboards_bishop);
        _magic_bitboards_bishop = NULL;
    }
}

uint64_t magic_bitboards_get_rook_index(uint32_t square, uint64_t blockers)
{
    return 0ULL;
}

uint64_t magic_bitboards_get_bishop_index(uint32_t square, uint64_t blockers)
{
    return 0ULL;
}

uint64_t magic_bitboards_get_rook(uint32_t square, uint64_t blockers)
{
    return 0ULL;
}

uint64_t magic_bitboards_get_bishop(uint32_t square, uint64_t blockers)
{
    return 0ULL;
}