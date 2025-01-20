#pragma once

#if !defined(__MOVE)
#define __MOVE

#include "cchess/cchess.h"

/* 
    A move is represented using 16 bits

    0  000  000000  000000
    c   P    from     to

    c stands for capturing or not
    P stands for piece
    from is the square from where we move
    to is the square where we move
*/

typedef uint16_t Move;

#define MOVE_GET_PIECE(m) (((m) >> 12) & 0x7)
#define MOVE_GET_IS_CAPTURING(m) ((m) & (0x8000))
#define MOVE_GET_FROM_SQUARE(m) (((m) >> 6) & 0x3F)
#define MOVE_GET_TO_SQUARE(m) ((m) & 0x3F)

#define MOVE_SET_PIECE(m, p) ((m) = (((m) & ~0x7000) | (p << 12)))
#define MOVE_SET_IS_CAPTURING(m, c) ((m) = (((m) & ~0x8000) | ((c) << 15)))
#define MOVE_SET_FROM_SQUARE(m, r) ((m) = (((m) & ~0xFC0) | ((r) << 6)))
#define MOVE_SET_TO_SQUARE(m, r) ((m) = (((m) & ~0x3F) | (r)))

typedef uint64_t (*move_gen_func)(const uint32_t, 
                                  const uint32_t,
                                  const uint64_t,
                                  const uint64_t);

CCHESS_API uint64_t move_gen_pawn(const uint32_t square,
                                  const uint32_t side,
                                  const uint64_t blockers_white,
                                  const uint64_t blockers_black);

CCHESS_API uint64_t move_gen_knight(const uint32_t square,
                                    const uint32_t side,
                                    const uint64_t blockers_white,
                                    const uint64_t blockers_black);

CCHESS_API uint64_t move_gen_bishop(const uint32_t square,
                                    const uint32_t side,
                                    const uint64_t blockers_white,
                                    const uint64_t blockers_black);

CCHESS_API uint64_t move_gen_rook(const uint32_t square,
                                  const uint32_t side,
                                  const uint64_t blockers_white,
                                  const uint64_t blockers_black);

CCHESS_API uint64_t move_gen_queen(const uint32_t square,
                                   const uint32_t side,
                                   const uint64_t blockers_white,
                                   const uint64_t blockers_black);

CCHESS_API uint64_t move_gen_king(const uint32_t square,
                                  const uint32_t side,
                                  const uint64_t blockers_white,
                                  const uint64_t blockers_black);

CCHESS_API void move_gen_init(void);

CCHESS_API void move_gen_destroy(void);

#endif /* !defined(__MOVE) */