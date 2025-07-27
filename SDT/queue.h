#ifndef _SDT_QUEUE_LIB_H
#define _SDT_QUEUE_LIB_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    void** arr;
    int head;
    int tail;
    int max_size;
    void (*free_func)(void*);
} SDTQueueARR;

static inline SDTQueueARR* sdt_create_arr_queue(void** arr, int max_size, void (*free_func)(void*)) {
    SDTQueueARR* q = (SDTQueueARR*)malloc(sizeof(SDTQueueARR));
    q->arr = arr;
    q->max_size = max_size;
    q->head = 0;
    q->tail = 0;
    q->free_func = free_func;
    return q;
}

static inline bool sdt_arr_queue_is_empty(SDTQueueARR* q) {
    return q->head == q->tail;
}

static inline bool sdt_arr_queue_is_full(SDTQueueARR* q) {
    return ((q->tail + 1) % q->max_size) == q->head;
}

static inline bool sdt_arr_enqueue(SDTQueueARR* q, void* item) {
    if (sdt_arr_queue_is_full(q)) return false;
    q->arr[q->tail] = item;
    q->tail = (q->tail + 1) % q->max_size;
    return true;
}

static inline bool sdt_arr_dequeue(SDTQueueARR* q) {
    if (sdt_arr_queue_is_empty(q)) return false;
    if (q->free_func && q->arr[q->head]) q->free_func(q->arr[q->head]);
    q->arr[q->head] = NULL;
    q->head = (q->head + 1) % q->max_size;
    return true;
}

static inline void* sdt_arr_peek(SDTQueueARR* q) {
    if (sdt_arr_queue_is_empty(q)) return NULL;
    return q->arr[q->head];
}

static inline int sdt_arr_get_length(SDTQueueARR* q) {
    if (q->tail >= q->head)
        return q->tail - q->head;
    else
        return q->max_size - q->head + q->tail;
}

static inline void sdt_destroy_arr_queue(SDTQueueARR* q) {
    free(q);
}

#endif
