#ifndef SYS_THREAD_H
#define SYS_THREAD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --- Thread Data Types --- */

typedef void (*sys_thread_f)(void * p);

#if defined(_WIN32) || defined(_WIN64)
    typedef void * sys_thread_handle_t;
    typedef uint32_t sys_thread_id_t;
#elif defined(__unix__) || defined(__APPLE__) || defined(__linux__)
    #include <pthread.h>
    typedef pthread_t sys_thread_handle_t;
    typedef uint64_t sys_thread_id_t;
#else
    #error "Unsupported platform"
#endif

/* --- Core Thread API --- */

/**
 * @brief Spawns a fire-and-forget detached thread.
 * @param f Function pointer to execute.
 * @param p Pointer to user data argument.
 * @return 0 on success, non-zero on failure.
 */
int sys_thread(sys_thread_f f, void * p);

/**
 * @brief Spawns a joinable thread.
 * @param out_handle Pointer to store the created thread handle.
 * @param f Function pointer to execute.
 * @param p Pointer to user data argument.
 * @return 0 on success, non-zero on failure.
 */
int sys_thread_joinable(sys_thread_handle_t * out_handle, sys_thread_f f, void * p);

/**
 * @brief Waits for a joinable thread to terminate and cleans up resources.
 * @param handle Thread handle returned by sys_thread_joinable.
 * @return 0 on success, non-zero on failure.
 */
int sys_thread_join(sys_thread_handle_t handle);

/**
 * @brief Yields execution of the current thread to other threads.
 */
void sys_thread_yield(void);

/**
 * @brief Retrieves the current thread's unique identifier.
 * @return Thread ID.
 */
sys_thread_id_t sys_thread_get_id(void);

#ifdef __cplusplus
}
#endif

#endif // SYS_THREAD_H

/* ========================== IMPLEMENTATION ========================== */

#ifdef SYS_THREAD_IMPLEMENTATION
#ifndef SYS_THREAD_IMPLEMENTATION_INCLUDED
#define SYS_THREAD_IMPLEMENTATION_INCLUDED

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <process.h>

    typedef struct {
        sys_thread_f func;
        void * arg;
    } sys_internal_thread_arg_t;

    static void __cdecl sys_internal_win_entry(void * p) {
        sys_internal_thread_arg_t * targ = (sys_internal_thread_arg_t *)p;
        sys_thread_f fn = targ->func;
        void * user_arg = targ->arg;
        free(targ);

        fn(user_arg);
    }

    static DWORD WINAPI sys_internal_win_joinable_entry(LPVOID p) {
        sys_internal_thread_arg_t * targ = (sys_internal_thread_arg_t *)p;
        sys_thread_f fn = targ->func;
        void * user_arg = targ->arg;
        free(targ);

        fn(user_arg);
        return 0;
    }

    int sys_thread(sys_thread_f f, void * p) {
        if (!f) return -1;

        sys_internal_thread_arg_t * targ = (sys_internal_thread_arg_t *)malloc(sizeof(sys_internal_thread_arg_t));
        if (!targ) return -1;

        targ->func = f;
        targ->arg = p;

        uintptr_t res = _beginthread(sys_internal_win_entry, 0, targ);
        if (res == (uintptr_t)-1L || res == 0) {
            free(targ);
            return -1;
        }

        return 0;
    }

    int sys_thread_joinable(sys_thread_handle_t * out_handle, sys_thread_f f, void * p) {
        if (!out_handle || !f) return -1;

        sys_internal_thread_arg_t * targ = (sys_internal_thread_arg_t *)malloc(sizeof(sys_internal_thread_arg_t));
        if (!targ) return -1;

        targ->func = f;
        targ->arg = p;

        HANDLE hThread = CreateThread(NULL, 0, sys_internal_win_joinable_entry, targ, 0, NULL);
        if (!hThread) {
            free(targ);
            return -1;
        }

        *out_handle = (sys_thread_handle_t)hThread;
        return 0;
    }

    int sys_thread_join(sys_thread_handle_t handle) {
        if (!handle) return -1;
        
        DWORD res = WaitForSingleObject((HANDLE)handle, INFINITE);
        CloseHandle((HANDLE)handle);
        return (res == WAIT_OBJECT_0) ? 0 : -1;
    }

    void sys_thread_yield(void) {
        Sleep(0);
    }

    sys_thread_id_t sys_thread_get_id(void) {
        return (sys_thread_id_t)GetCurrentThreadId();
    }

#elif defined(__unix__) || defined(__APPLE__) || defined(__linux__)
    #include <pthread.h>
    #include <sched.h>

    typedef struct {
        sys_thread_f func;
        void * arg;
    } sys_internal_thread_arg_t;

    static void * sys_internal_posix_fire_and_forget_entry(void * p) {
        sys_internal_thread_arg_t * targ = (sys_internal_thread_arg_t *)p;
        sys_thread_f fn = targ->func;
        void * user_arg = targ->arg;

        pthread_detach(pthread_self());
        free(targ);

        fn(user_arg);
        return NULL;
    }

    static void * sys_internal_posix_joinable_entry(void * p) {
        sys_internal_thread_arg_t * targ = (sys_internal_thread_arg_t *)p;
        sys_thread_f fn = targ->func;
        void * user_arg = targ->arg;
        free(targ);

        fn(user_arg);
        return NULL;
    }

    int sys_thread(sys_thread_f f, void * p) {
        if (!f) return -1;

        sys_internal_thread_arg_t * targ = (sys_internal_thread_arg_t *)malloc(sizeof(sys_internal_thread_arg_t));
        if (!targ) return -1;

        targ->func = f;
        targ->arg = p;

        pthread_t thread;
        if (pthread_create(&thread, NULL, sys_internal_posix_fire_and_forget_entry, targ) != 0) {
            free(targ);
            return -1;
        }

        return 0;
    }

    int sys_thread_joinable(sys_thread_handle_t * out_handle, sys_thread_f f, void * p) {
        if (!out_handle || !f) return -1;

        sys_internal_thread_arg_t * targ = (sys_internal_thread_arg_t *)malloc(sizeof(sys_internal_thread_arg_t));
        if (!targ) return -1;

        targ->func = f;
        targ->arg = p;

        pthread_t thread;
        if (pthread_create(&thread, NULL, sys_internal_posix_joinable_entry, targ) != 0) {
            free(targ);
            return -1;
        }

        *out_handle = (sys_thread_handle_t)thread;
        return 0;
    }

    int sys_thread_join(sys_thread_handle_t handle) {
        return (pthread_join((pthread_t)handle, NULL) == 0) ? 0 : -1;
    }

    void sys_thread_yield(void) {
        sched_yield();
    }

    sys_thread_id_t sys_thread_get_id(void) {
        return (sys_thread_id_t)pthread_self();
    }

#endif

#ifdef __cplusplus
}
#endif

#endif // SYS_THREAD_IMPLEMENTATION_INCLUDED
#endif // SYS_THREAD_IMPLEMENTATION

// A Project Design by Arshad Latti with help of Gemini 2.5 and implemented by Gemini 2.5