#pragma once

#if !defined(__MOVE)
#define __MOVE

#include "cchess/cchess.h"
#include "cchess/board_macros.h"

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