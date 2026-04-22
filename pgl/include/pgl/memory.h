#ifndef PGL_MEMORY_H
#define PGL_MEMORY_H
#include <pgl/defines.h>
// scratch memory this is a list of blocks

// usage:
// - call the functions
// - trim to the max count
typedef struct ScratchBlock ScratchBlock;
struct ScratchBlock {
  ScratchBlock *next;
  usize         capacity;
  char          data[];
};

typedef struct Scratch {
  ScratchBlock *first;
  ScratchBlock *current;
  ScratchBlock *peak; // high water line for trimming
  usize         blocks_used;
} Scratch;

typedef struct ScratchMarker {
  Scratch      *scratch;
  ScratchBlock *current;
  usize         blocks_used;
} ScratchMarker;

void         *_scratch_push(Scratch *scratch, usize size, usize alignment);
void         *_scratch_push_zero(Scratch *scratch, usize size, usize alignment);
ScratchMarker scratch_get_mark(Scratch *scratch);
void          scratch_pop_to(ScratchMarker marker);
void          scratch_clear(Scratch *scratch);
ScratchMarker scratch_begin(Scratch *other_scratch);
void          scratch_end(ScratchMarker marker);
void          scratch_trim(Scratch *scratch);
void          scratch_free(Scratch *scratch);
#define scratch_push(scratch, type)                   (type *)_scratch_push(scratch, sizeof(type), alignof(type))
#define scratch_push_zero(scratch, type)              (type *)_scratch_push_zero(scratch, sizeof(type), alignof(type))
#define scratch_push_array(scratch, count, type)      (type *)_scratch_push(scratch, (count) * sizeof(type), alignof(type))
#define scratch_push_array_zero(scratch, count, type) (type *)_scratch_push_zero(scratch, (count) * sizeof(type), alignof(type))

// dynamic block this is a dense + indirect access no delete.

typedef struct DynamicBlock {
  char *data;
  usize capacity;
  usize peak;
  usize size;
} DynamicBlock;

void  _dynamic_block_push(DynamicBlock *dynamic_block, const void *data, usize size);
void *_dynamic_block_at(DynamicBlock *dynamic_block, usize index, usize size);
void  dynamic_block_trim_to(DynamicBlock *dynamic_block, usize size);
#define dynamic_block_push(dBlock, data, type)              _dynamic_block_push(dBlock, data, sizeof(type))
#define dynamic_block_push_array(dBlock, data, count, type) _dynamic_block_push(dBlock, data, (count) * sizeof(type))
#define dynamic_block_at(dBlock, index, type)               (type *)_dynamic_block_at(dBlock, index, sizeof(type))

// pool memory this is a dense + indirect access + generational index handle

#endif // PGL_MEMORY_H
