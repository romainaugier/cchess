#include "cchess/magic_bitboards.h"
#include "cchess/move.h"
#include "cchess/random.h"

// #define CCHESS_ENABLE_PROFILING
#include "cchess/profiling.h"

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string.h>

#define DEBUG_MAGIC_NUMBERS_ITERATIONS 0

uint32_t magic_index(uint64_t blockers, uint64_t magic, uint32_t bits) 
{
    return (uint32_t)((blockers * magic) >> (64 - bits));
}

void generate_blocker_boards(uint64_t mask, 
                             uint64_t* blockers, 
                             uint64_t* count, 
                             uint32_t index,
                             uint64_t current) 
{
    if(mask == 0)
    {
        blockers[*count] = current;
        (*count)++;

        return;
    }

    const uint64_t lsb = mask & -mask;
    
    generate_blocker_boards(mask & (mask - 1), blockers, count, index + 1, current | lsb);
    
    generate_blocker_boards(mask & (mask - 1), blockers, count, index + 1, current);
}

void test_magic_number(const uint64_t magic,
                       const uint32_t bits, 
                       uint64_t* restrict table, 
                       uint64_t* restrict blockers, 
                       const uint64_t blocker_count,
                       uint8_t* restrict filled_indices,
                       uint32_t* collisions, 
                       uint32_t* unique_count) 
{
    memset(table, 0, sizeof(uint64_t) * (1 << bits));
    memset(filled_indices, 0, sizeof(uint8_t) * (1 << bits));

    uint32_t total_collisions = 0;
    uint32_t uniques_indices = 0;

    for(uint32_t i = 0; i < (uint32_t)blocker_count; ++i) 
    {
        uint32_t index = magic_index(blockers[i], magic, bits);
        
        if(table[index] == 0) 
        {
            table[index] = blockers[i];
            uniques_indices++;
        } 
        else if(table[index] != blockers[i])
        {
            total_collisions++;
        }
        
        filled_indices[index] = 1;
    }

    uint32_t filled_count = 0;

    for(uint32_t i = 0; i < (1 << bits); ++i) 
    {
        filled_count += filled_indices[i];
    }

    *collisions = total_collisions;
    *unique_count = filled_count;
}

uint64_t magic_number_find_best(const uint64_t mask,
                                const uint32_t square, 
                                const uint32_t relevant_bits,
                                const uint32_t num_iterations) 
{
    uint64_t best_magic = 0;
    uint32_t min_collisions = INT32_MAX;
    uint32_t max_uniques_indices = 0;

    uint64_t blocker_count = 0;
    uint64_t* blockers = (uint64_t*)calloc(0x4000, sizeof(uint64_t));

    generate_blocker_boards(mask, blockers, &blocker_count, 0, 0);

    uint64_t table[1 << relevant_bits];

    uint8_t* filled_indices = (uint8_t*)calloc(1 << relevant_bits, sizeof(uint32_t));

    for(uint32_t attempt = 0; attempt < num_iterations; ++attempt) 
    {
        uint64_t magic = random_wyhash_64() & 
                         random_wyhash_64() &
                         random_wyhash_64() & 
                         random_wyhash_64();

        uint32_t collisions = 0;
        uint32_t uniques_indices = 0;

        test_magic_number(magic,
                          relevant_bits,
                          table,
                          blockers,
                          blocker_count,
                          filled_indices,
                          &collisions,
                          &uniques_indices); 

        if(collisions < min_collisions ||
           (collisions == min_collisions && uniques_indices > max_uniques_indices)) 
        {
            best_magic = magic;
            min_collisions = collisions;
            max_uniques_indices = uniques_indices;
        }

#if DEBUG_MAGIC_NUMBERS_ITERATIONS
        printf("\r[%u/%u] Square: %u | 0x%llX %u %u                        ",
               attempt,
               num_iterations,
               square,
               best_magic,
               min_collisions,
               max_uniques_indices);
#endif /* DEBUG_MAGIC_NUMBERS_ITERATIONS */

        if(min_collisions == 0)
        {
            break;
        }
    }

#if DEBUG_MAGIC_NUMBERS_ITERATIONS
    printf("\n");
#endif /* DEBUG_MAGIC_NUMBERS_ITERATIONS */

    printf("Square: %u | 0x%llX %u %u\n",
           square,
           best_magic,
           min_collisions,
           max_uniques_indices);

    free(blockers);
    free(filled_indices);

    return best_magic;
}

uint64_t magic_number_find_best_rook(const uint32_t square,
                                     const uint32_t relevant_bits,
                                     const uint32_t num_iterations)
{
    const uint64_t mask = move_gen_rook_mask_special(square);

    const uint64_t magic = magic_number_find_best(mask, 
                                                  square,
                                                  relevant_bits,
                                                  num_iterations);

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
                                                  num_iterations);

    // printf("Perfect magic number for square %u for bishop: 0x%llx\n", square, magic);

    return magic;
}

void magic_number_find_all()
{
    #pragma omp parallel for
    for(uint32_t i = 0; i < 64; i++)
    {
        SCOPED_PROFILE_START_MILLISECONDS(magic_number_find);

        uint64_t magic_rook = magic_number_find_best_rook(i,
                                                          ROOK_RELEVANT_BITS,
                                                          10000000);

        SCOPED_PROFILE_END_MILLISECONDS(magic_number_find);
    }

    #pragma omp parallel for
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
    if(_magic_bitboards_rook == NULL)
    {
        printf("Initializing rooks magic bitboards\n");

        SCOPED_PROFILE_START_MILLISECONDS(magic_bitboards_rook_init);

        uint64_t total_size = 0;

        _magic_bitboards_rook = (uint64_t*)calloc(64 * 4096, sizeof(uint64_t));

        for(uint64_t square = 0; square < 64; square++)
        {
            const uint64_t mask = move_gen_rook_mask_special(square);

            uint64_t offset = 0;

            generate_blocker_boards(mask, 
                                    &_magic_bitboards_rook[total_size],
                                    &offset,
                                    0,
                                    0);

            total_size += (uint64_t)_magic_offsets_rook[square];
        }

        _magic_bitboards_rook = (uint64_t*)realloc(_magic_bitboards_rook,
                                                   total_size * sizeof(uint64_t));

        printf("Initialized rooks magic bitboards (%llu). Total lookup size: %.02f kbytes\n", 
               total_size,
               (double)(total_size * sizeof(uint64_t)) / 1000.0);

        SCOPED_PROFILE_END_MILLISECONDS(magic_bitboards_rook_init);
    }

    if(_magic_bitboards_bishop == NULL)
    {
        printf("Initializing bishops magic bitboards\n");

        SCOPED_PROFILE_START_MILLISECONDS(magic_bitboards_bishop_init);

        uint64_t total_size = 0;

        _magic_bitboards_bishop = (uint64_t*)calloc(64 * 512, sizeof(uint64_t));

        for(uint64_t square = 0; square < 64; square++)
        {
            const uint64_t mask = move_gen_bishop_mask_special(square);

            uint64_t offset = 0;

            generate_blocker_boards(mask, 
                                    &_magic_bitboards_bishop[total_size],
                                    &offset,
                                    0,
                                    0);

            total_size += (uint64_t)_magic_offsets_bishop[square];
        }

        _magic_bitboards_bishop = (uint64_t*)realloc(_magic_bitboards_bishop,
                                                   total_size * sizeof(uint64_t));

        printf("Initialized bishops magic bitboards (%llu). Total lookup size: %.02f kbytes\n", 
               total_size,
               (double)(total_size * sizeof(uint64_t)) / 1000.0);

        SCOPED_PROFILE_END_MILLISECONDS(magic_bitboards_bishop_init);
    }
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
    assert(_magic_bitboards_rook != NULL);

    const uint64_t offset = _magic_offsets_rook[square];
    const uint64_t magic = _magic_numbers_rook[square];

    return (uint64_t)magic_index(blockers, magic, ROOK_RELEVANT_BITS);
}

uint64_t magic_bitboards_get_bishop_index(uint32_t square, uint64_t blockers)
{
    assert(_magic_bitboards_bishop != NULL);

    const uint64_t offset = _magic_offsets_bishop[square];
    const uint64_t magic = _magic_numbers_bishop[square];

    return magic_index(blockers, magic, BISHOP_RELEVANT_BITS);
}

uint64_t magic_bitboards_get_rook(uint32_t square, uint64_t blockers)
{
    assert(_magic_bitboards_rook != NULL);

    const uint64_t offset = _magic_offsets_rook[square];
    const uint64_t magic = _magic_numbers_rook[square];

    const uint32_t index = magic_index(blockers, magic, ROOK_RELEVANT_BITS);

    return _magic_bitboards_rook[offset + index];
}

uint64_t magic_bitboards_get_bishop(uint32_t square, uint64_t blockers)
{
    assert(_magic_bitboards_bishop != NULL);

    const uint64_t offset = _magic_offsets_bishop[square];
    const uint64_t magic = _magic_numbers_bishop[square];

    const uint32_t index = magic_index(blockers, magic, BISHOP_RELEVANT_BITS);

    return _magic_bitboards_bishop[offset + index];
}