#include <assert.h>
#include <pgl/memory.h>
#include <stddef.h> // size_t
#include <stdint.h> // uintptr_t
#include <stdlib.h> // malloc, free
#include <string.h> // memset

// GLOBAL
static Scratch G_scratch[2] = { 0 };

#define MinBlockSize 65536

// calculates the padding needed to align a value x to a boundary b,
// where b must be a power of two, x is unsigned.
// Hacker's delight Section 3-1
#define AlignPadPow2(x, b) (-(x) & ((b) - 1))

// aligns a value x up to the nearest multiple of b,
// where b must be a power of two, x is unsigned.
// Hacker's delight Section 3-1
#define AlignPow2(x, b) (((x) + (b) - 1) & (-(b)))

// x is unsigned
// Hacker's delight Section 2-1
#define IsPow2OrZero(x) ((((x) - 1) & (x)) == 0)

// x is unsigned
// Hacker's delight Section 2-1
#define IsPow2(x) ((x) != 0 && IsPow2OrZero(x))

#define Max(A, B) (((A) > (B)) ? (A) : (B))

static void *Malloc(size_t size) {
  void *p = malloc(size);
  if (p) {
    return p;
  }
  abort();
}

static void *Realloc(void *ptr, size_t size) {
  void *p = realloc(ptr, size);
  if (p) {
    return p;
  }
  abort();
}

void *_ScratchPush(Scratch *scratch, size_t size, size_t alignment) {
  assert(scratch);
  assert(size > 0);
  assert(IsPow2(alignment));
  if (scratch->first == NULL) {
    size_t        mallocSize = Max(size + alignment, MinBlockSize); // + alignment so we alway have gap to align
    ScratchBlock *newBlock = Malloc(sizeof(ScratchBlock) + mallocSize);
    newBlock->next = NULL;
    newBlock->capacity = mallocSize;
    scratch->first = newBlock;
    scratch->current = newBlock;
    scratch->maxBlock = newBlock;
    scratch->currentBlockUsed = 0;
  }
  size_t alignPad = AlignPadPow2((uintptr_t)(scratch->current->data + scratch->currentBlockUsed), alignment);
  if (scratch->currentBlockUsed + alignPad + size > scratch->current->capacity) {
    ScratchBlock *nextBlock = scratch->current->next;
    // If next block exists and fits the data
    if (nextBlock && (AlignPadPow2((uintptr_t)nextBlock->data, alignment) + size <= nextBlock->capacity)) {
      scratch->current = nextBlock;
    } else { // Must allocate a NEW block
      size_t        mallocSize = Max(size + alignment, MinBlockSize);
      ScratchBlock *newBlock = Malloc(sizeof(ScratchBlock) + mallocSize);
      newBlock->next = scratch->current->next;
      scratch->current->next = newBlock;
      scratch->current = newBlock;
    }
    // Reset usage for the new current block
    scratch->currentBlockUsed = 0;
    alignPad = AlignPadPow2((uintptr_t)scratch->current->data, alignment);
  }
  if (scratch->maxBlock->next == scratch->current) {
    scratch->maxBlock = scratch->current;
  }
  void *result = scratch->current->data + scratch->currentBlockUsed + alignPad;
  scratch->currentBlockUsed += alignPad + size;
  return result;
}

void *_ScratchPushZero(Scratch *scratch, size_t size, size_t alignment) {
  void *result = _ScratchPush(scratch, size, alignment);
  memset(result, 0, size);
  return result;
}

ScratchMarker ScratchGetMark(Scratch *scratch) {
  assert(scratch);
  ScratchMarker result;
  result.scratch = scratch;
  result.current = scratch->current;
  result.currentBlockUsed = scratch->currentBlockUsed;
  return result;
}

void ScratchPopTo(ScratchMarker marker) {
  assert(marker.scratch);
  if (!marker.current) {
    marker.current = marker.scratch->first; // incase we not init yet
  }
  marker.scratch->current = marker.current;
  marker.scratch->currentBlockUsed = marker.currentBlockUsed;
}

void ScratchClear(Scratch *scratch) {
  ScratchMarker m = { scratch, scratch->first, 0 };
  ScratchPopTo(m);
}

ScratchMarker ScratchBegin(Scratch *otherScratch) {
  Scratch *chosen = (G_scratch == otherScratch) ? G_scratch + 1 : G_scratch;
  return ScratchGetMark(chosen);
}

void ScratchEnd(ScratchMarker marker) {
  ScratchPopTo(marker);
}

void ScratchTrim(Scratch *scratch) {
  assert(scratch);
  if (!scratch->maxBlock) {
    return;
  }
  for (ScratchBlock *p = scratch->maxBlock->next; p != NULL;) {
    ScratchBlock *toFree = p;
    p = p->next;
    free(toFree);
  }
  scratch->maxBlock = scratch->current;
}

void ScratchFree(Scratch *scratch) {
  assert(scratch);
  for (ScratchBlock *p = scratch->first; p != NULL;) {
    ScratchBlock *toFree = p;
    p = p->next;
    free(toFree);
  }
  scratch->first = NULL;
  scratch->current = NULL;
  scratch->maxBlock = NULL;
  scratch->currentBlockUsed = 0;
}

#define InitialDynamicBlockSize 4096

void _DynamicBlockPush(DynamicBlock *dynamicBlock, const void *data, size_t size) {
  assert(dynamicBlock);
  assert(size > 0);
  if (dynamicBlock->capacity == 0) {
    size_t mallocSize = Max(size, InitialDynamicBlockSize);
    dynamicBlock->data = Malloc(mallocSize);
    dynamicBlock->capacity = mallocSize;
  }
  size_t newSize = dynamicBlock->size + size;
  if (newSize > dynamicBlock->capacity) {
    size_t ReallocSize = Max(newSize, dynamicBlock->capacity * 2);
    dynamicBlock->data = Realloc(dynamicBlock->data, ReallocSize);
    dynamicBlock->capacity = ReallocSize;
  }
  void *dst = dynamicBlock->data + dynamicBlock->size;
  memmove(dst, data, size);
  dynamicBlock->size = newSize;
  dynamicBlock->maxSize = Max(dynamicBlock->maxSize, newSize);
}

void *_DynamicBlockAt(DynamicBlock *dynamicBlock, size_t index, size_t size) {
  return dynamicBlock->data + size * index;
}

void DynamicBlockTrimTo(DynamicBlock *dynamicBlock, size_t size) {
  assert(dynamicBlock);
  assert(size <= dynamicBlock->size);
  dynamicBlock->size = size;
  dynamicBlock->maxSize = Max(dynamicBlock->maxSize, size);
}
