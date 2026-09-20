/**
 * @file sys_file.h
 * @brief Single-header portable 64-bit file stream I/O library for Win32 and POSIX.
 *
 * Usage:
 *   In EXACTLY ONE C source file, define the implementation macro before including this header:
 *
 *     #define SYS_FILE_IMPLEMENTATION
 *     #include "sys_file.h"
 *
 *   In all other source files, simply include the header normally:
 *
 *     #include "sys_file.h"
 */

#ifndef SYS_FILE_H
#define SYS_FILE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque structure representing an active open file stream.
 */
typedef struct sys_file_t sys_file_t;

/**
 * @brief File access and creation mode semantics.
 */
typedef enum sys_file_mode {
    SYS_FILE_OPEN_READ,        /**< Open existing file for reading. Fails if missing. */
    SYS_FILE_OPEN_WRITE,       /**< Open existing file for writing. Fails if missing. */
    SYS_FILE_OPEN_READ_WRITE,  /**< Open existing file for read/write. Fails if missing. */
    SYS_FILE_NEW_WRITE,        /**< Create new file for writing. Overwrites/truncates by default. */
    SYS_FILE_NEW_READ_WRITE    /**< Create new file for read/write. Overwrites/truncates by default. */
} sys_file_mode;

/**
 * @brief Mode-specific file creation flags.
 */
#define SYS_FILE_OPEN_ALWAYS  1  /**< For SYS_FILE_OPEN_*: Open existing file, or create if missing */
#define SYS_FILE_NO_OVERWRITE 1  /**< For SYS_FILE_NEW_*: Create file only if it does not exist; fail if exists */

/**
 * @brief Positioning reference points for file pointer seeks.
 */
typedef enum sys_seek_origin {
    SYS_SEEK_SET = 0,  /**< Seek relative to start of file */
    SYS_SEEK_CUR = 1,  /**< Seek relative to current position */
    SYS_SEEK_END = 2   /**< Seek relative to end of file */
} sys_seek_origin;

/* ------------------------------------------------------------------------- */
/*                           FILE STREAM OPERATIONS                          */
/* ------------------------------------------------------------------------- */

/**
 * @brief Opens or creates a file stream using host-native path syntax.
 *
 * @param path Null-terminated UTF-8 encoded host file path.
 * @param mode File access and creation mode.
 * @param option Mode-specific creation option; use 0 for default behavior.
 * @return Pointer to allocated sys_file_t handle on success, NULL on failure.
 */
sys_file_t *sys_file_open(
    const char *path,
    sys_file_mode mode,
    int option
);

/**
 * @brief Opens or creates a file stream using strict portable path rules.
 *
 * Requires POSIX forward slashes ('/') and rejects backslashes ('\').
 * Normalizes Windows drive paths (e.g., "/c/data/file.txt") before delegating.
 *
 * @param path Null-terminated UTF-8 portable file path.
 * @param mode File access and creation mode.
 * @param option Mode-specific creation option; use 0 for default behavior.
 * @return Pointer to allocated sys_file_t handle on success, NULL on failure.
 */
sys_file_t *portable_file_open(
    const char *path,
    sys_file_mode mode,
    int option
);

/**
 * @brief Reads data from an active file stream into a buffer.
 * @param file Active file handle pointer.
 * @param buffer Destination buffer.
 * @param bytes_to_read Number of bytes requested.
 * @param bytes_read Output pointer receiving actual bytes read (must not be NULL).
 * @return 0 on success, non-zero on I/O failure.
 */
int sys_file_read(sys_file_t *file, void *buffer, size_t bytes_to_read, size_t *bytes_read);

/**
 * @brief Writes data from a buffer to an active file stream.
 * @param file Active file handle pointer.
 * @param buffer Source buffer containing payload.
 * @param bytes_to_write Number of bytes to write.
 * @param bytes_written Output pointer receiving actual bytes written (must not be NULL).
 * @return 0 on success, non-zero on I/O failure.
 */
int sys_file_write(sys_file_t *file, const void *buffer, size_t bytes_to_write, size_t *bytes_written);

/**
 * @brief Repositions file stream offset in 64-bit address space.
 * @param file Active file handle pointer.
 * @param offset 64-bit signed offset in bytes.
 * @param origin Seek reference point.
 * @param new_position Optional output pointer receiving new absolute byte position (can be NULL).
 * @return 0 on success, non-zero on seek failure.
 */
int sys_file_seek(sys_file_t *file, int64_t offset, sys_seek_origin origin, int64_t *new_position);

/**
 * @brief Queries current 64-bit offset position of the stream pointer.
 * @param file Active file handle pointer.
 * @return 64-bit byte offset from start, or -1 on invalid handle/error.
 */
int64_t sys_file_tell(sys_file_t *file);

/**
 * @brief Queries total 64-bit byte size of the file stream.
 * @param file Active file handle pointer.
 * @return 64-bit size in bytes, or -1 on invalid handle/error.
 */
int64_t sys_file_size(sys_file_t *file);

/**
 * @brief Truncates or extends an open file stream to a specified 64-bit size.
 * @param file Active file handle pointer opened with write access.
 * @param size Target 64-bit file size in bytes.
 * @return 0 on success, non-zero on failure.
 */
int sys_file_truncate(sys_file_t *file, uint64_t size);

/**
 * @brief Flushes unwritten OS buffers directly to physical storage.
 * @param file Active file handle pointer.
 * @return 0 on success, non-zero on failure.
 */
int sys_file_flush(sys_file_t *file);

/**
 * @brief Flushes pending stream data and closes active file handle.
 * @param file Active file handle pointer (safe to pass NULL).
 */
void sys_file_close(sys_file_t *file);

#ifdef __cplusplus
}
#endif

#endif /* SYS_FILE_H */

/* ------------------------------------------------------------------------- */
/*                             IMPLEMENTATION                                */
/* ------------------------------------------------------------------------- */

#ifdef SYS_FILE_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#else
    #define _LARGEFILE64_SOURCE
    #define _FILE_OFFSET_BITS 64
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <errno.h>
#endif

struct sys_file_t {
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    HANDLE handle;
#else
    int fd;
#endif
};

/* --- Platform Helpers --- */

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
static wchar_t *sys_file__utf8_to_utf16(const char *utf8_str) {
    int wlen;
    wchar_t *wstr;
    if (!utf8_str) return NULL;
    wlen = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, NULL, 0);
    if (wlen <= 0) return NULL;
    wstr = (wchar_t *)malloc((size_t)wlen * sizeof(wchar_t));
    if (!wstr) return NULL;
    if (MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, wstr, wlen) <= 0) {
        free(wstr);
        return NULL;
    }
    return wstr;
}
#endif

/* --- Public API Implementation --- */

sys_file_t *sys_file_open(const char *path, sys_file_mode mode, int option) {
    sys_file_t *file;
    if (!path) return NULL;

    file = (sys_file_t *)malloc(sizeof(sys_file_t));
    if (!file) return NULL;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    {
        DWORD desired_access = 0;
        DWORD creation_disposition = 0;
        wchar_t *wpath;

        switch (mode) {
            case SYS_FILE_OPEN_READ:
                desired_access = GENERIC_READ;
                creation_disposition = (option == SYS_FILE_OPEN_ALWAYS) ? OPEN_ALWAYS : OPEN_EXISTING;
                break;
            case SYS_FILE_OPEN_WRITE:
                desired_access = GENERIC_WRITE;
                creation_disposition = (option == SYS_FILE_OPEN_ALWAYS) ? OPEN_ALWAYS : OPEN_EXISTING;
                break;
            case SYS_FILE_OPEN_READ_WRITE:
                desired_access = GENERIC_READ | GENERIC_WRITE;
                creation_disposition = (option == SYS_FILE_OPEN_ALWAYS) ? OPEN_ALWAYS : OPEN_EXISTING;
                break;
            case SYS_FILE_NEW_WRITE:
                desired_access = GENERIC_WRITE;
                creation_disposition = (option == SYS_FILE_NO_OVERWRITE) ? CREATE_NEW : CREATE_ALWAYS;
                break;
            case SYS_FILE_NEW_READ_WRITE:
                desired_access = GENERIC_READ | GENERIC_WRITE;
                creation_disposition = (option == SYS_FILE_NO_OVERWRITE) ? CREATE_NEW : CREATE_ALWAYS;
                break;
            default:
                free(file);
                return NULL;
        }

        wpath = sys_file__utf8_to_utf16(path);
        if (!wpath) {
            free(file);
            return NULL;
        }

        file->handle = CreateFileW(
            wpath,
            desired_access,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            creation_disposition,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        free(wpath);

        if (file->handle == INVALID_HANDLE_VALUE) {
            free(file);
            return NULL;
        }
    }
#else
    {
        int flags = 0;
        mode_t permissions = 0666;

        switch (mode) {
            case SYS_FILE_OPEN_READ:
                flags = O_RDONLY;
                if (option == SYS_FILE_OPEN_ALWAYS) flags |= O_CREAT;
                break;
            case SYS_FILE_OPEN_WRITE:
                flags = O_WRONLY;
                if (option == SYS_FILE_OPEN_ALWAYS) flags |= O_CREAT;
                break;
            case SYS_FILE_OPEN_READ_WRITE:
                flags = O_RDWR;
                if (option == SYS_FILE_OPEN_ALWAYS) flags |= O_CREAT;
                break;
            case SYS_FILE_NEW_WRITE:
                flags = O_WRONLY | O_CREAT;
                if (option == SYS_FILE_NO_OVERWRITE) {
                    flags |= O_EXCL;
                } else {
                    flags |= O_TRUNC;
                }
                break;
            case SYS_FILE_NEW_READ_WRITE:
                flags = O_RDWR | O_CREAT;
                if (option == SYS_FILE_NO_OVERWRITE) {
                    flags |= O_EXCL;
                } else {
                    flags |= O_TRUNC;
                }
                break;
            default:
                free(file);
                return NULL;
        }

        file->fd = open(path, flags, permissions);
        if (file->fd < 0) {
            free(file);
            return NULL;
        }
    }
#endif

    return file;
}

sys_file_t *portable_file_open(const char *path, sys_file_mode mode, int option) {
    const char *p;
    char *norm_path;
    size_t len;
    sys_file_t *res;

    if (!path) return NULL;

    /* Reject backslashes strictly */
    for (p = path; *p; p++) {
        if (*p == '\\') return NULL;
    }

    len = strlen(path);
    norm_path = (char *)malloc(len + 1);
    if (!norm_path) return NULL;
    memcpy(norm_path, path, len + 1);

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    {
        size_t i;
        /* Translate POSIX drive paths: e.g. "/c/data/file.txt" -> "C:/data/file.txt" */
        if (len >= 3 && norm_path[0] == '/' && norm_path[2] == '/' &&
            ((norm_path[1] >= 'a' && norm_path[1] <= 'z') || 
             (norm_path[1] >= 'A' && norm_path[1] <= 'Z'))) {
            norm_path[0] = norm_path[1];
            norm_path[1] = ':';
        }
        /* Convert remaining forward slashes to backslashes for Win32 API */
        for (i = 0; i < len; i++) {
            if (norm_path[i] == '/') norm_path[i] = '\\';
        }
    }
#endif

    res = sys_file_open(norm_path, mode, option);
    free(norm_path);
    return res;
}

int sys_file_read(sys_file_t *file, void *buffer, size_t bytes_to_read, size_t *bytes_read) {
    if (!file || !buffer || !bytes_read) return -1;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    {
        DWORD dw_read = 0;
        if (!ReadFile(file->handle, buffer, (DWORD)bytes_to_read, &dw_read, NULL)) {
            *bytes_read = 0;
            return -1;
        }
        *bytes_read = (size_t)dw_read;
        return 0;
    }
#else
    {
        ssize_t res = read(file->fd, buffer, bytes_to_read);
        if (res < 0) {
            *bytes_read = 0;
            return -1;
        }
        *bytes_read = (size_t)res;
        return 0;
    }
#endif
}

int sys_file_write(sys_file_t *file, const void *buffer, size_t bytes_to_write, size_t *bytes_written) {
    if (!file || !buffer || !bytes_written) return -1;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    {
        DWORD dw_written = 0;
        if (!WriteFile(file->handle, buffer, (DWORD)bytes_to_write, &dw_written, NULL)) {
            *bytes_written = 0;
            return -1;
        }
        *bytes_written = (size_t)dw_written;
        return 0;
    }
#else
    {
        ssize_t res = write(file->fd, buffer, bytes_to_write);
        if (res < 0) {
            *bytes_written = 0;
            return -1;
        }
        *bytes_written = (size_t)res;
        return 0;
    }
#endif
}

int sys_file_seek(sys_file_t *file, int64_t offset, sys_seek_origin origin, int64_t *new_position) {
    if (!file) return -1;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    {
        LARGE_INTEGER li_offset;
        LARGE_INTEGER li_new_pos;
        DWORD win_origin;

        switch (origin) {
            case SYS_SEEK_SET: win_origin = FILE_BEGIN; break;
            case SYS_SEEK_CUR: win_origin = FILE_CURRENT; break;
            case SYS_SEEK_END: win_origin = FILE_END; break;
            default: return -1;
        }

        li_offset.QuadPart = offset;
        if (!SetFilePointerEx(file->handle, li_offset, &li_new_pos, win_origin)) {
            return -1;
        }
        if (new_position) *new_position = (int64_t)li_new_pos.QuadPart;
        return 0;
    }
#else
    {
        int posix_origin;
        off_t res;

        switch (origin) {
            case SYS_SEEK_SET: posix_origin = SEEK_SET; break;
            case SYS_SEEK_CUR: posix_origin = SEEK_CUR; break;
            case SYS_SEEK_END: posix_origin = SEEK_END; break;
            default: return -1;
        }

        res = lseek(file->fd, (off_t)offset, posix_origin);
        if (res == (off_t)-1) return -1;
        if (new_position) *new_position = (int64_t)res;
        return 0;
    }
#endif
}

int64_t sys_file_tell(sys_file_t *file) {
    int64_t pos = -1;
    if (sys_file_seek(file, 0, SYS_SEEK_CUR, &pos) != 0) return -1;
    return pos;
}

int64_t sys_file_size(sys_file_t *file) {
    if (!file) return -1;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    {
        LARGE_INTEGER li_size;
        if (!GetFileSizeEx(file->handle, &li_size)) return -1;
        return (int64_t)li_size.QuadPart;
    }
#else
    {
        struct stat st;
        if (fstat(file->fd, &st) != 0) return -1;
        return (int64_t)st.st_size;
    }
#endif
}

int sys_file_truncate(sys_file_t *file, uint64_t size) {
    if (!file) return -1;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    {
        int64_t current_pos = sys_file_tell(file);
        if (current_pos < 0) return -1;

        if (sys_file_seek(file, (int64_t)size, SYS_SEEK_SET, NULL) != 0) return -1;

        if (!SetEndOfFile(file->handle)) {
            sys_file_seek(file, current_pos, SYS_SEEK_SET, NULL);
            return -1;
        }

        sys_file_seek(file, current_pos, SYS_SEEK_SET, NULL);
        return 0;
    }
#else
    {
        if (ftruncate(file->fd, (off_t)size) != 0) return -1;
        return 0;
    }
#endif
}

int sys_file_flush(sys_file_t *file) {
    if (!file) return -1;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    return FlushFileBuffers(file->handle) ? 0 : -1;
#else
    return fsync(file->fd);
#endif
}

void sys_file_close(sys_file_t *file) {
    if (!file) return;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    if (file->handle != INVALID_HANDLE_VALUE) {
        CloseHandle(file->handle);
    }
#else
    if (file->fd >= 0) {
        close(file->fd);
    }
#endif

    free(file);
}

#endif /* SYS_FILE_IMPLEMENTATION */

// A Project Design by Arshad Latti with help of Gemini 2.5 and implemented by Gemini 2.5