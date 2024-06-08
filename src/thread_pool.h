#pragma once

// system headers
#include <threads.h>
#include <stdint.h>

// app headers
#include "atomic_types.h"
#include "renderer_constants.h"

#define MAX_JOBS 100

typedef void (*thread_func_t)(void* args);

typedef struct
{
    thread_func_t   func;
    void*           args;
} job_t;

typedef struct
{
    mtx_t           mtx;
    cnd_t           work_cnd;
    cnd_t           done_cnd;

    job_t           jobs[MAX_JOBS];
    uint32_t        count;
    uint32_t        head;
    uint32_t        tail;

    thrd_t          threads[THREAD_COUNT];

    atomic_bool_t   stop;
    const char*     name;
} thread_pool_t;

thread_pool_t*  thread_pool_new(const char* pool_name);
void            thread_pool_add(thread_pool_t* pool, thread_func_t func, void* args);
void            thread_pool_wait(thread_pool_t* pool);
void            thread_pool_free(thread_pool_t* pool);