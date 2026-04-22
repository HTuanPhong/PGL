#include <pgl/defines.h>
#include <pgl/memory.h>
#include <stdlib.h> // malloc, free, abort

// GLOBAL
static Scratch G_scratch[2] = { 0 };

#define MIN_BLOCK_SIZE 65536

// calculates the padding needed to align a value x to a boundary b,
// where b must be a power of two, x is unsigned.
// Hacker's delight Section 3-1
#define align_pad_pow2(x, b) (-(x) & ((b) - 1))

// aligns a value x up to the nearest multiple of b,
// where b must be a power of two, x is unsigned.
// Hacker's delight Section 3-1
#define align_pow2(x, b) (((x) + (b) - 1) & (-(b)))

// x is unsigned
// Hacker's delight Section 2-1
#define is_pow2_or_zero(x) ((((x) - 1) & (x)) == 0)

// x is unsigned
// Hacker's delight Section 2-1
#define is_pow2(x) ((x) != 0 && is_pow2_or_zero(x))

static void *malloc_checked(usize size) {
  void *p = malloc(size);
  if (p) {
    return p;
  }
  abort();
}

static void *realloc_checkded(void *ptr, usize size) {
  void *p = realloc(ptr, size);
  if (p) {
    return p;
  }
  abort();
}

void *_scratch_push(Scratch *scratch, usize size, usize alignment) {
  assert(scratch);
  assert(size > 0);
  assert(is_pow2(alignment));
  if (scratch->first == NULL) {
    usize         malloc_size = max(size + alignment, MIN_BLOCK_SIZE); // + alignment so we alway have gap to align
    ScratchBlock *new_block = malloc_checked(sizeof(ScratchBlock) + malloc_size);
    new_block->next = NULL;
    new_block->capacity = malloc_size;
    scratch->first = new_block;
    scratch->current = new_block;
    scratch->peak = new_block;
    scratch->blocks_used = 0;
  }
  usize alignPad = align_pad_pow2((uptr)(scratch->current->data + scratch->blocks_used), alignment);
  if (scratch->blocks_used + alignPad + size > scratch->current->capacity) {
    ScratchBlock *nextBlock = scratch->current->next;
    // If next block exists and fits the data
    if (nextBlock && (align_pad_pow2((uptr)nextBlock->data, alignment) + size <= nextBlock->capacity)) {
      scratch->current = nextBlock;
    } else { // Must allocate a NEW block
      usize         malloc_size = max(size + alignment, MIN_BLOCK_SIZE);
      ScratchBlock *new_block = malloc_checked(sizeof(ScratchBlock) + malloc_size);
      new_block->next = scratch->current->next;
      scratch->current->next = new_block;
      scratch->current = new_block;
    }
    // Reset usage for the new current block
    scratch->blocks_used = 0;
    alignPad = align_pad_pow2((uptr)scratch->current->data, alignment);
  }
  if (scratch->peak->next == scratch->current) {
    scratch->peak = scratch->current;
  }
  void *result = scratch->current->data + scratch->blocks_used + alignPad;
  scratch->blocks_used += alignPad + size;
  return result;
}

void *_scratch_push_zero(Scratch *scratch, usize size, usize alignment) {
  void *result = _scratch_push(scratch, size, alignment);
  mem_set(result, 0, size);
  return result;
}

ScratchMarker scratch_get_mark(Scratch *scratch) {
  assert(scratch);
  ScratchMarker result;
  result.scratch = scratch;
  result.current = scratch->current;
  result.blocks_used = scratch->blocks_used;
  return result;
}

void scratch_pop_to(ScratchMarker marker) {
  assert(marker.scratch);
  if (!marker.current) {
    marker.current = marker.scratch->first; // incase we not init yet
  }
  marker.scratch->current = marker.current;
  marker.scratch->blocks_used = marker.blocks_used;
}

void scratch_clear(Scratch *scratch) {
  ScratchMarker m = { scratch, scratch->first, 0 };
  scratch_pop_to(m);
}

ScratchMarker scratch_begin(Scratch *other_scratch) {
  Scratch *chosen = (G_scratch == other_scratch) ? G_scratch + 1 : G_scratch;
  return scratch_get_mark(chosen);
}

void scratch_end(ScratchMarker marker) {
  scratch_pop_to(marker);
}

void scratch_trim(Scratch *scratch) {
  assert(scratch);
  if (!scratch->peak) {
    return;
  }
  for (ScratchBlock *p = scratch->peak->next; p != NULL;) {
    ScratchBlock *toFree = p;
    p = p->next;
    free(toFree);
  }
  scratch->peak = scratch->current;
}

void scratch_free(Scratch *scratch) {
  assert(scratch);
  for (ScratchBlock *p = scratch->first; p != NULL;) {
    ScratchBlock *toFree = p;
    p = p->next;
    free(toFree);
  }
  scratch->first = NULL;
  scratch->current = NULL;
  scratch->peak = NULL;
  scratch->blocks_used = 0;
}

#define INITIAL_DYNAMIC_BLOCK_SIZE 4096

void _dynamic_block_push(DynamicBlock *dynamic_block, const void *data, usize size) {
  assert(dynamic_block);
  assert(size > 0);
  if (dynamic_block->capacity == 0) {
    usize malloc_size = max(size, INITIAL_DYNAMIC_BLOCK_SIZE);
    dynamic_block->data = malloc_checked(malloc_size);
    dynamic_block->capacity = malloc_size;
  }
  usize newSize = dynamic_block->size + size;
  if (newSize > dynamic_block->capacity) {
    usize ReallocSize = max(newSize, dynamic_block->capacity * 2);
    dynamic_block->data = realloc_checkded(dynamic_block->data, ReallocSize);
    dynamic_block->capacity = ReallocSize;
  }
  void *dst = dynamic_block->data + dynamic_block->size;
  mem_move(dst, data, size);
  dynamic_block->size = newSize;
  dynamic_block->peak = max(dynamic_block->peak, newSize);
}

void *_dynamic_block_at(DynamicBlock *dynamic_block, usize index, usize size) {
  return dynamic_block->data + size * index;
}

void dynamic_block_trim_to(DynamicBlock *dynamic_block, usize size) {
  assert(dynamic_block);
  assert(size <= dynamic_block->size);
  dynamic_block->size = size;
  dynamic_block->peak = max(dynamic_block->peak, size);
}
