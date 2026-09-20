#ifndef SYS_TIME_H
#define SYS_TIME_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --- Core Time & Sleep API --- */

/**
 * @brief Sleep for a given number of milliseconds.
 * @param ms Duration in milliseconds.
 */
void sys_sleep_ms(uint32_t ms);

/**
 * @brief Sleep for a given number of nanoseconds.
 * @param ns Duration in nanoseconds.
 */
void sys_sleep_ns(uint64_t ns);

/**
 * @brief High-precision hybrid sleep/spin delay for sub-millisecond accuracy.
 * @param ns Target delay in nanoseconds.
 */
void sys_delay_precise_ns(uint64_t ns);

/**
 * @brief Get system uptime tick count in milliseconds.
 * @return Monotonic time in milliseconds.
 */
uint64_t sys_ticks_count(void);

/**
 * @brief Get system uptime tick count in nanoseconds.
 * @return Monotonic time in nanoseconds.
 */
uint64_t sys_ticks_ns(void);

/* --- Stateless Performance Counter API --- */

/**
 * @brief Captures the current performance tick timestamp in nanoseconds.
 * @return Current timestamp in nanoseconds.
 */
static inline uint64_t performance_counter_start(void) {
    return sys_ticks_ns();
}

/**
 * @brief Calculates elapsed time in nanoseconds since start_ticks.
 * @param start_ticks The timestamp returned by performance_counter_start().
 * @return Elapsed nanoseconds.
 */
static inline uint64_t performance_counter_stop_ns(uint64_t start_ticks) {
    uint64_t now = sys_ticks_ns();
    return (now >= start_ticks) ? (now - start_ticks) : 0;
}

/**
 * @brief Calculates elapsed time in milliseconds since start_ticks.
 * @param start_ticks The timestamp returned by performance_counter_start().
 * @return Elapsed milliseconds.
 */
static inline uint32_t performance_counter_stop_ms(uint64_t start_ticks) {
    return (uint32_t)(performance_counter_stop_ns(start_ticks) / 1000000ULL);
}

#ifdef __cplusplus
}
#endif

#endif // SYS_TIME_H

/* ========================== IMPLEMENTATION ========================== */

#ifdef SYS_TIME_IMPLEMENTATION
#ifndef SYS_TIME_IMPLEMENTATION_INCLUDED
#define SYS_TIME_IMPLEMENTATION_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>

    #ifndef CREATE_WAITABLE_TIMER_HIGH_RESOLUTION
        #define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002
    #endif

    typedef HANDLE (WINAPI *pfn_CreateWaitableTimerExW)(
        LPSECURITY_ATTRIBUTES lpTimerAttributes,
        LPCWSTR lpTimerName,
        DWORD dwFlags,
        DWORD dwDesiredAccess
    );

    static double sys_internal_get_freq_scale(void) {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        return 1000000000.0 / (double)freq.QuadPart;
    }

    uint64_t sys_ticks_ns(void) {
        static double scale = 0.0;
        if (scale == 0.0) {
            scale = sys_internal_get_freq_scale();
        }
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        return (uint64_t)((double)counter.QuadPart * scale);
    }

    void sys_sleep_ms(uint32_t ms) {
        Sleep((DWORD)ms);
    }

    void sys_sleep_ns(uint64_t ns) {
        if (ns == 0) return;

        static pfn_CreateWaitableTimerExW pCreateWaitableTimerExW = NULL;
        static int checked_fn = 0;

        if (!checked_fn) {
            HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
            if (hKernel32) {
                pCreateWaitableTimerExW = (pfn_CreateWaitableTimerExW)GetProcAddress(hKernel32, "CreateWaitableTimerExW");
            }
            checked_fn = 1;
        }

        HANDLE timer = NULL;
        if (pCreateWaitableTimerExW) {
            timer = pCreateWaitableTimerExW(NULL, NULL, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
        }

        if (timer) {
            LARGE_INTEGER due_time;
            due_time.QuadPart = -(LONGLONG)(ns / 100ULL);
            if (due_time.QuadPart == 0) due_time.QuadPart = -1;
            SetWaitableTimer(timer, &due_time, 0, NULL, NULL, 0);
            WaitForSingleObject(timer, INFINITE);
            CloseHandle(timer);
        } else {
            HANDLE std_timer = CreateWaitableTimerW(NULL, TRUE, NULL);
            if (std_timer) {
                LARGE_INTEGER due_time;
                due_time.QuadPart = -(LONGLONG)(ns / 100ULL);
                if (due_time.QuadPart == 0) due_time.QuadPart = -1;
                SetWaitableTimer(std_timer, &due_time, 0, NULL, NULL, 0);
                WaitForSingleObject(std_timer, INFINITE);
                CloseHandle(std_timer);
            } else {
                uint32_t ms = (uint32_t)(ns / 1000000ULL);
                Sleep(ms > 0 ? ms : 1);
            }
        }
    }

#elif defined(__unix__) || defined(__APPLE__) || defined(__linux__)
    #include <time.h>
    #include <unistd.h>

    uint64_t sys_ticks_ns(void) {
        struct timespec ts;
#if defined(CLOCK_MONOTONIC_RAW)
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
#else
        clock_gettime(CLOCK_MONOTONIC, &ts);
#endif
        return ((uint64_t)ts.tv_sec * 1000000000ULL) + (uint64_t)ts.tv_nsec;
    }

    void sys_sleep_ms(uint32_t ms) {
        struct timespec ts;
        ts.tv_sec = ms / 1000;
        ts.tv_nsec = (ms % 1000) * 1000000L;
        nanosleep(&ts, NULL);
    }

    void sys_sleep_ns(uint64_t ns) {
        struct timespec ts;
        ts.tv_sec = ns / 1000000000ULL;
        ts.tv_nsec = ns % 1000000000ULL;
        nanosleep(&ts, NULL);
    }

#else
    #error "Unsupported platform"
#endif

uint64_t sys_ticks_count(void) {
    return sys_ticks_ns() / 1000000ULL;
}

void sys_delay_precise_ns(uint64_t ns) {
    uint64_t start = sys_ticks_ns();
    // Yield/Sleep coarse time if long enough (> 2ms)
    if (ns > 2000000ULL) {
        sys_sleep_ms((uint32_t)((ns - 1000000ULL) / 1000000ULL));
    }
    // Busy-wait remaining duration using architecture pause hint
    while ((sys_ticks_ns() - start) < ns) {
#if defined(_MSC_VER)
        YieldProcessor();
#elif defined(__GNUC__) || defined(__clang__)
    #if defined(__i386__) || defined(__x86_64__)
        __asm__ __volatile__("pause" ::: "memory");
    #elif defined(__aarch64__) || defined(__arm__)
        __asm__ __volatile__("yield" ::: "memory");
    #endif
#endif
    }
}

#ifdef __cplusplus
}
#endif

#endif // SYS_TIME_IMPLEMENTATION_INCLUDED
#endif // SYS_TIME_IMPLEMENTATION
// sys_time Design by Arshad Latti with help of Gemini and implemented by Gemini