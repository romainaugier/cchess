#pragma once

#if !defined(__BITUTILS)
#define __BITUTILS

#include "cchess/cchess.h"

#define BIT64(bit) ((uint64_t)1 << (bit))

#define BIT32(bit) ((uint32_t)1 << (bit))

#define HAS_BIT(flag, bit) ((flag) & (bit))

#define SET_BIT(flag, bit) ((flag) |= (bit))

#define UNSET_BIT(flag, bit) ((flag) &= ~(bit))


#if defined(CCHESS_WIN)
#include <intrin.h>
#endif /* defined(CCHESS_WIN) */

CCHESS_FORCE_INLINE uint64_t popcount_u32(const uint32_t x)
{
#if defined(CCHESS_MSVC)
    return __popcnt(x);
#elif defined(CCHESS_GCC) || defined(CCHESS_CLANG)
    return __builtin_popcountl(x);
#else
    /* A good compiler will replace it with popcnt instruction if available */
    uint32_t v = x;
    v = v - ((v >> 1) & (uint32_t)~(uint32_t)0/3);
    v = (v & (uint32_t)~(uint32_t)0/15*3) + ((v >> 2) & (uint32_t)~(uint32_t)0/15*3);
    v = (v + (v >> 4)) & (uint32_t)~(uint32_t)0/255*15;
    return (uint32_t)(v * ((uint32_t)~(uint32_t)0/255)) >> (sizeof(uint32_t) - 1) * 8;
#endif /* defined(CCHESS_WIN) */
}

CCHESS_FORCE_INLINE uint64_t popcount_u64(const uint64_t x)
{
#if defined(CCHESS_MSVC)
    return __popcnt64(x);
#elif defined(CCHESS_GCC) || defined(CCHESS_CLANG)
    return __builtin_popcountll(x);
#else
    /* A good compiler will replace it with popcnt instruction if available */
    uint64_t v = x;
    v = v - ((v >> 1) & (uint64_t)~(uint64_t)0/3);
    v = (v & (uint64_t)~(uint64_t)0/15*3) + ((v >> 2) & (uint64_t)~(uint64_t)0/15*3);
    v = (v + (v >> 4)) & (uint64_t)~(uint64_t)0/255*15;
    return (uint64_t)(v * ((uint64_t)~(uint64_t)0/255)) >> (sizeof(uint64_t) - 1) * 8;
#endif /* defined(CCHESS_WIN) */
}

CCHESS_FORCE_INLINE uint32_t clz_u64(const uint64_t x)
{
#if defined(CCHESS_MSVC)
    unsigned long trailing_zero = 0;

    if(_BitScanReverse64(&trailing_zero, x))
    {
        return 63 - trailing_zero;
    }

    return 32;
#elif defined(CCHESS_GCC)
    return __builtin_clzll(x);
#endif /* defined(CCHESS_MSVC) */
}

CCHESS_FORCE_INLINE uint32_t ctz_u64(const uint64_t x)
{
#if defined(CCHESS_MSVC)
    unsigned long trailing_zero = 0;

    if(_BitScanForward64(&trailing_zero, x))
    {
        return trailing_zero;
    }

    return 63;
#elif defined(CCHESS_GCC)
    return __builtin_ctzll(x);
#endif /* defined(CCHESS_MSVC) */
}

CCHESS_FORCE_INLINE uint32_t pext_u32(const uint32_t x, const uint32_t y)
{
    return _pext_u32(x, y);
}

CCHESS_FORCE_INLINE uint64_t pext_u64(const uint64_t x, const uint64_t y)
{
    return _pext_u64(x, y);
}

CCHESS_FORCE_INLINE uint8_t abs_u8(const int8_t x)
{
    const uint8_t mask = x >> 7;
    return (uint8_t)((mask ^ x) - mask);
}

CCHESS_FORCE_INLINE uint16_t abs_u16(const int16_t x)
{
    const uint16_t mask = x >> 15;
    return (uint16_t)((mask ^ x) - mask);
}

CCHESS_FORCE_INLINE uint32_t abs_u32(const int32_t x)
{
    const uint32_t mask = x >> 31;
    return (uint32_t)((mask ^ x) - mask);
}

CCHESS_FORCE_INLINE uint64_t abs_u64(const int64_t x)
{
    const uint64_t mask = x >> 63;
    return (uint64_t)((mask ^ x) - mask);
}

CCHESS_FORCE_INLINE uint64_t lsb_u64(const uint64_t x)
{
    return x & -x;
}

CCHESS_FORCE_INLINE uint64_t clsb_u64(const uint64_t x)
{
    return x & (x - 1);
}

#endif /* !defined(__BITUTILS) */