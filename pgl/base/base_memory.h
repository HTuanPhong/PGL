#ifndef BASE_MEMORY_H
#define BASE_MEMORY_H

/*

Persistent heterogeneous data | Persistent homogeneous data
  Arena                       |   Pool arena dense  (generation handle)
                              |
------------------------------+-------------------------------
                              |
Scratch heterogeneous data    | Scratch homogeneous data
  Thread local frame arena    |   Thread local frame arena



Implementation everything is built based on arena:
  Arena
    |------------------|---------------------|
    v                  v                     v
  Scratch arena     Pool arena sparse     Pool arena dense

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

// scratch memory this is a list of blocks

// usage:
// - call the FUNCTIONs
// - trim to the max count
typedef struct ScratchBlock ScratchBlock;
struct ScratchBlock {
  ScratchBlock *next;
  u64           capacity;
  char          data[];
};

typedef struct Scratch {
  ScratchBlock *first;
  ScratchBlock *current;
  ScratchBlock *peak; // high water line for trimming
  u64           current_block_usage;
} Scratch;

typedef struct ScratchMarker {
  Scratch      *scratch;
  ScratchBlock *current;
  u64           current_block_usage;
} ScratchMarker;

// scratch core
FUNCTION void         *_scratch_push(Scratch *scratch, u64 size, u64 alignment);
FUNCTION void         *_scratch_push_zero(Scratch *scratch, u64 size, u64 alignment);
FUNCTION ScratchMarker scratch_get_mark(Scratch *scratch);
FUNCTION void          scratch_pop_to(ScratchMarker *marker);

// context usage
FUNCTION ScratchMarker scratch_begin(Scratch *other_scratch);
FUNCTION void          scratch_end(ScratchMarker marker);

// helpers
FUNCTION void scratch_pop(Scratch *scratch, u64 amount);

// periodically
FUNCTION void scratch_clear(Scratch *scratch);
FUNCTION void scratch_trim(Scratch *scratch);

// clean up
FUNCTION void scratch_free(Scratch *scratch);

// push helpers
#define scratch_push(scratch, type)                   (type *)_scratch_push(scratch, size_of(type), align_of(type))
#define scratch_push_zero(scratch, type)              (type *)_scratch_push_zero(scratch, size_of(type), align_of(type))
#define scratch_push_array(scratch, type, count)      (type *)_scratch_push(scratch, (count) * size_of(type), align_of(type))
#define scratch_push_array_zero(scratch, type, count) (type *)_scratch_push_zero(scratch, (count) * size_of(type), align_of(type))

#endif // BASE_MEMORY_H
