FUNCTION void *
_arena_push(Arena *arena, u64 size, u64 alignment) {
  assert(arena);
  assert(size > 0);
  assert(is_pow2(alignment));
  u32 page_size = os_app_data.system_info.page_size;
  assert(alignment <= page_size);
  u32 header_size = size_of(ArenaMemoryBlock);

  // if first time push then init
  if (arena->first == NULL) {
    u32 align_padding_init = align_pad_pow2(header_size, alignment);
    u64 malloc_size = align_pow2(header_size + align_padding_init + size, page_size);

    ArenaMemoryBlock *new_block = os_memory_reserve(malloc_size);
    new_block->next = NULL;
    new_block->capacity = malloc_size;
    arena->first = new_block;
    arena->current = new_block;
    arena->peak = new_block;
    arena->current_block_usage = header_size;
  }

  // cal alignment
  u32 align_pad = align_pad_pow2(arena->current_block_usage, alignment);

  // if unfit then find more memory
  if (arena->current_block_usage + align_pad + size > arena->current->capacity) {
    u32               align_padding_init = align_pad_pow2(header_size, alignment);
    u64               needed = header_size + align_padding_init + size;
    ArenaMemoryBlock *nextBlock = arena->current->next;
    // If next block exists and fits the data
    if (nextBlock && (needed <= nextBlock->capacity)) {
      arena->current = nextBlock;
    } else { // Must allocate a NEW block
      u64               malloc_size = align_pow2(needed, page_size);
      ArenaMemoryBlock *new_block = os_memory_reserve(malloc_size);
      new_block->next = arena->current->next;
      new_block->capacity = malloc_size;
      arena->current->next = new_block;
      arena->current = new_block;
    }
    // Reset usage for the new current block
    arena->current_block_usage = header_size;
    align_pad = align_padding_init;
  }

  // update high water rise peak for trim op later
  if (arena->peak->next == arena->current) {
    arena->peak = arena->current;
  }

  // finally push
  void *result = (u8 *)(arena->current) + arena->current_block_usage + align_pad;
  arena->current_block_usage += align_pad + size;
  return result;
}

FUNCTION void *
_arena_push_zero(Arena *arena, u64 size, u64 alignment) {
  void *result = _arena_push(arena, size, alignment);
  mem_set(result, 0, size);
  return result;
}

FUNCTION ArenaMarker
arena_get_mark(Arena *arena) {
  ArenaMarker result;
  result.arena = arena;
  result.current = arena->current;
  result.current_block_usage = arena->current_block_usage;
  return result;
}

FUNCTION void
arena_pop_to(ArenaMarker *marker) {
  if (!marker->current) {
    marker->current = marker->arena->first; // incase we not init yet
  }
  marker->arena->current = marker->current;
  marker->arena->current_block_usage = marker->current_block_usage;
}

FUNCTION void
arena_clear(Arena *arena) {
  ArenaMarker m = { arena, arena->first, 0 };
  arena_pop_to(&m);
}

FUNCTION ArenaMarker
arena_begin(Arena *other_arena) {
  Arena *G_arena = thread_local_context.arena;
  Arena *chosen = (G_arena == other_arena) ? (G_arena + 1) : G_arena;
  return arena_get_mark(chosen);
}

FUNCTION void
arena_end(ArenaMarker marker) {
  arena_pop_to(&marker);
}

FUNCTION void
arena_trim(Arena *arena) {
  if (!arena->peak) {
    return;
  }
  for (ArenaMemoryBlock *p = arena->peak->next; p != NULL;) {
    ArenaMemoryBlock *toFree = p;
    p = p->next;
    os_memory_release(toFree, toFree->capacity);
  }
  arena->peak = arena->current;
}

FUNCTION void
arena_free(Arena *arena) {
  for (ArenaMemoryBlock *p = arena->first; p != NULL;) {
    ArenaMemoryBlock *toFree = p;
    p = p->next;
    os_memory_release(toFree, toFree->capacity);
  }
  arena->first = NULL;
  arena->current = NULL;
  arena->peak = NULL;
  arena->current_block_usage = 0;
}

FUNCTION void
arena_pop(Arena *arena, u64 amount) {
  if (arena->current_block_usage >= amount) {
    arena->current_block_usage -= amount;
  }
}

//
