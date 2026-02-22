#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// -----------------------------------------------------------------------------
// 1. "MULTI-CORE BY DEFAULT" ENVIRONMENT SETUP
// -----------------------------------------------------------------------------

// We use C11 thread-local variables so the Lane API is implicitly available 
// anywhere in the codebase without passing context structs around.
_Thread_local int ctx_lane_idx;
_Thread_local int ctx_lane_count;

// A custom barrier implementation for maximum cross-platform portability 
// (Acts as our LaneSync mechanism)
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int threshold;
    int count;
    int generation;
} custom_barrier_t;

custom_barrier_t global_barrier;

void barrier_init(custom_barrier_t *b, int count) {
    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->cond, NULL);
    b->threshold = count;
    b->count = count;
    b->generation = 0;
}

void barrier_wait(custom_barrier_t *b) {
    pthread_mutex_lock(&b->mutex);
    int gen = b->generation;
    if (--b->count == 0) {
        b->generation++;
        b->count = b->threshold;
        pthread_cond_broadcast(&b->cond);
    } else {
        while (gen == b->generation) {
            pthread_cond_wait(&b->cond, &b->mutex);
        }
    }
    pthread_mutex_unlock(&b->mutex);
}

// -----------------------------------------------------------------------------
// 2. THE MULTI-CORE API
// -----------------------------------------------------------------------------

int LaneIdx(void)   { return ctx_lane_idx; }
int LaneCount(void) { return ctx_lane_count; }
void LaneSync(void) { barrier_wait(&global_barrier); }

// -----------------------------------------------------------------------------
// 3. APPLICATION LOGIC ("WIDE" BY DEFAULT)
// -----------------------------------------------------------------------------

#define DATA_SIZE 20
int shared_data[DATA_SIZE];
int global_sum = 0;

// This function is "Wide". EVERY CPU core enters this function simultaneously.
void ProcessDataWide(void) {
    
    // NARROW EXECUTION: We only want ONE core to initialize the data.
    if (LaneIdx() == 0) {
        printf("[Lane 0] Initializing data... (Narrow Block)\n");
        for (int i = 0; i < DATA_SIZE; i++) {
            shared_data[i] = i + 1; // [1, 2, 3, ..., 20]
        }
    }

    // SYNC: No lane is allowed to proceed until Lane 0 finishes setup.
    LaneSync();

    // WIDE EXECUTION: Implicit Data Parallelism
    // Because all lanes are running this, they interleave the workload naturally.
    int local_sum = 0;
    for (int i = LaneIdx(); i < DATA_SIZE; i += LaneCount()) {
        printf("[Lane %d] processing index %d\n", LaneIdx(), i);
        shared_data[i] *= 2; // Simulate work
        local_sum += shared_data[i];
    }

    // NARROW / ATOMIC AGGREGATION: 
    // Safely combine our lane's local sum into the global sum
    __atomic_fetch_add(&global_sum, local_sum, __ATOMIC_SEQ_CST);

    // SYNC: Wait for all lanes to finish math before printing final result
    LaneSync();

    // NARROW EXECUTION: Final Output
    if (LaneIdx() == 0) {
        printf("\n[Lane 0] All processing complete. Final Sum = %d\n", global_sum);
    }
}

// -----------------------------------------------------------------------------
// 4. THREAD LAUNCHER
// -----------------------------------------------------------------------------

struct ThreadArg {
    int lane_idx;
    int lane_count;
};

void* ThreadEntry(void* arg) {
    struct ThreadArg* a = (struct ThreadArg*)arg;
    
    // Seed the thread-local context for this specific lane
    ctx_lane_idx = a->lane_idx;
    ctx_lane_count = a->lane_count;

    // Dive straight into the Wide execution model
    ProcessDataWide();

    return NULL;
}

int main(void) {
    int num_cores = 4; // In a real app, query OS for core count
    
    barrier_init(&global_barrier, num_cores);
    pthread_t threads[num_cores];
    struct ThreadArg args[num_cores];

    printf("Booting Multi-Core By Default environment with %d lanes...\n\n", num_cores);

    // Launch all lanes. From this point on, the system is fully parallelized.
    for (int i = 0; i < num_cores; i++) {
        args[i].lane_idx = i;
        args[i].lane_count = num_cores;
        pthread_create(&threads[i], NULL, ThreadEntry, &args[i]);
    }

    // Join threads on shutdown
    for (int i = 0; i < num_cores; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}