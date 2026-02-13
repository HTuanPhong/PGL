#ifndef PGL_STRING_H
#define PGL_STRING_H
#include <pgl/memory.h>
#include <stddef.h>

typedef struct String {
  char  *value; // Null terminated, and immutable
  size_t size;  // no count null
} String;

typedef struct {
  String string;
  size_t capacity; // no count null
} StringBuilder;

#define Str(lit) (String){ (char *)(lit), sizeof(lit) - 1 }

void StrBuilderReserve(Scratch *scratch, StringBuilder *builder, size_t size);
void StrBuilderPush(Scratch *scratch, StringBuilder *builder, String new);

// string interning?

#endif // PGL_STRING_H
