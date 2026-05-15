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

function void *
_scratch_push(Scratch *scratch, u64 size, u64 alignment) {
  assert(scratch);
  assert(size > 0);
  assert(is_pow2(alignment));
  u32 page_size = os_app_data.system_info.page_size;
  assert(alignment <= page_size);
  u32 header_size = size_of(ScratchBlock);

  // if first time push then init
  if (scratch->first == NULL) {
    u32 align_padding_init = align_pad_pow2(header_size, alignment);
    u64 malloc_size = align_pow2(header_size + align_padding_init + size, page_size);

    ScratchBlock *new_block = os_memory_reserve(malloc_size);
    new_block->next = NULL;
    new_block->capacity = malloc_size;
    scratch->first = new_block;
    scratch->current = new_block;
    scratch->peak = new_block;
    scratch->current_block_usage = header_size;
  }

  // cal alignment
  u32 align_pad = align_pad_pow2(scratch->current_block_usage, alignment);

  // if unfit then find more memory
  if (scratch->current_block_usage + align_pad + size > scratch->current->capacity) {
    u32           align_padding_init = align_pad_pow2(header_size, alignment);
    u64           needed = header_size + align_padding_init + size;
    ScratchBlock *nextBlock = scratch->current->next;
    // If next block exists and fits the data
    if (nextBlock && (needed <= nextBlock->capacity)) {
      scratch->current = nextBlock;
    } else { // Must allocate a NEW block
      u64           malloc_size = align_pow2(needed, page_size);
      ScratchBlock *new_block = os_memory_reserve(malloc_size);
      new_block->next = scratch->current->next;
      new_block->capacity = malloc_size;
      scratch->current->next = new_block;
      scratch->current = new_block;
    }
    // Reset usage for the new current block
    scratch->current_block_usage = header_size;
    align_pad = align_padding_init;
  }

  // update high water rise peak for trim op later
  if (scratch->peak->next == scratch->current) {
    scratch->peak = scratch->current;
  }

  // finally push
  void *result = (u8 *)(scratch->current) + scratch->current_block_usage + align_pad;
  scratch->current_block_usage += align_pad + size;
  return result;
}

function void *
_scratch_push_zero(Scratch *scratch, u64 size, u64 alignment) {
  void *result = _scratch_push(scratch, size, alignment);
  mem_set(result, 0, size);
  return result;
}

function ScratchMarker
scratch_get_mark(Scratch *scratch) {
  ScratchMarker result;
  result.scratch = scratch;
  result.current = scratch->current;
  result.current_block_usage = scratch->current_block_usage;
  return result;
}

function void
scratch_pop_to(ScratchMarker *marker) {
  if (!marker->current) {
    marker->current = marker->scratch->first; // incase we not init yet
  }
  marker->scratch->current = marker->current;
  marker->scratch->current_block_usage = marker->current_block_usage;
}

function void
scratch_clear(Scratch *scratch) {
  ScratchMarker m = { scratch, scratch->first, 0 };
  scratch_pop_to(&m);
}

function ScratchMarker
scratch_begin(Scratch *other_scratch) {
  Scratch *G_scratch = thread_local_context.scratch;
  Scratch *chosen = (G_scratch == other_scratch) ? (G_scratch + 1) : G_scratch;
  return scratch_get_mark(chosen);
}

function void
scratch_end(ScratchMarker marker) {
  scratch_pop_to(&marker);
}

function void
scratch_trim(Scratch *scratch) {
  if (!scratch->peak) {
    return;
  }
  for (ScratchBlock *p = scratch->peak->next; p != NULL;) {
    ScratchBlock *toFree = p;
    p = p->next;
    os_memory_release(toFree, toFree->capacity);
  }
  scratch->peak = scratch->current;
}

function void
scratch_free(Scratch *scratch) {
  for (ScratchBlock *p = scratch->first; p != NULL;) {
    ScratchBlock *toFree = p;
    p = p->next;
    os_memory_release(toFree, toFree->capacity);
  }
  scratch->first = NULL;
  scratch->current = NULL;
  scratch->peak = NULL;
  scratch->current_block_usage = 0;
}

function void
scratch_pop(Scratch *scratch, u64 amount) {
  if (scratch->current_block_usage >= amount) {
    scratch->current_block_usage -= amount;
  }
}
