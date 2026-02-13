#include <pgl/memory.h>
#include <pgl/string.h>
#include <string.h>

#define Max(A, B) (((A) > (B)) ? (A) : (B))

void StrBuilderReserve(Scratch *scratch, StringBuilder *builder, size_t size) {
  if (builder->capacity >= size) {
    return;
  }
  char *newBuffer = ScratchPushArray(scratch, size + 1, char);
  if (builder->string.size) {
    memmove(newBuffer, builder->string.value, builder->string.size + 1);
  }
  builder->string.value = newBuffer;
  builder->capacity = size;
}

void StrBuilderPush(Scratch *scratch, StringBuilder *builder, String new) {
  size_t newSize = builder->string.size + new.size;
  StrBuilderReserve(scratch, builder, Max(builder->capacity * 2, newSize));
  memmove(builder->string.value + builder->string.size, new.value, new.size + 1);
  builder->string.size += new.size;
}
