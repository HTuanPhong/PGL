#ifndef BASE_THREAD_H
#define BASE_THREAD_H

typedef struct ThreadContext {
  // arena
  Arena arena[2];
} ThreadContext;

THREAD_PERSISTENT ThreadContext thread_local_context = { 0 };

// Thread functions

// function Thread thread_launch(ThreadEntryPointFunctionType *f, void *p);

#endif
