#ifndef STA_CO_H
#define STA_CO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>

/* Custom Atomic Spinlock Implementation */
typedef volatile long spinlock_t;
#define SPINLOCK_UNLOCKED 0

static inline void spinlock_lock(spinlock_t *lock) {
    while (InterlockedExchange(lock, 1) == 1) {
        YieldProcessor();
    }
}

static inline void spinlock_unlock(spinlock_t *lock) {
    InterlockedExchange(lock, 0);
}

static inline int sys_core_count(void)
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return (int)si.dwNumberOfProcessors;
}

/* 
 * Declares worker context structures, wrapper thread logic, 
 * and ends directly at the user function header.
 */
#define a_co(name, typename) \
typedef struct { \
    spinlock_t lock; \
    int current_i; \
    int max_i; \
    typename * p; \
} name##_shared_t; \
typedef struct { \
    int id; \
    int i; \
    typename * p; \
    name##_shared_t * shared; \
} name##_co_t; \
void name##_work(name##_co_t * co); \
static unsigned int __stdcall name##_worker_thread(void * arg) { \
    name##_co_t * co = (name##_co_t *)arg; \
    while (1) { \
        spinlock_lock(&co->shared->lock); \
        if (co->shared->current_i >= co->shared->max_i) { \
            spinlock_unlock(&co->shared->lock); \
            break; \
        } \
        co->i = co->shared->current_i++; \
        spinlock_unlock(&co->shared->lock); \
        name##_work(co); \
    } \
    free(co); \
    return 0; \
} \
void name##_work(name##_co_t * co)

/* 
 * Parallel loop execution macro across system CPU cores.
 */
#define for_co(name, p_ptr, max_iterations) \
do { \
    int co__threads = sys_core_count(); \
    HANDLE * co__handles = (HANDLE *)malloc(sizeof(HANDLE) * co__threads); \
    if (co__handles) { \
        name##_shared_t co__shared; \
        co__shared.lock = SPINLOCK_UNLOCKED; \
        co__shared.current_i = 0; \
        co__shared.max_i = (max_iterations); \
        co__shared.p = (p_ptr); \
        int co__spawned = 0; \
        for (int co__k = 0; co__k < co__threads; co__k++) { \
            name##_co_t * co__data = (name##_co_t *)malloc(sizeof(name##_co_t)); \
            if (co__data) { \
                co__data->id = co__k; \
                co__data->i = 0; \
                co__data->p = (p_ptr); \
                co__data->shared = &co__shared; \
                co__handles[co__spawned] = (HANDLE)_beginthreadex(0, 0, &name##_worker_thread, co__data, 0, 0); \
                if (co__handles[co__spawned]) co__spawned++; \
                else free(co__data); \
            } \
        } \
        if (co__spawned > 0) { \
            WaitForMultipleObjects(co__spawned, co__handles, TRUE, INFINITE); \
            for (int co__k = 0; co__k < co__spawned; co__k++) { \
                CloseHandle(co__handles[co__k]); \
            } \
        } \
        free(co__handles); \
    } \
} while(0)

#ifdef __cplusplus
}
#endif

#endif /* STA_CO_H */

// Project STA Design by Arshad Latti with help of Gemini 2.5 and implemented by Gemini 2.5