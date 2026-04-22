#include <pgl/defines.h>
#include <pgl/memory.h>
#include <pgl/string.h>
#include <stdio.h>

StrView strview_from_cstr(CStr cstr) {
  StrView result = { cstr.value, cstr.size };
  return result;
}

StrView strview_from_strbuf(StrBuf strbuf) {
  StrView result = { strbuf.value, strbuf.size };
  return result;
}

// i32 strview_cmp_ignore_case(StrView a, StrView b);
// i32 strview_cmp(StrView a, StrView b);

// usize strview_find(StrView s, StrView needle);  // size if not found
// usize strview_rfind(StrView s, StrView needle); // size if not found

StrView strview_slice(StrView s, usize begin, usize end) {
  StrView result = { s.value + begin, end - begin };
  return result;
}

// bool strview_to_i32(StrView s, i32 *out);
// bool strview_to_i64(StrView s, i64 *out);
// bool strview_to_f32(StrView s, f32 *out);
// bool strview_to_f64(StrView s, f64 *out);

// u64 strview_hash(StrView s);

CStr cstr_from_strbuf(StrBuf strbu) {
  CStr result = { strbu.value, strbu.size };
  return result;
}

void strbuf_reserve(Scratch *scratch, StrBuf *strbuf, usize demand) {
  usize required = strbuf->size + demand;
  if (required <= strbuf->capacity) {
    return;
  }
  usize new_cap = strbuf->capacity * 2;
  if (required > new_cap) {
    new_cap = required;
  }
  if (new_cap == 0) {
    new_cap = 16;
  }
  char *new_buffer = scratch_push_array(scratch, new_cap + 1, char);
  if (strbuf->size) {
    mem_move(new_buffer, strbuf->value, strbuf->size);
  }
  strbuf->value = new_buffer;
  strbuf->capacity = new_cap;
  strbuf->value[strbuf->size] = 0;
}

void strbuf_append(Scratch *scratch, StrBuf *strbuf, StrView s) {
  strbuf_reserve(scratch, strbuf, s.size);
  mem_move(strbuf->value + strbuf->size, s.value, s.size);
  strbuf->size += s.size;
  strbuf->value[strbuf->size] = 0;
}

void strbuf_appendv(Scratch *scratch, StrBuf *strbuf, const char *fmt, va_list args) {
  va_list args2;
  va_copy(args2, args);

  i32 needed = vsnprintf(NULL, 0, fmt, args);
  if (needed > 0) {
    strbuf_reserve(scratch, strbuf, needed);
    vsnprintf(strbuf->value + strbuf->size,
              needed + 1,
              fmt,
              args2);
    strbuf->size += needed;
    strbuf->value[strbuf->size] = 0;
  }
  va_end(args2);
}

void strbuf_appendf(Scratch *scratch, StrBuf *strbuf, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  strbuf_appendv(scratch, strbuf, fmt, args);
  va_end(args);
}

void strbuf_clear(StrBuf *buf) {
  buf->size = 0;
  buf->value[0] = 0;
}
