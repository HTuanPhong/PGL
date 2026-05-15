#ifndef BASE_MEMORY_H
#define BASE_MEMORY_H

// scratch memory this is a list of blocks

// usage:
// - call the functions
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
function void         *_scratch_push(Scratch *scratch, u64 size, u64 alignment);
function void         *_scratch_push_zero(Scratch *scratch, u64 size, u64 alignment);
function ScratchMarker scratch_get_mark(Scratch *scratch);
function void          scratch_pop_to(ScratchMarker *marker);

// context usage
function ScratchMarker scratch_begin(Scratch *other_scratch);
function void          scratch_end(ScratchMarker marker);

// helpers
function void scratch_pop(Scratch *scratch, u64 amount);

// periodically
function void scratch_clear(Scratch *scratch);
function void scratch_trim(Scratch *scratch);

// clean up
function void scratch_free(Scratch *scratch);

// push helpers
#define scratch_push(scratch, type)                   (type *)_scratch_push(scratch, size_of(type), align_of(type))
#define scratch_push_zero(scratch, type)              (type *)_scratch_push_zero(scratch, size_of(type), align_of(type))
#define scratch_push_array(scratch, type, count)      (type *)_scratch_push(scratch, (count) * size_of(type), align_of(type))
#define scratch_push_array_zero(scratch, type, count) (type *)_scratch_push_zero(scratch, (count) * size_of(type), align_of(type))

#endif // BASE_MEMORY_H
