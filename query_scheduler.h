#ifndef QUERY_SCHEDULER_H
#define QUERY_SCHEDULER_H

typedef struct ScheduledQuery ScheduledQuery;
typedef struct EngineHeap EngineHeap;
EngineHeap* init_scheduler();
void enqueue_request(EngineHeap* eh, int rank, const char* stmt);
const char* dequeue_next_target(EngineHeap* eh);

#endif
