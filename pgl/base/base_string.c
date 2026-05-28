FUNCTION Str8 str8_from_cstring(u8 *cstr) {
  return (Str8){
    cstr, mem_strlen((char *)cstr)
  };
}

FUNCTION Str16 str16_from_cstring(u16 *cstr) {
  return (Str16){
    cstr, mem_strlen((char *)cstr) / 2
  };
}

// i32 str8_cmp_ignore_case(Str8 a, Str8 b);
// i32 str8_cmp(Str8 a, Str8 b);

// u64 str8_find(Str8 s, Str8 needle);  // size if not found
// u64 str8_rfind(Str8 s, Str8 needle); // size if not found

FUNCTION Str8
str8_slice(Str8 s, u64 begin, u64 end) {
  Str8 result = { s.value + begin, end - begin };
  return result;
}

// b8 str8_to_i32(Str8 s, i32 *out);
// b8 str8_to_i64(Str8 s, i64 *out);
// b8 str8_to_f32(Str8 s, f32 *out);
// b8 str8_to_f64(Str8 s, f64 *out);

// u64 str8_hash(Str8 s);

FUNCTION void
str8buf_reserve(Scratch *scratch, Str8Buf *strbuf, u64 demand) {
  u64 required = strbuf->str.size + demand;
  if (required <= strbuf->capacity) {
    return;
  }
  u64 new_cap = strbuf->capacity * 2;
  if (required > new_cap) {
    new_cap = required;
  }
  if (new_cap == 0) {
    new_cap = 16;
  }
  u8 *new_buffer = scratch_push_array(scratch, u8, new_cap + 1);
  if (strbuf->str.size) {
    mem_move(new_buffer, strbuf->str.value, strbuf->str.size);
  }
  strbuf->str.value = new_buffer;
  strbuf->capacity = new_cap;
  strbuf->str.value[strbuf->str.size] = 0;
}

FUNCTION void
str8buf_append(Scratch *scratch, Str8Buf *strbuf, Str8 s) {
  str8buf_reserve(scratch, strbuf, s.size);
  mem_move(strbuf->str.value + strbuf->str.size, s.value, s.size);
  strbuf->str.size += s.size;
  strbuf->str.value[strbuf->str.size] = 0;
}

FUNCTION void
str8buf_appendv(Scratch *scratch, Str8Buf *strbuf, const char *fmt, va_list args) {
  va_list args2;
  va_copy(args2, args);

  i32 needed = vsnprintf(NULL, 0, fmt, args);
  if (needed > 0) {
    str8buf_reserve(scratch, strbuf, needed);
    vsnprintf((char *)(strbuf->str.value + strbuf->str.size),
              needed + 1,
              fmt,
              args2);
    strbuf->str.size += needed;
    strbuf->str.value[strbuf->str.size] = 0;
  }
  va_end(args2);
}

FUNCTION void
str8buf_appendf(Scratch *scratch, Str8Buf *strbuf, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  str8buf_appendv(scratch, strbuf, fmt, args);
  va_end(args);
}

FUNCTION void
str8buf_clear(Str8Buf *buf) {
  buf->str.size = 0;
  buf->str.value[0] = 0;
}

FUNCTION UnicodeDecode
utf8_decode(u8 *str, u32 cap) {
  LOCAL_PERSISTENT u8 length[] = {
    1, 1, 1, 1, // 000xx
    1, 1, 1, 1, //
    1, 1, 1, 1, //
    1, 1, 1, 1, //
    0, 0, 0, 0, // 100xx
    0, 0, 0, 0, //
    2, 2, 2, 2, // 110xx
    3, 3,       // 1110x
    4,          // 11110
    0           // 11111
  };
  LOCAL_PERSISTENT u8 first_byte_mask[] = { 0, 0x7F, 0x1F, 0x0F, 0x07 };
  LOCAL_PERSISTENT u8 final_shift[] = { 0, 18, 12, 6, 0 };
  UnicodeDecode       result = { 0xFFFFFFFF, 1 };

  u8 byte = str[0];
  u8 l = length[byte >> 3];
  if (0 < l && l <= cap) {
    u32 cp = (byte & first_byte_mask[l]) << 18;
    switch (l) {
    case 4:  cp |= ((str[3] & 0x3F) << 0);
    case 3:  cp |= ((str[2] & 0x3F) << 6);
    case 2:  cp |= ((str[1] & 0x3F) << 12);
    default: break;
    }
    cp >>= final_shift[l];

    result.codepoint = cp;
    result.increment = l;
  }

  return result;
}

FUNCTION UnicodeDecode
utf16_decode(u16 *str, u32 cap) {
  UnicodeDecode result = { 0xFFFFFFFF, 1 };

  u16 x = str[0];
  if (x < 0xD800 || 0xDFFF < x) {
    result.codepoint = x;
  } else if (cap >= 2) {
    u16 y = str[1];
    if (0xD800 <= x && x < 0xDC00 && 0xDC00 <= y && y < 0xE000) {
      u16 xj = x - 0xD800;
      u16 yj = y - 0xDC00;
      u32 xy = (xj << 10) | yj;
      result.codepoint = xy + 0x10000;
      result.increment = 2;
    }
  }
  return result;
}

FUNCTION u32
utf8_encode(u8 *dst, u32 codepoint) {
  u32 size = 0;
  if (codepoint < (1 << 7)) {
    dst[0] = codepoint;
    size = 1;
  } else if (codepoint < (1 << 11)) {
    dst[0] = 0xC0 | (codepoint >> 6);
    dst[1] = 0x80 | (codepoint & 0x3F);
    size = 2;
  } else if (codepoint < (1 << 16)) {
    dst[0] = 0xE0 | (codepoint >> 12);
    dst[1] = 0x80 | ((codepoint >> 6) & 0x3F);
    dst[2] = 0x80 | (codepoint & 0x3F);
    size = 3;
  } else if (codepoint < (1 << 21)) {
    dst[0] = 0xF0 | (codepoint >> 18);
    dst[1] = 0x80 | ((codepoint >> 12) & 0x3F);
    dst[2] = 0x80 | ((codepoint >> 6) & 0x3F);
    dst[3] = 0x80 | (codepoint & 0x3F);
    size = 4;
  } else {
    dst[0] = '?';
    size = 1;
  }
  return size;
}

FUNCTION u32
utf16_encode(u16 *dst, u32 codepoint) {
  u32 size = 0;
  if (codepoint == 0xFFFFFFFF) {
    dst[0] = (u16)('?');
    size = 1;
  } else if (codepoint < 0x10000) {
    dst[0] = codepoint;
    size = 1;
  } else {
    u32 cpj = codepoint - 0x10000;
    dst[0] = (cpj >> 10) + 0xD800;
    dst[1] = (cpj & 0x3FF) + 0xDC00;
    size = 2;
  }
  return size;
}

FUNCTION Str8
str8_from_str16(Scratch *scratch, Str16 input) {
  Str8 result = { 0 };
  if (input.size) {
    u64           cap = input.size * 3;
    u8           *str = scratch_push_array(scratch, u8, cap + 1);
    u16          *ptr = input.value;
    u16          *opl = ptr + input.size;
    u64           size = 0;
    UnicodeDecode consume;
    for (; ptr < opl; ptr += consume.increment) {
      consume = utf16_decode(ptr, opl - ptr);
      size += utf8_encode(str + size, consume.codepoint);
    }
    str[size] = 0;
    scratch_pop(scratch, (cap - size));
    result.value = str;
    result.size = size;
  }
  return result;
}

FUNCTION Str16
str16_from_str8(Scratch *scratch, Str8 input) {
  Str16 result = { 0 };
  if (input.size) {
    u64           cap = input.size * 2;
    u16          *str = scratch_push_array(scratch, u16, cap + 1);
    u8           *ptr = input.value;
    u8           *opl = ptr + input.size;
    u64           size = 0;
    UnicodeDecode consume;
    for (; ptr < opl; ptr += consume.increment) {
      consume = utf8_decode(ptr, opl - ptr);
      size += utf16_encode(str + size, consume.codepoint);
    }
    str[size] = 0;
    scratch_pop(scratch, (cap - size) * 2);
    result.value = str;
    result.size = size;
  }
  return result;
}

FUNCTION Str8
str8_get_left_of_last_slash(Str8 str) {
  if (str.size > 0) {
    u8 *ptr = str.value + str.size - 1;
    for (; ptr >= str.value; ptr -= 1) {
      if (*ptr == '/' || *ptr == '\\') {
        break;
      }
    }
    if (ptr >= str.value) {
      str.size = (u64)(ptr - str.value);
    } else {
      str.size = 0;
    }
  }
  return str;
}

FUNCTION Str8
str8_get_right_of_last_slash(Str8 str) {
  if (str.size > 0) {
    u8 *ptr = str.value + str.size - 1;
    for (; ptr >= str.value; ptr -= 1) {
      if (*ptr == '/' || *ptr == '\\') {
        break;
      }
    }
    if (ptr >= str.value) {
      ptr += 1;
      str.size = (u64)(str.value + str.size - ptr);
      str.value = ptr;
    }
  }
  return str;
}
