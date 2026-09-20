#ifndef SYS_CLOCK_H
#define SYS_CLOCK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================= */
/* --- Data Structures ---                                                   */
/* ========================================================================= */

/**
 * @brief Represents broken-down calendar time.
 */
typedef struct sys_clock {
    int32_t year;          /**< Year (e.g., 2026) */
    int32_t month;         /**< Month (1 - 12) */
    int32_t day;           /**< Day of the month (1 - 31) */
    int32_t hour;          /**< Hours (0 - 23) */
    int32_t minute;        /**< Minutes (0 - 59) */
    int32_t second;        /**< Seconds (0 - 59) */
    int32_t millisecond;   /**< Milliseconds (0 - 999) */
    int32_t day_of_week;   /**< Day of week (0 = Sunday, 6 = Saturday) */
    int32_t day_of_year;   /**< Day of year (1 - 366) */
    int32_t is_dst;        /**< Daylight Saving Time flag (>0 DST, 0 No DST, <0 Unknown) */
    int32_t utc_offset_sec;/**< Offset from UTC in seconds (e.g., +18000 for UTC+5) */
} sys_clock_t;

/* ========================================================================= */
/* --- Core Clock & Epoch API ---                                            */
/* ========================================================================= */

/**
 * @brief Gets the current system time as a 64-bit epoch representation.
 * @return Epoch timestamp in seconds (UTC).
 */
uint64_t sys_epoch(void);

/**
 * @brief Retrieves current system clock/calendar time.
 * @param out_clock Pointer to output sys_clock_t struct.
 * @param is_out_utc Non-zero for UTC time, 0 for local time.
 * @return 0 on success, non-zero on error (e.g., NULL pointer).
 */
int sys_clock_now(sys_clock_t * out_clock, int is_out_utc);

/* ========================================================================= */
/* --- Conversion API ---                                                    */
/* ========================================================================= */

/**
 * @brief Converts an epoch timestamp to a broken-down clock structure.
 * @param epoch Seconds since Unix epoch.
 * @param out_clock Pointer to target sys_clock_t struct.
 * @param is_out_utc Non-zero for UTC output, 0 for local time output.
 * @return 0 on success, non-zero on error.
 */
int sys_clock_from_epoch(uint64_t epoch, sys_clock_t * out_clock, int is_out_utc);

/**
 * @brief Converts a broken-down clock structure back to an epoch timestamp.
 * @note The 'millisecond' field is explicitly ignored during calculation as 
 *       the returned epoch represents whole seconds elapsed since Unix epoch.
 * @param clock Pointer to source sys_clock_t struct.
 * @param is_input_utc Non-zero if date is in UTC, 0 if in local time.
 * @param out_epoch Pointer to store output epoch timestamp (in seconds).
 * @return 0 on success, non-zero on error.
 */
int sys_clock_to_epoch(const sys_clock_t * clock, int is_input_utc, uint64_t * out_epoch);

#ifdef __cplusplus
}
#endif

#endif /* SYS_CLOCK_H */

/* ========================================================================= */
/* --- IMPLEMENTATION ---                                                    */
/* ========================================================================= */

#ifdef SYS_CLOCK_IMPLEMENTATION
#ifndef SYS_CLOCK_IMPLEMENTATION_INCLUDED
#define SYS_CLOCK_IMPLEMENTATION_INCLUDED

#include <time.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>

    /* Epoch shift constant: 100-ns intervals between Jan 1, 1601 and Jan 1, 1970 */
    #define SYS_INTERNAL_EPOCH_DIFF 116444736000000000ULL

    uint64_t sys_epoch(void) {
        FILETIME ft;
        ULARGE_INTEGER uli;

        GetSystemTimeAsFileTime(&ft);
        uli.LowPart = ft.dwLowDateTime;
        uli.HighPart = ft.dwHighDateTime;

        if (uli.QuadPart < SYS_INTERNAL_EPOCH_DIFF) {
            return 0;
        }

        return (uint64_t)((uli.QuadPart - SYS_INTERNAL_EPOCH_DIFF) / 10000000ULL);
    }

    int sys_clock_now(sys_clock_t * out_clock, int is_out_utc) {
        SYSTEMTIME st;
        TIME_ZONE_INFORMATION tzi;
        DWORD tz_res;

        if (!out_clock) {
            return -1;
        }

        memset(out_clock, 0, sizeof(sys_clock_t));

        if (is_out_utc) {
            GetSystemTime(&st);
            out_clock->utc_offset_sec = 0;
            out_clock->is_dst = 0;
        } else {
            GetLocalTime(&st);
            tz_res = GetTimeZoneInformation(&tzi);
            if (tz_res == TIME_ZONE_ID_INVALID) {
                return -1;
            }

            /* Calculate UTC offset in seconds */
            if (tz_res == TIME_ZONE_ID_DAYLIGHT) {
                out_clock->utc_offset_sec = -(int32_t)(tzi.Bias + tzi.DaylightBias) * 60;
                out_clock->is_dst = 1;
            } else {
                out_clock->utc_offset_sec = -(int32_t)(tzi.Bias + tzi.StandardBias) * 60;
                out_clock->is_dst = 0;
            }
        }

        out_clock->year = (int32_t)st.wYear;
        out_clock->month = (int32_t)st.wMonth;
        out_clock->day = (int32_t)st.wDay;
        out_clock->hour = (int32_t)st.wHour;
        out_clock->minute = (int32_t)st.wMinute;
        out_clock->second = (int32_t)st.wSecond;
        out_clock->millisecond = (int32_t)st.wMilliseconds;
        out_clock->day_of_week = (int32_t)st.wDayOfWeek;

        /* Calculate day of year */
        {
            struct tm t;
            memset(&t, 0, sizeof(struct tm));
            t.tm_year = out_clock->year - 1900;
            t.tm_mon = out_clock->month - 1;
            t.tm_mday = out_clock->day;
            if (mktime(&t) != (time_t)-1) {
                out_clock->day_of_year = t.tm_yday + 1;
            } else {
                out_clock->day_of_year = 0;
            }
        }

        return 0;
    }

#elif defined(__unix__) || defined(__APPLE__) || defined(__linux__)
    #include <sys/time.h>

    uint64_t sys_epoch(void) {
        struct timespec ts;
        if (clock_gettime(CLOCK_REALTIME, &ts) != 0) {
            return 0;
        }
        return (uint64_t)ts.tv_sec;
    }

    int sys_clock_now(sys_clock_t * out_clock, int is_out_utc) {
        struct timespec ts;
        struct tm tm_buf;
        struct tm * res_tm;
        time_t sec;

        if (!out_clock) {
            return -1;
        }

        if (clock_gettime(CLOCK_REALTIME, &ts) != 0) {
            return -1;
        }

        sec = ts.tv_sec;
        memset(out_clock, 0, sizeof(sys_clock_t));

        if (is_out_utc) {
            res_tm = gmtime_r(&sec, &tm_buf);
            if (!res_tm) {
                return -1;
            }
            out_clock->utc_offset_sec = 0;
            out_clock->is_dst = 0;
        } else {
            res_tm = localtime_r(&sec, &tm_buf);
            if (!res_tm) {
                return -1;
            }
            out_clock->utc_offset_sec = (int32_t)tm_buf.tm_gmtoff;
            out_clock->is_dst = tm_buf.tm_isdst;
        }

        out_clock->year = (int32_t)tm_buf.tm_year + 1900;
        out_clock->month = (int32_t)tm_buf.tm_mon + 1;
        out_clock->day = (int32_t)tm_buf.tm_mday;
        out_clock->hour = (int32_t)tm_buf.tm_hour;
        out_clock->minute = (int32_t)tm_buf.tm_min;
        out_clock->second = (int32_t)tm_buf.tm_sec;
        out_clock->millisecond = (int32_t)(ts.tv_nsec / 1000000L);
        out_clock->day_of_week = (int32_t)tm_buf.tm_wday;
        out_clock->day_of_year = (int32_t)tm_buf.tm_yday + 1;

        return 0;
    }

#else
    #error "Unsupported platform"
#endif

int sys_clock_from_epoch(uint64_t epoch, sys_clock_t * out_clock, int is_out_utc) {
    time_t sec;
    struct tm tm_buf;
    struct tm * res_tm;

    if (!out_clock) {
        return -1;
    }

    sec = (time_t)epoch;
    memset(out_clock, 0, sizeof(sys_clock_t));

#if defined(_WIN32) || defined(_WIN64)
    if (is_out_utc) {
        if (gmtime_s(&tm_buf, &sec) != 0) {
            return -1;
        }
        res_tm = &tm_buf;
        out_clock->utc_offset_sec = 0;
        out_clock->is_dst = 0;
    } else {
        if (localtime_s(&tm_buf, &sec) != 0) {
            return -1;
        }
        res_tm = &tm_buf;
        {
            TIME_ZONE_INFORMATION tzi;
            DWORD tz_res = GetTimeZoneInformation(&tzi);
            if (tz_res != TIME_ZONE_ID_INVALID) {
                if (tz_res == TIME_ZONE_ID_DAYLIGHT) {
                    out_clock->utc_offset_sec = -(int32_t)(tzi.Bias + tzi.DaylightBias) * 60;
                    out_clock->is_dst = 1;
                } else {
                    out_clock->utc_offset_sec = -(int32_t)(tzi.Bias + tzi.StandardBias) * 60;
                    out_clock->is_dst = 0;
                }
            }
        }
    }
#else
    if (is_out_utc) {
        res_tm = gmtime_r(&sec, &tm_buf);
        if (!res_tm) {
            return -1;
        }
        out_clock->utc_offset_sec = 0;
        out_clock->is_dst = 0;
    } else {
        res_tm = localtime_r(&sec, &tm_buf);
        if (!res_tm) {
            return -1;
        }
        out_clock->utc_offset_sec = (int32_t)tm_buf.tm_gmtoff;
        out_clock->is_dst = tm_buf.tm_isdst;
    }
#endif

    out_clock->year = (int32_t)res_tm->tm_year + 1900;
    out_clock->month = (int32_t)res_tm->tm_mon + 1;
    out_clock->day = (int32_t)res_tm->tm_mday;
    out_clock->hour = (int32_t)res_tm->tm_hour;
    out_clock->minute = (int32_t)res_tm->tm_min;
    out_clock->second = (int32_t)res_tm->tm_sec;
    out_clock->millisecond = 0;
    out_clock->day_of_week = (int32_t)res_tm->tm_wday;
    out_clock->day_of_year = (int32_t)res_tm->tm_yday + 1;

    return 0;
}

int sys_clock_to_epoch(const sys_clock_t * clock, int is_input_utc, uint64_t * out_epoch) {
    struct tm t;
    time_t calculated_epoch;

    if (!clock || !out_epoch) {
        return -1;
    }

    memset(&t, 0, sizeof(struct tm));
    t.tm_year = clock->year - 1900;
    t.tm_mon = clock->month - 1;
    t.tm_mday = clock->day;
    t.tm_hour = clock->hour;
    t.tm_min = clock->minute;
    t.tm_sec = clock->second;
    t.tm_isdst = is_input_utc ? 0 : clock->is_dst;

    if (is_input_utc) {
#if defined(_WIN32) || defined(_WIN64)
        calculated_epoch = _mkgmtime(&t);
#elif defined(_BSD_SOURCE) || defined(_SVID_SOURCE) || defined(_DEFAULT_SOURCE) || defined(__linux__)
        calculated_epoch = timegm(&t);
#else
        /* Standard C fallback if timegm is unavailable */
        {
            time_t local_sec = mktime(&t);
            struct tm gbuf;
            time_t offset;
            if (local_sec == (time_t)-1) {
                return -1;
            }
#if defined(_WIN32) || defined(_WIN64)
            gmtime_s(&gbuf, &local_sec);
#else
            gmtime_r(&local_sec, &gbuf);
#endif
            offset = mktime(&gbuf) - local_sec;
            calculated_epoch = local_sec - offset;
        }
#endif
    } else {
        calculated_epoch = mktime(&t);
    }

    if (calculated_epoch == (time_t)-1) {
        return -1;
    }

    *out_epoch = (uint64_t)calculated_epoch;
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif /* SYS_CLOCK_IMPLEMENTATION_INCLUDED */
#endif /* SYS_CLOCK_IMPLEMENTATION */

// A Project Design by Arshad Latti with help of Gemini 2.5 and implemented by Gemini 2.5