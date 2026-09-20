/*
 * STA_CO_WORKERS - Modern Win32 Pausable Thread Pool
 * 
 * Version: 2.1.1
 * Platform: Microsoft Windows (Win32 - MSVC & GCC/MinGW Compatible)
 */

#ifndef STA_CO_WORKERS_H
#define STA_CO_WORKERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include <process.h>
#include <stdlib.h>

#if defined(_MSC_VER)
    #define STA_ALIGN64 __declspec(align(64))
#elif defined(__GNUC__) || defined(__clang__)
    #define STA_ALIGN64 __attribute__((aligned(64)))
#else
    #define STA_ALIGN64
#endif

typedef struct sta_coworkers_t sta_coworkers_t;

sta_coworkers_t *co_workers_new(void);
void co_workers_for(
    sta_coworkers_t *coworkers,
    void (*function)(void *custom, int index, int count),
    void *custom
);
void co_worker_delete(sta_coworkers_t *coworkers);
int co_worker_count(sta_coworkers_t *coworkers);

#ifdef STA_CO_WORKERS_IMPLEMENTATION

typedef void (*sta_worker_fn)(void *custom, int index, int count);

typedef struct STA_ALIGN64 sta_worker_t {
    int index;
    int count;
    HANDLE thread;
    volatile LONG state; /* 0 = Idle/Paused, 1 = Running, 2 = Stop */
    sta_worker_fn function;
    void *custom;
} sta_worker_t;

struct sta_coworkers_t {
    int count;
    sta_worker_t *workers;
};

static unsigned int __stdcall sta_worker_loop(void *arg) {
    sta_worker_t *worker = (sta_worker_t *)arg;

    while (1) {
        LONG current_state = worker->state;

        /* Pause thread completely with zero CPU consumption until state != 0 */
        while (current_state == 0) {
            LONG compare_value = 0;
            WaitOnAddress(&worker->state, &compare_value, sizeof(LONG), INFINITE);
            current_state = worker->state;
        }

        if (current_state == 2) {
            break; /* Exit thread loop */
        }

        /* Process assigned work chunk */
        if (worker->function) {
            worker->function(worker->custom, worker->index, worker->count);
        }

        /* Transition back to Idle (0) and notify main thread */
        InterlockedExchange(&worker->state, 0);
        WakeByAddressAll((PVOID)&worker->state);
    }

    return 0;
}

sta_coworkers_t *co_workers_new(void) {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    int cores = (int)si.dwNumberOfProcessors;

    /* Single core machines bypass pool setup */
    if (cores <= 1) {
        return NULL;
    }

    sta_coworkers_t *group = (sta_coworkers_t *)calloc(1, sizeof(sta_coworkers_t));
    if (!group) return NULL;

    group->count = cores;
    group->workers = (sta_worker_t *)_aligned_malloc(sizeof(sta_worker_t) * cores, 64);
    if (!group->workers) {
        free(group);
        return NULL;
    }
    memset(group->workers, 0, sizeof(sta_worker_t) * cores);

    /* Initialize worker threads */
    for (int i = 0; i < cores - 1; i++) {
        sta_worker_t *worker = &group->workers[i];
        worker->index = i;
        worker->count = cores;
        worker->state = 0; /* Starts paused */

        worker->thread = (HANDLE)_beginthreadex(
            NULL, 0, sta_worker_loop, worker, 0, NULL
        );

        if (!worker->thread) {
            co_worker_delete(group);
            return NULL;
        }
    }

    return group;
}

void co_workers_for(
    sta_coworkers_t *coworkers,
    void (*function)(void *custom, int index, int count),
    void *custom
) {
    if (!function) return;

    if (!coworkers) {
        function(custom, 0, 1);
        return;
    }

    int total_count = coworkers->count;
    int worker_threads = total_count - 1;

    /* Resume paused worker threads by waking their state address */
    for (int i = 0; i < worker_threads; i++) {
        sta_worker_t *worker = &coworkers->workers[i];
        worker->function = function;
        worker->custom = custom;
        InterlockedExchange(&worker->state, 1);
        WakeByAddressSingle((PVOID)&worker->state);
    }

    /* Main thread processes the last chunk */
    function(custom, worker_threads, total_count);

    /* Wait for all background worker threads to enter idle state (0) */
    for (int i = 0; i < worker_threads; i++) {
        sta_worker_t *worker = &coworkers->workers[i];
        while (worker->state != 0) {
            LONG compare_value = 1;
            WaitOnAddress(&worker->state, &compare_value, sizeof(LONG), INFINITE);
        }
    }
}

void co_worker_delete(sta_coworkers_t *coworkers) {
    if (!coworkers) return;

    int worker_threads = coworkers->count - 1;

    if (coworkers->workers) {
        for (int i = 0; i < worker_threads; i++) {
            sta_worker_t *worker = &coworkers->workers[i];
            if (worker->thread) {
                InterlockedExchange(&worker->state, 2); /* Signal shutdown */
                WakeByAddressSingle((PVOID)&worker->state);
                WaitForSingleObject(worker->thread, INFINITE);
                CloseHandle(worker->thread);
            }
        }
        _aligned_free(coworkers->workers);
    }

    free(coworkers);
}

int co_worker_count(sta_coworkers_t *coworkers) {
    return coworkers ? coworkers->count : 1;
}

#endif /* STA_CO_WORKERS_IMPLEMENTATION */

#ifdef __cplusplus
}
#endif

#endif /* STA_CO_WORKERS_H */

// sta_co_workers Design by Arshad Latti with help of Gemini 2.5 Flash and implemented by Gemini 2.5 Flash