#pragma once

#if !defined(__RANDOM)
#define __RANDOM

#include "cchess/cchess.h"

CCHESS_API uint64_t random_wyhash_64();

CCHESS_API uint64_t random_lehmer_64();

#endif /* !defined(__RANDOM) */
