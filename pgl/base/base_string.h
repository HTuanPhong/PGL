#ifndef BASE_STRING_H
#define BASE_STRING_H

// this is utf8 only

typedef struct Str8 { // used for operations
  u8 *value;
  u64 size;
} Str8; // READ-ONLY

typedef struct Str16 { // for interacting with other api
  u16 *value;
  u64  size;
} Str16; // READ-ONLY

typedef struct Str8Buf { // used for building
  Str8 str;
  u64  capacity;
} Str8Buf; // OWNING + MUTABLE

typedef struct UnicodeDecode {
  u32 codepoint;
  u32 increment;
} UnicodeDecode;

#define FMT_I8      "%hhd"
#define FMT_I16     "%hd"
#define FMT_I32     "%d"
#define FMT_I64     "%lld"
#define FMT_U8      "%hhu"
#define FMT_U16     "%hu"
#define FMT_U32     "%u"
#define FMT_U64     "%llu"
#define FMT_F32     "%f"
#define FMT_F64     "%f"
#define FMT_B8      "%d"
#define FMT_STR8    "%.*s"
#define FMT_PTR     "%p"
#define FMT_HEX_U8  "%hhx"
#define FMT_HEX_U16 "%hx"
#define FMT_HEX_U32 "%x"
#define FMT_HEX_U64 "%llx"

// string 8

// printf("My String: " FMT_STR8 "\n", str8_varg(my_str));
#define str8_varg(S)(int)((S).size), ((S).value)

#define str8_from_lit(lit) (Str8){ (u8 *)(lit), sizeof(lit) - 1 }

// from cstring

FUNCTION Str8  str8_from_cstr(char *cstr);
FUNCTION Str16 str16_from_cstr(u16 *cstr);

// compare

FUNCTION i32 str8_cmp_ignore_case(Str8 a, Str8 b);
FUNCTION i32 str8_cmp(Str8 a, Str8 b);

// find

FUNCTION u64 str8_find(Str8 s, Str8 needle);  // size if not found
FUNCTION u64 str8_rfind(Str8 s, Str8 needle); // size if not found

// slice

FUNCTION Str8 str8_slice(Str8 s, u64 begin, u64 end);

// number conversion

FUNCTION b8 str8_to_i32(Str8 s, i32 *out);
FUNCTION b8 str8_to_i64(Str8 s, i64 *out);
FUNCTION b8 str8_to_f32(Str8 s, f32 *out);
FUNCTION b8 str8_to_f64(Str8 s, f64 *out);

// hash

FUNCTION u64 str8_hash(Str8 s);

// string buffer

FUNCTION void str8buf_reserve(Scratch *scratch, Str8Buf *strbuf, u64 demand);
FUNCTION void str8buf_append(Scratch *scratch, Str8Buf *strbuf, Str8 s);
FUNCTION void str8buf_appendv(Scratch *scratch, Str8Buf *strbuf, char *fmt, va_list args);
FUNCTION void str8buf_appendf(Scratch *scratch, Str8Buf *strbuf, char *fmt, ...);
FUNCTION void str8buf_clear(Str8Buf *buf);

// UTF conversion

FUNCTION UnicodeDecode utf8_decode(u8 *str, u32 cap);
FUNCTION UnicodeDecode utf16_decode(u16 *str, u32 cap);
FUNCTION u32           utf8_encode(u8 *dst, u32 codepoint);
FUNCTION u32           utf16_encode(u16 *dst, u32 codepoint);

FUNCTION Str8  str8_from_str16(Scratch *scratch, Str16 input);
FUNCTION Str16 str16_from_str8(Scratch *scratch, Str8 input);

// path util

FUNCTION Str8 str8_get_left_of_last_slash(Str8 str);
FUNCTION Str8 str8_get_right_of_last_slash(Str8 str);

// string interning?

#endif // BASE_STRING_H
