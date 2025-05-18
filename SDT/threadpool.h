#include <cstdlib>
#include <stdlib.h>
#include <pthread.h>
#include "queue.h"

typedef struct {
    int thread_count;
    int worker_count;
    SDTQueueARR* work;
    pthread_mutex_t work_mutex;
    pthread_cond_t signal; // for workers
    pthread_cond_t stopped; // for workers
    int paused; // number of threads paused, so the calling thread can tell when work was paused
    bool pause;
    bool stop;
} SDTThreadpool;

typedef struct {
    void* (*work)(void* arg);
    void* arg;
} SDTJob;


static inline bool sdt_add_job(SDTThreadpool* tp, SDTJob* job) {
    if (tp->stop) return false;
    return sdt_arr_enqueue(tp->work, job);
}

static inline SDTJob* sdt_get_job(SDTThreadpool* tp) {
    if (tp->pause || tp->stop) return NULL;
    return (SDTJob*)sdt_arr_peek(tp->work);
}

static inline bool sdt_remove_latest_job(SDTThreadpool* tp) {
    return sdt_arr_dequeue(tp->work);
}


static inline void* _worker_thread(void *arg) {
    SDTThreadpool* tp = (SDTThreadpool*)arg;
return_after_pause:
    tp->paused--;
    while (1) {
        pthread_mutex_lock(&tp->work_mutex);
        while (sdt_arr_queue_is_empty(tp->work) && !tp->pause)
            pthread_cond_wait(&tp->signal, &tp->work_mutex);

        if (tp->pause)
            break;

        if (tp->stop) {
            pthread_mutex_unlock(&tp->work_mutex);
            return NULL;
        }

        SDTJob* work = sdt_get_job(tp);
        tp->worker_count++;
        pthread_mutex_unlock(&tp->work_mutex);
        if (work != NULL) {
            work->work(work->arg);
            sdt_remove_latest_job(tp);
        }
        pthread_mutex_lock(&tp->work_mutex);
        tp->worker_count--;
        if (!tp->stop && tp->worker_count == 0 && sdt_arr_queue_is_empty(tp->work))
            pthread_cond_signal(&tp->stopped);
        pthread_mutex_unlock(&tp->work_mutex);
    }
    pthread_mutex_unlock(&tp->work_mutex);
    tp->paused++;
    while (tp->pause)
        pthread_cond_wait(&tp->signal, &tp->work_mutex);
    goto return_after_pause;
}

static inline SDTThreadpool* sdt_create_threadpool(int thread_count, int max_work) {
    SDTThreadpool* tp = (SDTThreadpool*)malloc(sizeof(SDTThreadpool));
    void** jobs = (void**)malloc(sizeof(SDTJob*) * max_work);
    tp->work = sdt_create_arr_queue(jobs, max_work, free);
    tp->work_mutex = PTHREAD_MUTEX_INITIALIZER;
    tp->signal = PTHREAD_COND_INITIALIZER;
    tp->pause = false;
    tp->stop = false;
    tp->paused = 0;
    pthread_t thread;
    for (int i = 0; i < thread_count; ++i) {
        pthread_create(&thread, NULL, _worker_thread, tp);
        pthread_detach(thread);
    }
    return tp;
}

static inline void sdt_destroy_threadpool(SDTThreadpool* tp) {
    pthread_mutex_lock(&tp->work_mutex);
    while (!sdt_arr_queue_is_empty(tp->work)) {
        sdt_arr_dequeue(tp->work);
    }
    tp->stop = true;
    pthread_cond_broadcast(&tp->signal);
    pthread_mutex_lock(&tp->work_mutex);
    while (1) {
        if (sdt_arr_queue_is_empty(tp->work) || (!tp->stop && tp->worker_count != 0)) {
            pthread_cond_wait(&tp->stopped, &tp->work_mutex);
        } else {
            break;
        }
    }
    pthread_mutex_unlock(&tp->work_mutex);
    pthread_mutex_destroy(&tp->work_mutex);
    pthread_cond_destroy(&tp->signal);

    free(tp);
}

static inline bool sdt_threadpool_add_work(SDTThreadpool* tp, void*(*func)(void* arg), void* arg) {
    SDTJob* job = (SDTJob*)malloc(sizeof(SDTJob));
    job->work = func;
    job->arg = arg;

    pthread_mutex_lock(&tp->work_mutex);
    bool ret = sdt_add_job(tp, job);
    if (ret) pthread_cond_broadcast(&tp->signal);
    pthread_mutex_unlock(&tp->work_mutex);
    
    return ret;
}

// worker function
