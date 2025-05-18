#ifndef _SDT_QUEUE_LIB_H
#define _SDT_QUEUE_LIB_H
#include <stdlib.h>

struct SDTQueueARR {
    void** arr;
    int tail;
    int head;
    int max_size;
    void (*free_func)(void*);
};
typedef struct SDTQueueARR SDTQueueARR;

static inline SDTQueueARR* sdt_create_arr_queue(void** arr, int max_size, void (*free_func)(void*)) {
    SDTQueueARR* _queue = (SDTQueueARR*)malloc(sizeof(SDTQueueARR));
    _queue->max_size = max_size;
    _queue->arr = arr;
    _queue->head = -1; _queue->tail = 0;
    _queue->free_func = free_func;
    return _queue;
}

static inline bool sdt_arr_queue_is_full(SDTQueueARR* queue) {
    return queue->tail == queue->max_size; 
}

static inline bool sdt_arr_queue_is_empty(SDTQueueARR* queue) {
    return queue->head == queue->tail - 1; 
}

static inline bool sdt_arr_enqueue(SDTQueueARR* queue, void* item) {
    if (sdt_arr_queue_is_full(queue)) return false;
    queue->arr[queue->tail++] = item;
    return true;
}

static inline bool sdt_arr_dequeue(SDTQueueARR* queue) {
    if (sdt_arr_queue_is_empty(queue)) return false;
    queue->head++;
    queue->free_func(queue->arr[queue->head]);
    queue->arr[queue->head] = NULL;
    return true;
}

static inline void* sdt_arr_peek(SDTQueueARR* queue) {
    if (sdt_arr_queue_is_empty(queue)) return NULL;
    return queue->arr[queue->head + 1]; 
}

static inline void sdt_destroy_arr_queue(SDTQueueARR* queue) {
    free(queue);
}

#endif
