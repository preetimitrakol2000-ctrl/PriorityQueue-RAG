#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUEUE_SIZE 10

typedef struct {
    int tracking_priority_rank; // Lower values represent higher processing importance
    char evaluation_statement[128];
} ScheduledQuery;

typedef struct {
    ScheduledQuery processing_array[MAX_QUEUE_SIZE];
    int size;
} EngineHeap;

#ifdef _WIN32
    __declspec(dllexport) EngineHeap* init_scheduler();
    __declspec(dllexport) void enqueue_request(EngineHeap* eh, int rank, const char* stmt);
    __declspec(dllexport) const char* dequeue_next_target(EngineHeap* eh);
#endif

EngineHeap* init_scheduler() {
    EngineHeap* eh = (EngineHeap*)malloc(sizeof(EngineHeap));
    eh->size = 0;
    return eh;
}

void enqueue_request(EngineHeap* eh, int rank, const char* stmt) {
    if (eh->size >= MAX_QUEUE_SIZE) return;

    int idx = eh->size;
    eh->processing_array[idx].tracking_priority_rank = rank;
    strncpy(eh->processing_array[idx].evaluation_statement, stmt, sizeof(eh->processing_array[idx].evaluation_statement) - 1);
    eh->size++;

    // Standard min-heap up-heap bubbling routines
    while (idx > 0 && eh->processing_array[idx].tracking_priority_rank < eh->processing_array[(idx - 1) / 2].tracking_priority_rank) {
        ScheduledQuery temp = eh->processing_array[idx];
        eh->processing_array[idx] = eh->processing_array[(idx - 1) / 2];
        eh->processing_array[(idx - 1) / 2] = temp;
        idx = (idx - 1) / 2;
    }
}

const char* dequeue_next_target(EngineHeap* eh) {
    if (eh->size == 0) return "QUEUE_EMPTY";

    // Allocate memory loop spaces safely to avoid local reference return issues
    static char extraction_buffer[128];
    strncpy(extraction_buffer, eh->processing_array[0].evaluation_statement, sizeof(extraction_buffer) - 1);

    eh->processing_array[0] = eh->processing_array[eh->size - 1];
    eh->size--;

    // Standard min-heap down-heap shift traversals
    int idx = 0;
    while (idx * 2 + 1 < eh->size) {
        int left_child = idx * 2 + 1;
        int right_child = idx * 2 + 2;
        int smallest_target = left_child;

        if (right_child < eh->size && eh->processing_array[right_child].tracking_priority_rank < eh->processing_array[left_child].tracking_priority_rank) {
            smallest_target = right_child;
        }
        if (eh->processing_array[idx].tracking_priority_rank <= eh->processing_array[smallest_target].tracking_priority_rank) break;

        ScheduledQuery temp = eh->processing_array[idx];
        eh->processing_array[idx] = eh->processing_array[smallest_target];
        eh->processing_array[smallest_target] = temp;
        idx = smallest_target;
    }

    return extraction_buffer;
}
