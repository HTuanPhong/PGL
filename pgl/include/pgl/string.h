#ifndef PGL_STRING_H
#define PGL_STRING_H
#include <pgl/defines.h>
#include <pgl/memory.h>
#include <stdarg.h>

// UTF-8 is king latin is life

// the lower struct become the upper struct with out copy
// the upper struct become the lower struct with copy

typedef struct StrView { // commonly used
  const char *value;
  usize       size;
} StrView; // READ-ONLY

typedef struct CStr { // only use to talk with other C api
  const char *value;
  usize       size;
} CStr; // READ-ONLY + NULL TERMINATED

typedef struct StrBuf { // commonly used
  char *value;
  usize size;
  usize capacity;
} StrBuf; // OWNING + MUTABLE + NULL TERMINATED

// printf("My String: %.*s\n", std_varg(my_str));
#define str_varg(S) (int)((S).size), ((S).value)

#define strview_from_lit(lit) (StrView){ (lit), sizeof(lit) - 1 }
StrView strview_from_cstr(CStr cstr);
StrView strview_from_strbuf(StrBuf strbuf);
i32     strview_cmp_ignore_case(StrView a, StrView b);
i32     strview_cmp(StrView a, StrView b);

usize strview_find(StrView s, StrView needle);  // size if not found
usize strview_rfind(StrView s, StrView needle); // size if not found

StrView strview_slice(StrView s, usize begin, usize end);

b8 strview_to_i32(StrView s, i32 *out);
b8 strview_to_i64(StrView s, i64 *out);
b8 strview_to_f32(StrView s, f32 *out);
b8 strview_to_f64(StrView s, f64 *out);

u64 strview_hash(StrView s);

#define cstr_from_lit(lit) (CStr){ (lit), sizeof(lit) - 1 }
CStr cstr_from_strbuf(StrBuf strbu);

void strbuf_reserve(Scratch *scratch, StrBuf *strbuf, usize demand);
void strbuf_append(Scratch *scratch, StrBuf *strbuf, StrView s);
void strbuf_appendv(Scratch *scratch, StrBuf *strbuf, const char *fmt, va_list args);
void strbuf_appendf(Scratch *scratch, StrBuf *strbuf, const char *fmt, ...);
void strbuf_clear(StrBuf *buf);

// i8      %hhd
// i16     %hd
// i32     %d
// i64     %lld
// u8      %hhu
// u16     %hu
// u32     %u
// u64     %llu
// usize   %zu
// isize   %zd
// f32     %f
// f64     %f
// b8    %d
// char    %c
// char*   %s
// ptr     %p
// StrView %.*s
// hex:
// u8      %hhx
// u16     %hx
// u32     %x
// u64     %llx
// Minimum width:
// %8d     pad to 8 characters
// Zero padded:
// %08d
// Precision (floats):
// %.3f     3 decimal places
// Dynamic width:
// %*d      len, num
// %.*f     len, num

// path util

// string interning?

#endif // PGL_STRING_H
