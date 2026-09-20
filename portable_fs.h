/**
 * @file portable_fs.h
 * @brief Single-header portable UTF-8 filesystem and directory management library for Win32 and POSIX.
 *
 * Usage:
 *   In EXACTLY ONE C source file, define the implementation macro before including this header:
 *
 *     #define PORTABLE_FS_IMPLEMENTATION
 *     #include "portable_fs.h"
 *
 *   In all other source files, simply include the header normally:
 *
 *     #include "portable_fs.h"
 *
 * Rules:
 *   1. Strictly requires POSIX forward slashes ('/'). Paths containing backslashes ('\') are rejected[cite: 2].
 *   2. On Windows, POSIX-style root drive paths like "/c/Windows" are translated to "C:\Windows"[cite: 2].
 *   3. Relative paths work identically across platforms without modification[cite: 2].
 */

#ifndef PORTABLE_FS_H
#define PORTABLE_FS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief File system object classification.
 */
typedef enum portable_fs_type {
    PORTABLE_FS_NONE = 0,   /**< Path does not exist */
    PORTABLE_FS_FILE = 1,   /**< Regular file */
    PORTABLE_FS_DIR  = 2    /**< Directory */
} portable_fs_type;

/**
 * @brief Metadata information container for a filesystem path.
 */
typedef struct portable_fs_info {
    portable_fs_type type;  /**< Item type (FILE, DIR, or NONE) */
    uint64_t size;          /**< File size in bytes (0 for directories) */
    uint64_t time_create;   /**< Creation time in POSIX epoch seconds */
    uint64_t time_mod;      /**< Last modification time in POSIX epoch seconds */
} portable_fs_info;

/* ------------------------------------------------------------------------- */
/*                         FILESYSTEM OPERATIONS                             */
/* ------------------------------------------------------------------------- */

/**
 * @brief Creates a single directory at the given path.
 * @param path Null-terminated UTF-8 path using POSIX forward slashes ('/')[cite: 2].
 * @return 0 on success, non-zero on error or if path contains backslashes[cite: 2].
 */
int portable_fs_create_dir(const char *path);

/**
 * @brief Recursively creates missing nested directories along a path (e.g., "data/cache/images").
 * @param path Null-terminated UTF-8 path using POSIX forward slashes ('/')[cite: 2].
 * @return 0 on success, non-zero on error[cite: 2].
 */
int portable_fs_create_dir_all(const char *path);

/**
 * @brief Removes an empty directory.
 * @param path Null-terminated UTF-8 path using POSIX forward slashes ('/')[cite: 2].
 * @return 0 on success, non-zero on failure[cite: 2].
 */
int portable_fs_remove_dir(const char *path);

/**
 * @brief Recursively removes a directory and all of its nested contents.
 * @param path Null-terminated UTF-8 path using POSIX forward slashes ('/')[cite: 2].
 * @return 0 on success, non-zero on failure[cite: 2].
 */
int portable_fs_remove_dir_all(const char *path);

/**
 * @brief Permanently deletes a file from the storage device.
 * @param path Null-terminated UTF-8 path using POSIX forward slashes ('/')[cite: 2].
 * @return 0 on success, non-zero on failure[cite: 2].
 */
int portable_fs_remove_file(const char *path);

/**
 * @brief Renames or moves a file or directory atomically.
 * @param old_path Current UTF-8 path.
 * @param new_path Target UTF-8 path.
 * @param overwrite If non-zero (true), replaces destination file if it exists. If 0 (false), fails if destination exists.
 * @return 0 on success, non-zero on failure.
 */
int portable_fs_rename(const char *old_path, const char *new_path, int overwrite);

/**
 * @brief Safely copies a file from source to destination path.
 * @param src_path UTF-8 path to source file.
 * @param dst_path UTF-8 path to destination file.
 * @param overwrite If non-zero (true), overwrites destination file if it exists. If 0 (false), fails if destination exists.
 * @return 0 on success, non-zero on failure.
 */
int portable_fs_copy_file(const char *src_path, const char *dst_path, int overwrite);

/**
 * @brief Checks if a path exists and determines its item classification.
 * @param path Null-terminated UTF-8 path using POSIX forward slashes ('/')[cite: 2].
 * @return PORTABLE_FS_NONE if non-existent, PORTABLE_FS_FILE for regular files, PORTABLE_FS_DIR for directories[cite: 2].
 */
portable_fs_type portable_fs_exists(const char *path);

/**
 * @brief Queries metadata stats (type, size, creation time, modification time) for a target path.
 * @param path Null-terminated UTF-8 path using POSIX forward slashes ('/')[cite: 2].
 * @param info Pointer to portable_fs_info structure to receive output metrics.
 * @return 0 on success, non-zero if target path does not exist or stat fails.
 */
int portable_fs_stat(const char *path, portable_fs_info *info);

/**
 * @brief Safely sends a file or directory to system Recycle Bin / Trash.
 * @param path Null-terminated UTF-8 path using POSIX forward slashes ('/')[cite: 2].
 * @return 0 on success, non-zero on failure.
 */
int portable_fs_send2trash(const char *path);

#ifdef __cplusplus
}
#endif

#endif /* PORTABLE_FS_H */

/* ------------------------------------------------------------------------- */
/*                             IMPLEMENTATION                                */
/* ------------------------------------------------------------------------- */

#ifdef PORTABLE_FS_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #include <shellapi.h>
#else
    #define _DEFAULT_SOURCE
    #define _BSD_SOURCE
    #define _POSIX_C_SOURCE 200809L
    #include <unistd.h>
    #include <dirent.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <errno.h>
    #include <utime.h>
#endif

/* ------------------------------------------------------------------------- */
/*                             INTERNAL HELPERS                              */
/* ------------------------------------------------------------------------- */

/* Validates path strictness: rejects backslashes and null/empty paths[cite: 2]. */
static int fs_internal_validate_path(const char *path) {
    const char *p;
    if (!path || *path == '\0') return 0;
    for (p = path; *p != '\0'; p++) {
        if (*p == '\\') return 0; /* Strictly reject backslashes[cite: 2] */
    }
    return 1;
}

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

static wchar_t *fs_internal_utf8_to_utf16(const char *utf8_str) {
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

/* Prepares path for Win32 API: Translates "/c/path" -> "C:\path" and converts '/' to '\'[cite: 2]. */
static wchar_t *fs_internal_prepare_win32_path(const char *path) {
    char *norm_path;
    wchar_t *wpath;
    size_t len, i;

    if (!fs_internal_validate_path(path)) return NULL;

    len = strlen(path);
    norm_path = (char *)malloc(len + 1);
    if (!norm_path) return NULL;
    memcpy(norm_path, path, len + 1);

    /* Normalize POSIX drive syntax "/c/path" -> "c:/path"[cite: 2] */
    if (len >= 3 && norm_path[0] == '/' && norm_path[2] == '/' &&
        ((norm_path[1] >= 'a' && norm_path[1] <= 'z') ||
         (norm_path[1] >= 'A' && norm_path[1] <= 'Z'))) {
        norm_path[0] = norm_path[1];
        norm_path[1] = ':';
    }

    /* Convert forward slashes to backslashes for Win32 subsystem */
    for (i = 0; i < len; i++) {
        if (norm_path[i] == '/') norm_path[i] = '\\';
    }

    wpath = fs_internal_utf8_to_utf16(norm_path);
    free(norm_path);
    return wpath;
}

/* Conversions from Windows FILETIME to POSIX Epoch Seconds */
static uint64_t fs_internal_filetime_to_posix_sec(const FILETIME *ft) {
    uint64_t windows_ticks = ((uint64_t)ft->dwHighDateTime << 32) | ft->dwLowDateTime;
    /* Windows epoch (1601-01-01) to POSIX epoch (1970-01-01) difference in 100-nanosecond units */
    uint64_t sec = (windows_ticks - 116444736000000000ULL) / 10000000ULL;
    return sec;
}

#endif

/* ------------------------------------------------------------------------- */
/*                             PUBLIC API                                    */
/* ------------------------------------------------------------------------- */

int portable_fs_create_dir(const char *path) {
    if (!fs_internal_validate_path(path)) return -1;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    {
        BOOL res;
        wchar_t *wpath = fs_internal_prepare_win32_path(path);
        if (!wpath) return -1;
        res = CreateDirectoryW(wpath, NULL);
        free(wpath);
        return res ? 0 : -1;
    }
#else
    return (mkdir(path, 0755) == 0) ? 0 : -1;
#endif
}

int portable_fs_create_dir_all(const char *path) {
    char *subpath;
    size_t len, i;

    if (!fs_internal_validate_path(path)) return -1;

    len = strlen(path);
    subpath = (char *)malloc(len + 1);
    if (!subpath) return -1;

    for (i = 0; i < len; i++) {
        subpath[i] = path[i];
        if (path[i] == '/' || i == len - 1) {
            subpath[i + 1] = '\0';
            /* Skip root slashes and Windows drive colon segments e.g. "c:" */
            if (i > 0 && subpath[i - 1] != ':') {
                if (portable_fs_exists(subpath) == PORTABLE_FS_NONE) {
                    if (portable_fs_create_dir(subpath) != 0) {
                        free(subpath);
                        return -1;
                    }
                }
            }
            subpath[i] = path[i];
        }
    }

    free(subpath);
    return 0;
}

int portable_fs_remove_dir(const char *path) {
    if (!fs_internal_validate_path(path)) return -1;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    {
        BOOL res;
        wchar_t *wpath = fs_internal_prepare_win32_path(path);
        if (!wpath) return -1;
        res = RemoveDirectoryW(wpath);
        free(wpath);
        return res ? 0 : -1;
    }
#else
    return (rmdir(path) == 0) ? 0 : -1;
#endif
}

int portable_fs_remove_dir_all(const char *path) {
    if (!fs_internal_validate_path(path)) return -1;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    {
        int res;
        size_t wlen;
        wchar_t *wpath;
        wchar_t *double_null_wpath;
        SHFILEOPSTRUCTW file_op;

        if (portable_fs_exists(path) != PORTABLE_FS_DIR) return -1;

        wpath = fs_internal_prepare_win32_path(path);
        if (!wpath) return -1;

        wlen = wcslen(wpath);
        double_null_wpath = (wchar_t *)malloc((wlen + 2) * sizeof(wchar_t));
        if (!double_null_wpath) {
            free(wpath);
            return -1;
        }

        memcpy(double_null_wpath, wpath, wlen * sizeof(wchar_t));
        double_null_wpath[wlen] = L'\0';
        double_null_wpath[wlen + 1] = L'\0';

        memset(&file_op, 0, sizeof(SHFILEOPSTRUCTW));
        file_op.wFunc = FO_DELETE;
        file_op.pFrom = double_null_wpath;
        file_op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

        res = SHFileOperationW(&file_op);

        free(wpath);
        free(double_null_wpath);

        return (res == 0) ? 0 : -1;
    }
#else
    {
        DIR *d;
        struct dirent *p;

        if (portable_fs_exists(path) != PORTABLE_FS_DIR) return -1;

        d = opendir(path);
        if (!d) return -1;

        while ((p = readdir(d)) != NULL) {
            char *buf;
            size_t len;

            if (strcmp(p->d_name, ".") == 0 || strcmp(p->d_name, "..") == 0) {
                continue;
            }

            len = strlen(path) + strlen(p->d_name) + 2;
            buf = (char *)malloc(len);
            if (!buf) {
                closedir(d);
                return -1;
            }

            sprintf(buf, "%s/%s", path, p->d_name);

            if (portable_fs_exists(buf) == PORTABLE_FS_DIR) {
                portable_fs_remove_dir_all(buf);
            } else {
                portable_fs_remove_file(buf);
            }
            free(buf);
        }

        closedir(d);
        return portable_fs_remove_dir(path);
    }
#endif
}

int portable_fs_remove_file(const char *path) {
    if (!fs_internal_validate_path(path)) return -1;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    {
        BOOL res;
        wchar_t *wpath = fs_internal_prepare_win32_path(path);
        if (!wpath) return -1;
        res = DeleteFileW(wpath);
        free(wpath);
        return res ? 0 : -1;
    }
#else
    return (unlink(path) == 0) ? 0 : -1;
#endif
}

int portable_fs_rename(const char *old_path, const char *new_path, int overwrite) {
    if (!fs_internal_validate_path(old_path) || !fs_internal_validate_path(new_path)) return -1;

    if (!overwrite && portable_fs_exists(new_path) != PORTABLE_FS_NONE) {
        return -1;
    }

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    {
        BOOL res;
        DWORD flags = MOVEFILE_COPY_ALLOWED;
        wchar_t *wold = fs_internal_prepare_win32_path(old_path);
        wchar_t *wnew = fs_internal_prepare_win32_path(new_path);

        if (!wold || !wnew) {
            free(wold);
            free(wnew);
            return -1;
        }

        if (overwrite) {
            flags |= MOVEFILE_REPLACE_EXISTING;
        }

        res = MoveFileExW(wold, wnew, flags);

        free(wold);
        free(wnew);
        return res ? 0 : -1;
    }
#else
    return (rename(old_path, new_path) == 0) ? 0 : -1;
#endif
}

int portable_fs_copy_file(const char *src_path, const char *dst_path, int overwrite) {
    if (!fs_internal_validate_path(src_path) || !fs_internal_validate_path(dst_path)) return -1;

    if (!overwrite && portable_fs_exists(dst_path) != PORTABLE_FS_NONE) {
        return -1;
    }

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    {
        BOOL res;
        wchar_t *wsrc = fs_internal_prepare_win32_path(src_path);
        wchar_t *wdst = fs_internal_prepare_win32_path(dst_path);

        if (!wsrc || !wdst) {
            free(wsrc);
            free(wdst);
            return -1;
        }

        res = CopyFileW(wsrc, wdst, overwrite ? FALSE : TRUE);

        free(wsrc);
        free(wdst);
        return res ? 0 : -1;
    }
#else
    {
        FILE *fsrc, *fdst;
        char buffer[8192];
        size_t bytes;

        fsrc = fopen(src_path, "rb");
        if (!fsrc) return -1;

        fdst = fopen(dst_path, "wb");
        if (!fdst) {
            fclose(fsrc);
            return -1;
        }

        while ((bytes = fread(buffer, 1, sizeof(buffer), fsrc)) > 0) {
            if (fwrite(buffer, 1, bytes, fdst) != bytes) {
                fclose(fsrc);
                fclose(fdst);
                return -1;
            }
        }

        fclose(fsrc);
        fclose(fdst);
        return 0;
    }
#endif
}

portable_fs_type portable_fs_exists(const char *path) {
    portable_fs_info info;
    if (portable_fs_stat(path, &info) != 0) {
        return PORTABLE_FS_NONE;
    }
    return info.type;
}

int portable_fs_stat(const char *path, portable_fs_info *info) {
    if (!info || !fs_internal_validate_path(path)) return -1;

    memset(info, 0, sizeof(portable_fs_info));

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    {
        WIN32_FILE_ATTRIBUTE_DATA data;
        wchar_t *wpath = fs_internal_prepare_win32_path(path);
        if (!wpath) return -1;

        if (!GetFileAttributesExW(wpath, GetFileExInfoStandard, &data)) {
            free(wpath);
            return -1;
        }
        free(wpath);

        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            info->type = PORTABLE_FS_DIR;
            info->size = 0;
        } else {
            info->type = PORTABLE_FS_FILE;
            info->size = ((uint64_t)data.nFileSizeHigh << 32) | data.nFileSizeLow;
        }

        info->time_create = fs_internal_filetime_to_posix_sec(&data.ftCreationTime);
        info->time_mod = fs_internal_filetime_to_posix_sec(&data.ftLastWriteTime);

        return 0;
    }
#else
    {
        struct stat st;
        if (stat(path, &st) != 0) return -1;

        if (S_ISDIR(st.st_mode)) {
            info->type = PORTABLE_FS_DIR;
            info->size = 0;
        } else if (S_ISREG(st.st_mode)) {
            info->type = PORTABLE_FS_FILE;
            info->size = (uint64_t)st.st_size;
        } else {
            info->type = PORTABLE_FS_NONE;
            return -1;
        }

        info->time_create = (uint64_t)st.st_ctime;
        info->time_mod = (uint64_t)st.st_mtime;

        return 0;
    }
#endif
}

int portable_fs_send2trash(const char *path) {
    if (!fs_internal_validate_path(path)) return -1;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    {
        int res;
        size_t wlen;
        wchar_t *wpath;
        wchar_t *double_null_wpath;
        SHFILEOPSTRUCTW file_op;

        wpath = fs_internal_prepare_win32_path(path);
        if (!wpath) return -1;

        wlen = wcslen(wpath);
        double_null_wpath = (wchar_t *)malloc((wlen + 2) * sizeof(wchar_t));
        if (!double_null_wpath) {
            free(wpath);
            return -1;
        }

        memcpy(double_null_wpath, wpath, wlen * sizeof(wchar_t));
        double_null_wpath[wlen] = L'\0';
        double_null_wpath[wlen + 1] = L'\0';

        memset(&file_op, 0, sizeof(SHFILEOPSTRUCTW));
        file_op.wFunc = FO_DELETE;
        file_op.pFrom = double_null_wpath;
        file_op.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

        res = SHFileOperationW(&file_op);

        free(wpath);
        free(double_null_wpath);

        return (res == 0) ? 0 : -1;
    }
#else
    /* Fallback on POSIX environments where no universal desktop trash bin is available */
    return portable_fs_remove_file(path);
#endif
}

#endif /* PORTABLE_FS_IMPLEMENTATION */

// A Project Design by Arshad Latti with help of Gemini 2.5 and implemented by Gemini 2.5