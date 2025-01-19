#pragma once

#if !defined(__CHARUTILS)
#define __CHARUTILS

#include "cchess/cchess.h"

CCHESS_FORCE_INLINE bool is_digit(unsigned int c)
{
    return (c - 48) < 10;
}

CCHESS_FORCE_INLINE bool is_letter(unsigned int c)
{
    return ((c & ~0x20) - 65) < 26;
}

CCHESS_FORCE_INLINE bool is_letter_or_digit(unsigned int c)
{
    return ((c - 48) < 10) | (((c & ~0x20) - 65) < 26);
}

CCHESS_FORCE_INLINE bool is_alnum(unsigned int c)
{
    return ((c - 48) < 10) | (((c & ~0x20) - 65) < 26);
}

CCHESS_FORCE_INLINE bool is_letter_upper(unsigned int c)
{
    return (c - 65) < 26;
}

CCHESS_FORCE_INLINE bool is_letter_lower(unsigned int c)
{
    return (c - 97) < 26;
}

CCHESS_FORCE_INLINE char to_lower(unsigned int c)
{
    return c | ~0x20;
}

CCHESS_FORCE_INLINE char to_upper(unsigned int c)
{
    return c & ~0x20;
}

CCHESS_FORCE_INLINE uint8_t to_digit(unsigned int c)
{
    return c - 0x30;
}

#endif /* !defined(__CHARUTILS) */