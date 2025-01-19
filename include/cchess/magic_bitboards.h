#pragma once

#if !defined(__MAGIC_BITBOARDS)
#define __MAGIC_BITBOARDS

#include "cchess/cchess.h"

#define ROOK_RELEVANT_BITS 12
#define BISHOP_RELEVANT_BITS 9

CCHESS_API uint64_t magic_number_find_best_rook(const uint32_t square, 
                                                const uint32_t relevant_bits,
                                                const uint32_t num_iterations);

CCHESS_API uint64_t magic_number_find_best_bishop(const uint32_t square, 
                                                  const uint32_t relevant_bits,
                                                  const uint32_t num_iterations);

CCHESS_API void magic_number_find_all();

CCHESS_API void magic_bitboards_init();

CCHESS_API void magic_bitboards_destroy();

CCHESS_API uint64_t magic_bitboards_get_rook_index(uint32_t square, uint64_t blockers);

CCHESS_API uint64_t magic_bitboards_get_bishop_index(uint32_t square, uint64_t blockers);

CCHESS_API uint64_t magic_bitboards_get_rook(uint32_t square, uint64_t blockers);

CCHESS_API uint64_t magic_bitboards_get_bishop(uint32_t square, uint64_t blockers);

#endif /* !defined(__MAGIC_BITBOARDS) */