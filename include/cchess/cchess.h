#pragma once

#if !defined(__CCHESS)
#define __CCHESS

#if defined(_MSC_VER)
#define CCHESS_MSVC
#pragma warning(disable:4711) /* function selected for automatic inline expansion */
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#elif defined(__GNUC__)
#define CCHESS_GCC
#elif defined(__clang__)
#define CCHESS_CLANG
#endif /* defined(_MSC_VER) */

#if !defined(CCHESS_VERSION_STR)
#define CCHESS_VERSION_STR "Debug"
#endif /* !defined(CCHESS_VERSION_STR) */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stddef.h>

#if INTPTR_MAX == INT64_MAX || defined(__x86_64__)
#define CCHESS_X64
#define CCHESS_SIZEOF_PTR 8
#elif INTPTR_MAX == INT32_MAX
#define CCHESS_X86
#define CCHESS_SIZEOF_PTR 4
#endif /* INTPTR_MAX == INT64_MAX || defined(__x86_64__) */

#if defined(_WIN32)
#define CCHESS_WIN
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif /* !defined(WIN32_LEAN_AND_MEAN) */
#if defined(CCHESS_X64)
#define CCHESS_PLATFORM_STR "WIN64"
#else
#define CCHESS_PLATFORM_STR "WIN32"
#endif /* defined(CCHESS_x64) */
#elif defined(__linux__)
#define CCHESS_LINUX
#if defined(CCHESS_X64)
#define CCHESS_PLATFORM_STR "LINUX64"
#else
#define CCHESS_PLATFORM_STR "LINUX32"
#endif /* defined(CCHESS_X64) */
#endif /* defined(_WIN32) */

#if defined(CCHESS_WIN)
#if defined(CCHESS_MSVC)
#define CCHESS_EXPORT __declspec(dllexport)
#define CCHESS_IMPORT __declspec(dllimport)
#elif defined(CCHESS_GCC) || defined(CCHESS_CLANG)
#define CCHESS_EXPORT __attribute__((dllexport))
#define CCHESS_IMPORT __attribute__((dllimport))
#endif /* defined(CCHESS_MSVC) */
#elif defined(CCHESS_LINUX)
#define CCHESS_EXPORT __attribute__((visibility("default")))
#define CCHESS_IMPORT
#endif /* defined(CCHESS_WIN) */

#if defined(CCHESS_MSVC)
#define CCHESS_FORCE_INLINE __forceinline
#define CCHESS_g_ENTRY
#define CCHESS_g_EXIT
#elif defined(CCHESS_GCC)
#define CCHESS_FORCE_INLINE inline __attribute__((always_inline)) 
#define CCHESS_g_ENTRY __attribute__((constructor))
#define CCHESS_g_EXIT __attribute__((destructor))
#elif defined(CCHESS_CLANG)
#define CCHESS_FORCE_INLINE __attribute__((always_inline))
#define CCHESS_g_ENTRY __attribute__((constructor))
#define CCHESS_g_EXIT __attribute__((destructor))
#endif /* defined(CCHESS_MSVC) */

#if defined(CCHESS_BUILD_SHARED)
#define CCHESS_API CCHESS_EXPORT
#else
#define CCHESS_API CCHESS_IMPORT
#endif /* defined(CCHESS_BUILD_SHARED) */

#if defined __cplusplus
#define CCHESS_CPP_ENTER extern "C" {
#define CCHESS_CPP_END }
#else
#define CCHESS_CPP_ENTER
#define CCHESS_CPP_END
#endif /* DEFINED __cplusplus */

#if !defined NULL
#define NULL (void*)0
#endif /* !defined NULL */

#if defined(CCHESS_WIN)
#define CCHESS_FUNCTION __FUNCTION__
#elif defined(CCHESS_GCC) || defined(CCHESS_CLANG)
#define CCHESS_FUNCTION __PRETTY_FUNCTION__
#endif /* CCHESS_WIN */

#define CCHESS_STATIC_FUNCTION static

#define CONCAT_(prefix, suffix)     prefix##suffix
#define CONCAT(prefix, suffix)      CONCAT_(prefix, suffix)

#define CCHESS_ASSERT(expr) assert(expr)
#define STATIC_ASSERT(expr)                             \
    struct CONCAT(__outscope_assert_, __COUNTER__)      \
    {                                                   \
        char                                            \
        outscope_assert                                 \
        [2*(expr)-1];                                   \
                                                        \
    } CONCAT(__outscope_assert_, __COUNTER__)

#define CCHESS_NOT_IMPLEMENTED "Function "CCHESS_FUNCTION" not implemented" 

#if defined(CCHESS_MSVC)
#define CCHESS_PACKED_STRUCT(__struct__) __pragma(pack(push, 1)) __struct__ __pragma(pack(pop))
#elif defined(CCHESS_GCC) || defined(CCHESS_CLANG)
#define CCHESS_PACKED_STRUCT(__struct__) __struct__ __attribute__((__packed__))
#else
#define CCHESS_PACKED_STRUCT(__struct__) __struct__
#endif /* defined(CCHESS_MSVC) */

#if defined(CCHESS_MSVC)
#define dump_struct(s) 
#elif defined(CCHESS_CLANG)
#define dump_struct(s) __builtin_dump_struct(s, printf)
#elif defined(CCHESS_GCC)
#define dump_struct(s) 
#endif /* defined(CCHESS_MSVC) */

#if defined(CCHESS_WIN)
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif /* defined(CCHESS_WIN) */

#if defined(DEBUG_BUILD)
#define CCHESS_DEBUG 1
#else
#define CCHESS_DEBUG 0
#endif /* defined(DEBUG_BUILD) */

#define CCHESS_ATEXIT_REGISTER(func, exit)                                          \
        int res_##func = atexit(func);                                              \
        if(res_##func != 0)                                                         \
        {                                                                           \
            fprintf(stderr, "Cannot register function \""#func"\" in atexit");      \
            if(exit) return 1;                                                      \
        }                                                                           \

#endif /* !defined(__CCHESS) */