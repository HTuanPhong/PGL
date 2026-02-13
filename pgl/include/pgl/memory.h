#ifndef PGL_MEMORY_H
#define PGL_MEMORY_H
#include <stddef.h> // size_t
// scratch memory this is a list of blocks

// usage:
// - call the functions
// - trim to the max count
typedef struct ScratchBlock ScratchBlock;
struct ScratchBlock {
  ScratchBlock *next;
  size_t        capacity;
  char          data[];
};

typedef struct Scratch {
  ScratchBlock *first;
  ScratchBlock *current;
  ScratchBlock *maxBlock;
  size_t        currentBlockUsed;
} Scratch;

typedef struct ScratchMarker {
  Scratch      *scratch;
  ScratchBlock *current;
  size_t        currentBlockUsed;
} ScratchMarker;

void         *_ScratchPush(Scratch *scratch, size_t size, size_t alignment);
void         *_ScratchPushZero(Scratch *scratch, size_t size, size_t alignment);
ScratchMarker ScratchGetMark(Scratch *scratch);
void          ScratchPopTo(ScratchMarker marker);
void          ScratchClear(Scratch *scratch);
ScratchMarker ScratchBegin(Scratch *otherScratch);
void          ScratchEnd(ScratchMarker marker);
void          ScratchTrim(Scratch *scratch);
void          ScratchFree(Scratch *scratch);
#define ScratchPush(scratch, type)                 (type *)_ScratchPush(scratch, sizeof(type), _Alignof(type))
#define ScratchPushZero(scratch, type)             (type *)_ScratchPushZero(scratch, sizeof(type), _Alignof(type))
#define ScratchPushArray(scratch, count, type)     (type *)_ScratchPush(scratch, (count) * sizeof(type), _Alignof(type))
#define ScratchPushArrayZero(scratch, count, type) (type *)_ScratchPushZero(scratch, (count) * sizeof(type), _Alignof(type))

// dynamic block this is a dense + indirect access no delete.

typedef struct DynamicBlock {
  char  *data;
  size_t capacity;
  size_t maxSize;
  size_t size;
} DynamicBlock;

void  _DynamicBlockPush(DynamicBlock *dynamicBlock, const void *data, size_t size);
void *_DynamicBlockAt(DynamicBlock *dynamicBlock, size_t index, size_t size);
void  DynamicBlockTrimTo(DynamicBlock *dynamicBlock, size_t size);
#define DynamicBlockPush(dBlock, data, type)             _DynamicBlockPush(dBlock, data, sizeof(type))
#define DynamicBlockPushArray(dBlock, data, count, type) _DynamicBlockPush(dBlock, data, (count) * sizeof(type))
#define DynamicBlockAt(dBlock, index, type)              (type *)_DynamicBlockAt(dBlock, index, sizeof(type))

// pool memory this is a dense + indirect access + generational index handle

#endif // PGL_MEMORY_H
