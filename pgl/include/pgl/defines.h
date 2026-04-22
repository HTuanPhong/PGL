#include <assert.h> //assert static_assert
#include <stddef.h> //size_t ptrdiff_t offsetof NULL
#include <stdint.h> //int_t
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
#elif defined(__EMSCRIPTEN__)
#  define PLATFORM_EMSCRIPTEN 1
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
#if !defined(PLATFORM_EMSCRIPTEN)
#  define PLATFORM_EMSCRIPTEN 0
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

#if !defined(__STDC_VERSION__) || (__STDC_VERSION__ < 201112L)
#  error "Unsupported C standard: C11 or newer required"
#endif

// Mem

#define mem_move(dst, src, size) memmove((dst), (src), (size))
#define mem_set(dst, byte, size) memset((dst), (byte), (size))
#define mem_cmp(a, b, size)      memcmp((a), (b), (size))

// Align

#define alignof(T) _Alignof(T)
#define alignas(T) _Alignas(T)

// Codebase Keywords

// #define internal      static
// #define global        static
// #define local_persist static
#define thread_local  _Thread_local
#define true          1
#define false         0

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;
typedef float     f32;
typedef double    f64;
typedef size_t    usize;
typedef ptrdiff_t isize;
typedef uintptr_t uptr;
typedef _Bool     b8;
// char (for poking around ptr memory)
// void (perfect best type ever)

static_assert(sizeof(f32) == 4, "f32 not 4 bytes");
static_assert(sizeof(f64) == 8, "f64 not 8 bytes");
static_assert(sizeof(b8) == 1, "bool not 1 byte");

// common

#define max(A, B)       (((A) > (B)) ? (A) : (B))
#define min(A, B)       (((A) < (B)) ? (A) : (B))
#define clamp_top(A, X) min(A, X)
#define clamp_bot(X, B) max(X, B)
#define clamp(A, X, B)  (((X) < (A)) ? (A) : ((X) > (B)) ? (B) : (X))
