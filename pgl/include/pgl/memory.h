#ifndef MEMORY_H
#define MEMORY_H
#include <stddef.h> // size_t
// scratch memory this is a list of blocks

// usage:
// - call the functions
// - trim to the max count

typedef struct ScratchBlock {
  ScratchBlock *next;
  size_t capacity;
  char data[];
} ScratchBlock;

typedef struct Scratch
{
  ScratchBlock *first;
  ScratchBlock *current;
  ScratchBlock *maxBlock;
  size_t currentBlockUsed;
} Scratch;

typedef struct ScratchMarker
{
  Scratch *scratch;
  ScratchBlock *current;
  size_t currentBlockUsed;
} ScratchMarker;

// GLOBAL
static Scratch G_scratch[2] = {0};

void *_ScratchPush(Scratch *scratch, size_t size, size_t alignment);
void *_ScratchPushZero(Scratch *scratch, size_t size, size_t alignment);
ScratchMarker ScratchGetMark(Scratch *scratch);
void ScratchPopTo(ScratchMarker marker);
void ScratchClear(Scratch *scratch);
ScratchMarker ScratchBegin(Scratch *otherScratch);
void ScratchEnd(ScratchMarker marker);
void ScratchTrim(Scratch *scratch);
void ScratchFree(Scratch *scratch);
#define ScratchPush(scratch, type) (type *)_ScratchPushPush(scratch, sizeof(type), _Alignof(type))
#define ScratchPushZero(scratch, type) (type *)_ScratchPushPushZero(scratch, sizeof(type), _Alignof(type))
#define ScratchPushArray(scratch, count, type) (type *)_ScratchPushPush(scratch, (count) * sizeof(type), _Alignof(type))
#define ScratchPushArrayZero(scratch, count, type) (type *)_ScratchPushPushZero(scratch, (count) * sizeof(type), _Alignof(type))
// pool memory this is a dense + indirect access + generational index handle


#endif //MEMORY_H