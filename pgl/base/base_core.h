#ifndef BASE_CORE_H
#define BASE_CORE_H

// outside headers

#include <math.h>
#include <stdarg.h>
#include <stddef.h> //size_t ptrdiff_t offsetof NULL
#include <stdint.h> //int_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //mem stuff

// Compiler cracking

#if defined(__clang__)
#  define COMPILER_CLANG 1
#elif defined(_MSC_VER)
#  define COMPILER_MSVC 1
#elif defined(__GNUC__) || defined(__GNUG__)
#  define COMPILER_GCC 1
#endif

// Zero undefined

#if !defined(COMPILER_MSVC)
#  define COMPILER_MSVC 0
#endif
#if !defined(COMPILER_GCC)
#  define COMPILER_GCC 0
#endif
#if !defined(COMPILER_CLANG)
#  define COMPILER_CLANG 0
#endif

// Platforms.

#if defined(_WIN32)
#  define PLATFORM_WINDOWS 1
#elif defined(__wasm__)
#  define PLATFORM_WEB 1
#elif defined(__APPLE__) && defined(__MACH__)
#  include <TargetConditionals.h>
#  if TARGET_OS_IPHONE
#    define PLATFORM_IOS 1
#  else
#    define PLATFORM_MACOS 1
#  endif
#elif defined(ANDROID) || defined(__ANDROID__)
#  define PLATFORM_ANDROID 1
#elif defined(linux) || defined(__linux) || defined(__linux__)
#  define PLATFORM_LINUX 1
#endif

// Zero undefined

#if !defined(PLATFORM_WINDOWS)
#  define PLATFORM_WINDOWS 0
#endif
#if !defined(PLATFORM_WEB)
#  define PLATFORM_WEB 0
#endif
#if !defined(PLATFORM_LINUX)
#  define PLATFORM_LINUX 0
#endif
#if !defined(PLATFORM_MACOS)
#  define PLATFORM_MACOS 0
#endif
#if !defined(PLATFORM_IOS)
#  define PLATFORM_IOS 0
#endif
#if !defined(PLATFORM_ANDROID)
#  define PLATFORM_ANDROID 0
#endif

// Bounds

#if !defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L)
#  error "Unsupported C standard: C99 or newer required"
#endif

// Align

#if COMPILER_MSVC
#  define align_of(T) __alignof(T)
#elif COMPILER_CLANG
#  define align_of(T) __alignof(T)
#elif COMPILER_GCC
#  define align_of(T) __alignof__(T)
#else
#  error align_of not defined for this compiler.
#endif

#if COMPILER_MSVC
#  define align_as(x) __declspec(align(x))
#elif COMPILER_CLANG || COMPILER_GCC
#  define align_as(x) __attribute__((aligned(x)))
#else
#  error align_as not defined for this compiler.
#endif

// Asserts

#if COMPILER_MSVC
#  define trap() __debugbreak()
#elif COMPILER_CLANG || COMPILER_GCC
#  define trap() __builtin_trap()
#else
#  error Unknown trap intrinsic for this compiler.
#endif

#define assert_always(x) do{if(!(x)) {trap();}}while(0)
#if BUILD_DEBUG
#  define assert(x) assert_always(x)
#else
#  define assert(x) (void)(x)
#endif

#define static_assert(C, ID) GLOBAL u8 glue(ID, __LINE__)[(C)?1:-1]

// rename

#define size_of(t) sizeof(t)
#define offset_of(t) offsetof(t)
#define mem_move(dst, src, size) memmove((dst), (src), (size))
#define mem_set(dst, byte, size) memset((dst), (byte), (size))
#define mem_cmp(a, b, size)      memcmp((a), (b), (size))
#define mem_strlen(ptr)          strlen(ptr)

// Version

#define version(major, minor, patch) (u64)((((u64)(major) & 0xffff) << 32) | ((((u64)(minor) & 0xffff) << 16)) | ((((u64)(patch) & 0xffff) << 0)))
#define major_from_version(version) (((version) & 0xffff00000000ull) >> 32)
#define minor_from_version(version) (((version) & 0x0000ffff0000ull) >> 16)
#define patch_from_version(version) (((version) & 0x00000000ffffull) >> 0)

// Macro utils

#define stringify_(S) #S
#define stringify(S) stringify_(S)

#define glue_(A, B) A##B
#define glue(A, B) glue_(A,B)

#define max(A, B)       (((A) > (B)) ? (A) : (B))
#define min(A, B)       (((A) < (B)) ? (A) : (B))
#define clamp_top(A, X) min(A, X)
#define clamp_bot(X, B) max(X, B)
#define clamp(A, X, B)  (((X) < (A)) ? (A) : ((X) > (B)) ? (B) : (X))

#define array_count(a) (size_of(a) / size_of((a)[0]))

#define ceil_int_div(a, b) (((a) + (b) - 1)/(b))

#define swap(T, a, b) do{T t__ = a; a = b; b = t__;}while(0)

// units

#define KB(n)  (((u64)(n)) << 10)
#define MB(n)  (((u64)(n)) << 20)
#define GB(n)  (((u64)(n)) << 30)
#define TB(n)  (((u64)(n)) << 40)
#define Thousand(n)   ((n)*1000)
#define Million(n)    ((n)*1000000)
#define Billion(n)    ((n)*1000000000)

// Codebase Keywords

#define FUNCTION          static
#define GLOBAL            static
#define LOCAL_PERSISTENT  static
#define EXTERNAL          extern

#if COMPILER_MSVC
#  define THREAD_PERSISTENT __declspec(thread)
#elif COMPILER_CLANG || COMPILER_GCC
#  define THREAD_PERSISTENT __thread
#else
#  error THREAD_PERSISTENT not defined for this compiler.
#endif

#define true          1
#define false         0

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef float    f32;
typedef double   f64;
// typedef size_t    usize;
// typedef ptrdiff_t isize;
// typedef uintptr_t uptr;
typedef _Bool b8;
// char (for poking around ptr memory)
// void (perfect best type ever)

static_assert(sizeof(f32) == 4, f32_not_4_bytes);
static_assert(sizeof(f64) == 8, f64_not_8_bytes);
static_assert(sizeof(b8) == 1, bool_not_1_byte);

#endif
