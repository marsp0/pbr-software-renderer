#include "thread_pool.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/syscall.h>

/********************
 *  Notes
 *
 ********************/

/********************/
/*      defines     */
/********************/

/********************/
/* static variables */
/********************/

static thread_local int64_t thread_id;

/********************/
/* static functions */
/********************/

static job_t* thread_pool_get_job(thread_pool_t* pool)
{
    job_t* job = NULL;

    if (pool->head == pool->tail)
    {
        return job;
    }

    job = &pool->jobs[pool->tail];

    pool->tail++;
    
    if (pool->tail == MAX_JOBS)
    {
        pool->tail = 0;
    }

    return job;
}

static int32_t worker(void* args)
{
    thread_id           = syscall(SYS_gettid);
    thread_pool_t* pool = (thread_pool_t*)args;
    job_t* job;

    while (!pool->stop)
    {
        mtx_lock(&pool->mtx);
        
        job = thread_pool_get_job(pool);

        if (!job)
        {
            cnd_wait(&pool->work_cnd, &pool->mtx);
        }

        mtx_unlock(&pool->mtx);

        if (pool->stop || !job)
        {
            continue;
        }
        
        job->func(job->args);
        
        mtx_lock(&pool->mtx);

        pool->count--;
        cnd_signal(&pool->done_cnd);

        mtx_unlock(&pool->mtx);
    }

    printf("[%ld] %s: exiting thread\n", thread_id, pool->name);

    return 0;
}

/********************/
/* public functions */
/********************/

thread_pool_t* thread_pool_new(const char* pool_name)
{
    thread_pool_t* pool = malloc(sizeof(thread_pool_t));

    pool->name          = pool_name;
    pool->stop          = false;
    pool->head          = 0;
    pool->tail          = 0;
    pool->count         = 0;

    mtx_init(&pool->mtx, mtx_plain);
    cnd_init(&pool->work_cnd);
    cnd_init(&pool->done_cnd);

    int32_t success;
    for (uint32_t i = 0; i < THREAD_COUNT; i++)
    {
        success = thrd_create(&pool->threads[i], worker, pool);
        assert(success == 0);
    }

    return pool;
}

void thread_pool_add(thread_pool_t* pool, thread_func_t func, void* args)
{
    mtx_lock(&pool->mtx);

    pool->jobs[pool->head].func = func;
    pool->jobs[pool->head].args = args;
    pool->head++;
    pool->count++;

    if (pool->head == MAX_JOBS)
    {
        pool->head = 0;
    }

    cnd_signal(&pool->work_cnd);
    mtx_unlock(&pool->mtx);
}

void thread_pool_wait(thread_pool_t* pool)
{
    mtx_lock(&pool->mtx);

    while(pool->count > 0)
    {
        cnd_wait(&pool->done_cnd, &pool->mtx);
    }

    mtx_unlock(&pool->mtx);
}

void thread_pool_free(thread_pool_t* pool)
{
    pool->stop = true;
    cnd_broadcast(&pool->work_cnd);

    int32_t success;

    for (uint32_t i = 0; i < THREAD_COUNT; i++)
    {
        thrd_join(pool->threads[i], &success);
        if (success != 0)
        {
            printf("%s thread join failed: %d", pool->name, success);
        }
    }

    mtx_destroy(&pool->mtx);
    cnd_destroy(&pool->work_cnd);
    cnd_destroy(&pool->done_cnd);
    free(pool);
}