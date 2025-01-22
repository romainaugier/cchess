#pragma once

#if !defined(__BOARD_MACROS)
#define __BOARD_MACROS

#include "cchess/cchess.h"

CCHESS_API const char* __piece_as_string(const uint32_t piece);

#define PIECE_AS_STRING(p) (__piece_as_string(p))

#define SIDE_AS_STRING(side) ((side) == PIECE_WHITE ? "White" : "Black")

#define RANK8 (uint64_t)0xFF00000000000000
#define RANK7 (uint64_t)0x00FF000000000000
#define RANK6 (uint64_t)0x0000FF0000000000
#define RANK5 (uint64_t)0x000000FF00000000
#define RANK4 (uint64_t)0x00000000FF000000
#define RANK3 (uint64_t)0x0000000000FF0000
#define RANK2 (uint64_t)0x000000000000FF00
#define RANK1 (uint64_t)0x00000000000000FF

#define RANKX(x) (((x) < 0) ? 0 : (((x) > 7) ? 0 : (uint64_t)0xFF << ((x) * 8)))

#define FILEA (uint64_t)0x0101010101010101
#define FILEB (uint64_t)0x0202020202020202
#define FILEC (uint64_t)0x0404040404040404
#define FILED (uint64_t)0x0808080808080808
#define FILEE (uint64_t)0x1010101010101010
#define FILEF (uint64_t)0x2020202020202020
#define FILEG (uint64_t)0x4040404040404040
#define FILEH (uint64_t)0x8080808080808080

CCHESS_API uint64_t __files(const uint32_t file);

#define FILEX(x) (((x) < 0) ? 0 : (((x) > 7) ? 0 : __files(x)))

#define INITIAL_WHITE_KNIGHTS (uint64_t)0x0000000000000042
#define INITIAL_BLACK_KNIGHTS (uint64_t)0x4200000000000000
#define INITIAL_WHITE_BISHOPS (uint64_t)0x0000000000000024
#define INITIAL_BLACK_BISHOPS (uint64_t)0x2400000000000000
#define INITIAL_WHITE_ROOKS (uint64_t)0x0000000000000081
#define INITIAL_BLACK_ROOKS (uint64_t)0x8100000000000000
#define INITIAL_WHITE_QUEEN (uint64_t)0x0000000000000008
#define INITIAL_BLACK_QUEEN (uint64_t)0x0800000000000000
#define INITIAL_WHITE_KING (uint64_t)0x0000000000000010
#define INITIAL_BLACK_KING (uint64_t)0x1000000000000000

#define BOARD_BIT(i, j) (BIT64(((i) + (j) * 8)))
#define BOARD_BIT_FROM_SQUARE(s) (1ULL << (s))
#define BOARD_HAS_BIT(b, s) (HAS_BIT((b), BIT64((s))))
#define BOARD_SET_BIT(b, s) (SET_BIT((b), BIT64((s))))
#define BOARD_UNSET_BIT(b, s) (UNSET_BIT((b), BIT64((s))))
#define BOARD_HAS_BIT_FROM_FILE_RANK(b, i, j) (HAS_BIT(b, BOARD_BIT(i, j)))
#define BOARD_SET_BIT_FROM_FILE_RANK(b, i, j) (SET_BIT(b, BOARD_BIT(i, j)))
#define BOARD_UNSET_BIT_FROM_FILE_RANK(b, i, j) (UNSET_BIT(b, BOARD_BIT(i, j)))

#define BOARD_FILE_FROM_POS(p) (p % 8)
#define BOARD_RANK_FROM_POS(p) (p / 8)

#define BOARD_POS_FROM_FILE_AND_RANK(f, r) ((f) + (r) * 8)
#define BOARD_BIT_FROM_FILE_AND_RANK(f, r) (((uint64_t)1 << BOARD_POS_FROM_FILE_AND_RANK(f, r)))
#define BOARD_BITS_FROM_FILE_AND_RANK(f, r, b) (((b) << BOARD_POS_FROM_FILE_AND_RANK(f, r)))

CCHESS_API uint64_t __main_diagonals(const uint32_t file, const uint32_t rank);

CCHESS_API uint64_t __anti_diagonals(const uint32_t file, const uint32_t rank);

#define BOARD_GET_MAIN_DIAGONAL(f, r) (__main_diagonals(f, r))
#define BOARD_GET_ANTI_DIAGONAL(f, r) (__anti_diagonals(f, r))

#define SIZEOF_DEBUG_BOARD (10 * 22 + 1)

CCHESS_API void board_debug_mask(const uint64_t mask);

#endif /* !defined(__BOARD_MACROS) */