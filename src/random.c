#include "cchess/random.h"

#include <threads.h>

static thread_local uint64_t wyhash64_x; 

uint64_t random_wyhash_64() 
{
    wyhash64_x += 0x60bee2bee120fc15;
    __uint128_t tmp;
    tmp = (__uint128_t) wyhash64_x * 0xa3b195354a39b70d;
    uint64_t m1 = (tmp >> 64) ^ tmp;
    tmp = (__uint128_t)m1 * 0x1b03738712fad5c9;
    uint64_t m2 = (tmp >> 64) ^ tmp;
    return m2;
}

static thread_local __uint128_t g_lehmer64_state;

uint64_t random_lehmer_64() 
{
    g_lehmer64_state *= 0xda942042e4dd58b5;
    return g_lehmer64_state >> 64;
}