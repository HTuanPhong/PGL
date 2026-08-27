#ifndef BASE_MEMORY_H
#define BASE_MEMORY_H

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

/*

Persistent heterogeneous data | Persistent homogeneous data
  Arena                       |   AoS Pool arena dense  (generation handle) (cold)
                              |   AoSoA where each chunk is just arena page (generation handle) (hot)
------------------------------+-------------------------------
                              |
Scratch heterogeneous data    | Scratch homogeneous data
  Thread local arena          |   Thread local arena



Implementation everything is built based on arena:
  Arena
    |-------------------------|
    v                         v
  Thread local arena      Pool arena dense
 (arena scope ability)   (generation handle ability)

Arena allocate in multiple of page size that mean the code to manage pages are easy.

Arena have high water line trim feature to return the unused pages.

I dont use the idea of virtual memory because not all platform can provide virtual memory and
at the end of the day its weird to reason about and lost potential for a sick page allocator.
In the future maybe there we be a global page allocator that keep a bank of memory pages. that way
i dont have to worry about multiple high water line anymore **hype**? this will eventually come when
i have the web platform supported.

About Persistent homogeneous data these question arise:
how do we loop fast through entity with the same:
 - inheritance/composition: -> use a pool of struct that give back generation handle
 - Dynamic state: -> array/list of the handle from the first one
 - Hierarchies: -> Relation table of the handle from the first one
 - spatial proximity: -> grid of handle chunk like minecraft
 - other forms: -> built upon handle
*/

// arena grow on page have water line to trim

typedef struct ArenaMemoryBlock ArenaMemoryBlock;
struct ArenaMemoryBlock {
  ArenaMemoryBlock *next;
  u64               capacity;
  char              data[];
};

typedef struct Arena {
  ArenaMemoryBlock *first; // for freeing arena
  ArenaMemoryBlock *peak;  // high water line for trimming
  ArenaMemoryBlock *current;
  u64               current_block_usage;
} Arena;

typedef struct ArenaMarker {
  Arena            *arena;
  ArenaMemoryBlock *current;
  u64               current_block_usage;
} ArenaMarker;

// dense entity structures

typedef struct Handle {
  u32 sparse_index;
  u32 generation;
} Handle;

typedef struct SparseSlot {
  u32 generation;
  union {
    u32 dense_index; // when active
    u32 next_free;   // when empty
  };
} SparseSlot;

typedef struct ArenaAoS {
  Arena sparse_arena;    // contain SparseSlot map sparse index -> dense index
  Arena dense_to_sparse; // map dense index -> sparse index
  Arena dense_arena;     // contain actual data
  u32   entity_size;
} ArenaAoS;

typedef struct ArenaAoSoA {
  Arena dense_arena;
  Arena sparse_arena;
  u32   entity_size;
} ArenaAoSoA;

// arena core
FUNCTION void       *_arena_push(Arena *arena, u64 size, u64 alignment);
FUNCTION void       *_arena_push_zero(Arena *arena, u64 size, u64 alignment);
FUNCTION ArenaMarker arena_get_mark(Arena *arena);
FUNCTION void        arena_pop_to(ArenaMarker *marker);

// context usage
FUNCTION ArenaMarker arena_begin(Arena *other_arena);
FUNCTION void        arena_end(ArenaMarker marker);

// helpers
FUNCTION void arena_pop(Arena *arena, u64 amount);

// periodically
FUNCTION void arena_clear(Arena *arena);
FUNCTION void arena_trim(Arena *arena);

// clean up
FUNCTION void arena_free(Arena *arena);

// push helpers
#define arena_push(arena, type)                   (type *)_arena_push(arena, size_of(type), align_of(type))
#define arena_push_zero(arena, type)              (type *)_arena_push_zero(arena, size_of(type), align_of(type))
#define arena_push_array(arena, type, count)      (type *)_arena_push(arena, (count) * size_of(type), align_of(type))
#define arena_push_array_zero(arena, type, count) (type *)_arena_push_zero(arena, (count) * size_of(type), align_of(type))

#endif // BASE_MEMORY_H
