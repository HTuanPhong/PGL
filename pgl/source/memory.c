#include <pgl/memory.h>
#include <stdlib.h> // malloc, free
#include <assert.h>
#include <string.h> // memset
#include <stddef.h> // size_t, uintptr_t

#define MinBlockSize 65536

// calculates the padding needed to align a value x to a boundary b, 
// where b must be a power of two, x is unsigned.
// Hacker's delight Section 3-1
#define AlignPadPow2(x, b) (-(x) & ((b) - 1))

// aligns a value x up to the nearest multiple of b,
// where b must be a power of two, x is unsigned.
// Hacker's delight Section 3-1
#define AlignPow2(x,b)  (((x) + (b) - 1)&(-(b)))

// x is unsigned
// Hacker's delight Section 2-1
#define IsPow2OrZero(x) ((((x) - 1)&(x)) == 0) 

// x is unsigned
// Hacker's delight Section 2-1
#define IsPow2(x)       ((x)!=0 && IsPow2OrZero(x))

#define Max(A,B)        (((A)>(B))?(A):(B))

static void *Malloc(size_t size) {
  void *p = malloc(size);
  if (p) return p; 
  abort();
}

static void *Realloc(void *ptr, size_t size) {
  void *p = realloc(ptr,size);
  if (p) return p; 
  abort();
}

void *_ScratchPush(Scratch *scratch, size_t size, size_t alignment) {
  assert(scratch);
  assert(size > 0);
  assert(IsPow2(alignment));
  if (scratch->first == NULL) {
    size_t mallocSize = Max(size + alignment,MinBlockSize);// + alignment so we alway have gap to align
    ScratchBlock *newBlock = Malloc(sizeof(ScratchBlock) + mallocSize);
    newBlock->next = NULL;
    newBlock->capacity = mallocSize;
    scratch->first = newBlock;
    scratch->current = newBlock;
    scratch->maxBlock = newBlock;
    scratch->currentBlockUsed = 0;
  }
  size_t alignPad = AlignPadPow2((uintptr_t)(scratch->current->data + scratch->currentBlockUsed),alignment);
  size_t alignedSize = alignPad + size;
  if (scratch->currentBlockUsed + alignedSize > scratch->current->capacity) {
    ScratchBlock *ChosenBlock = NULL;
    scratch->currentBlockUsed = 0;
    for (ScratchBlock *prev = scratch->current, *ptr = scratch->current->next; 
      ptr != NULL; 
      prev = ptr, ptr = ptr->next)
    {
      alignPad = AlignPadPow2((uintptr_t)ptr->data,alignment);
      alignedSize = alignPad + size;
      if (alignedSize <= ptr->capacity)
      {
        ChosenBlock = ptr;
        prev->next = ptr->next;
        ptr->next = scratch->current->next;
        scratch->current->next = ptr;
        scratch->current = ptr;
        break;
      }
    }
    if (!ChosenBlock) {
      size_t mallocSize = Max(size + alignment,MinBlockSize);// + alignment so we alway have gap to align
      ChosenBlock = Malloc(sizeof(ScratchBlock) + mallocSize);
      ChosenBlock->next = NULL;
      ChosenBlock->capacity = mallocSize;
      scratch->current->next = ChosenBlock;
      scratch->current = ChosenBlock;
      scratch->maxBlock = ChosenBlock;
      alignPad = AlignPadPow2((uintptr_t)ChosenBlock->data,alignment);
      alignedSize = alignPad + size;
    }
  }
  void *result = scratch->current->data + scratch->currentBlockUsed + alignPad;
  scratch->currentBlockUsed += alignedSize;
  return result;
}

void *_ScratchPushZero(Scratch *scratch, size_t size, size_t alignment) {
  void *result = _ScratchPush(scratch, size, alignment);
  memset(result, 0, size);
  return result;
}

ScratchMarker ScratchGetMark(Scratch *scratch) {
  ScratchMarker result;
  result.scratch = scratch;
  result.current = scratch->current;
  result.currentBlockUsed = scratch->currentBlockUsed;
  return result;
}

void ScratchPopTo(ScratchMarker marker) {
  if (!marker.current) marker.current = marker.scratch->first; //incase we not init yet
  marker.scratch->current = marker.current;
  marker.scratch->currentBlockUsed = marker.currentBlockUsed;
}

void ScratchClear(Scratch *scratch) {
  ScratchMarker m = {scratch, scratch->first, 0};
  ScratchPopTo(m);
}

ScratchMarker ScratchBegin(Scratch *otherScratch) {
  Scratch *chosen = (G_scratch == otherScratch) ? G_scratch+1 : G_scratch;
  return ScratchGetMark(chosen);
}

void ScratchEnd(ScratchMarker marker) {
  ScratchPopTo(marker);
}

void ScratchTrim(Scratch *scratch) {
  if (!scratch->maxBlock) return;
  for (ScratchBlock *p = scratch->maxBlock->next; p != NULL;)
  {
    ScratchBlock *toFree = p;
    p = p->next;
    free(toFree);
  }
  scratch->maxBlock = scratch->current;
}

void ScratchFree(Scratch *scratch) {
  for (ScratchBlock *p = scratch->first; p != NULL;)
  {
    ScratchBlock *toFree = p;
    p = p->next;
    free(toFree);
  }
  scratch->first = NULL;
  scratch->current = NULL;
  scratch->maxBlock = NULL;
  scratch->currentBlockUsed = 0;
}