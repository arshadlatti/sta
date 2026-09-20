/**
 * @file portable_dir.h
 * @brief Single-header STB-style portable UTF-8 directory scanning library for Win32 and Linux.
 *
 * Usage:
 *   In EXACTLY ONE C source file, define the implementation macro before including this header:
 *
 *     #define PORTABLE_DIR_IMPLEMENTATION
 *     #include "portable_dir.h"
 *
 *   In all other source files, simply include the header normally:
 *
 *     #include "portable_dir.h"
 *
 * Rules:
 *   1. Strictly requires POSIX forward slashes ('/'). Paths containing backslashes ('\') are rejected.
 *   2. On Windows, POSIX-style root drive paths like "/c/Windows" are translated to "C:\Windows".
 *   3. On Windows, path "/" lists logical drive roots as directories (e.g., "c", "d").
 *   4. Relative paths like "." or "src/lib" work identically across platforms without modification.
 */

#ifndef PORTABLE_DIR_H
#define PORTABLE_DIR_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represents a single file or directory entry.
 */
typedef struct portable_dir_entry {
    char *name;             /**< Dynamically allocated UTF-8 entry name */
    int is_dir;             /**< 1 if directory, 0 if regular file */
    uint64_t size;          /**< File size in bytes (0 for directories) */
    uint64_t time_create;   /**< Creation time in POSIX epoch seconds */
    uint64_t time_mod;      /**< Last modification time in POSIX epoch seconds */
} portable_dir_entry;

/**
 * @brief Container holding array of loaded directory entries.
 */
typedef struct portable_dir_t {
    portable_dir_entry *items;   /**< Dynamic array of directory entries */
    size_t count;                 /**< Total number of items in array */
} portable_dir_t;

/**
 * @brief Reads a directory and allocates a portable_dir_t container with all entries.
 *
 * Validates the UTF-8 input path, normalizes Unix-style drive letters (/c/dir -> C:\dir on Win32),
 * and scans the folder contents. On Windows, passing "/" returns available drive letters as directories.
 *
 * @param path Null-terminated UTF-8 encoded directory path using forward slashes ('/').
 * @return Pointer to allocated portable_dir_t on success, NULL on failure or if '\\' is detected.
 */
portable_dir_t *portable_dir_new(const char *path);

/**
 * @brief Frees all dynamically allocated memory associated with a portable_dir_t instance.
 *
 * Safely cleans up all item names, the items array, and the container struct.
 *
 * @param sd Pointer to portable_dir_t instance to delete. Safe to pass NULL.
 */
void portable_dir_delete(portable_dir_t *sd);

#ifdef __cplusplus
}
#endif

#endif /* PORTABLE_DIR_H */

/* ========================================================================= */
/*                             IMPLEMENTATION                                */
/* ========================================================================= */

#ifdef PORTABLE_DIR_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    #define PORTABLE_DIR_PLATFORM_WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#else
    #define PORTABLE_DIR_PLATFORM_POSIX
    #include <dirent.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

/* ------------------------------------------------------------------------- */
/*                           PLATFORM: WINDOWS                               */
/* ------------------------------------------------------------------------- */
#ifdef PORTABLE_DIR_PLATFORM_WIN32

/**
 * @brief Converts Windows FILETIME (100-nanosecond intervals since Jan 1, 1601) to POSIX epoch seconds.
 *
 * @param ft Pointer to FILETIME structure.
 * @return uint64_t Epoch seconds.
 */
static uint64_t portable_dir_filetime_to_epoch(const FILETIME *ft) {
    ULARGE_INTEGER ull;
    ull.LowPart = ft->dwLowDateTime;
    ull.HighPart = ft->dwHighDateTime;
    /* 116444736000000000ULL is the difference between 1601 and 1970 in 100ns units */
    if (ull.QuadPart < 116444736000000000ULL) return 0;
    return (uint64_t)((ull.QuadPart - 116444736000000000ULL) / 10000000ULL);
}

/**
 * @brief Converts UTF-8 string to Windows UTF-16 WCHAR string.
 *
 * @param utf8 Null-terminated UTF-8 string.
 * @return WCHAR* Allocated UTF-16 string or NULL on failure.
 */
static WCHAR *portable_dir_utf8_to_utf16(const char *utf8) {
    int target_len;
    WCHAR *wstr;

    if (!utf8) return NULL;
    target_len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    if (target_len <= 0) return NULL;

    wstr = (WCHAR *)malloc((size_t)target_len * sizeof(WCHAR));
    if (!wstr) return NULL; /* Check malloc return */

    if (MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, target_len) == 0) {
        free(wstr);
        return NULL;
    }
    return wstr;
}

/**
 * @brief Converts Windows UTF-16 WCHAR string to dynamic UTF-8 string.
 *
 * @param utf16 Null-terminated WCHAR string.
 * @return char* Allocated UTF-8 string or NULL on failure.
 */
static char *portable_dir_utf16_to_utf8(const WCHAR *utf16) {
    int target_len;
    char *str;

    if (!utf16) return NULL;
    target_len = WideCharToMultiByte(CP_UTF8, 0, utf16, -1, NULL, 0, NULL, NULL);
    if (target_len <= 0) return NULL;

    str = (char *)malloc((size_t)target_len * sizeof(char));
    if (!str) return NULL; /* Check malloc return */

    if (WideCharToMultiByte(CP_UTF8, 0, utf16, -1, str, target_len, NULL, NULL) == 0) {
        free(str);
        return NULL;
    }
    return str;
}

/**
 * @brief Internal helper to enumerate logical drive roots on Windows for virtual root "/".
 *
 * @return portable_dir_t* Pointer to container, or NULL on failure.
 */
static portable_dir_t *portable_dir_win32_enum_drives(void) {
    DWORD drives;
    int i;
    portable_dir_t *sd;

    drives = GetLogicalDrives();
    if (drives == 0) {
        return NULL;
    }

    sd = (portable_dir_t *)malloc(sizeof(portable_dir_t));
    if (!sd) return NULL; /* Check malloc return */

    sd->items = NULL;
    sd->count = 0;

    for (i = 0; i < 26; i++) {
        if (drives & (1UL << i)) {
            portable_dir_entry *new_items;
            portable_dir_entry *entry;

            new_items = (portable_dir_entry *)realloc(sd->items, (sd->count + 1) * sizeof(portable_dir_entry));
            if (!new_items) {
                /* Memory leak prevention */
                portable_dir_delete(sd);
                return NULL;
            }
            sd->items = new_items;

            entry = &sd->items[sd->count];
            entry->name = (char *)malloc(2 * sizeof(char));
            if (!entry->name) {
                portable_dir_delete(sd);
                return NULL; /* Check malloc return */
            }

            entry->name[0] = (char)('a' + i);
            entry->name[1] = '\0';
            entry->is_dir = 1;
            entry->size = 0;
            entry->time_create = 0;
            entry->time_mod = 0;

            sd->count++;
        }
    }

    return sd;
}

portable_dir_t *portable_dir_new(const char *path) {
    size_t len;
    size_t cur_len;
    size_t i;
    char *win_path;
    WCHAR *wpath;
    WIN32_FIND_DATAW find_data;
    HANDLE hFind;
    portable_dir_t *sd;
    size_t capacity;

    if (!path) return NULL;

    /* Reject paths containing backslashes */
    if (strchr(path, '\\') != NULL) {
        return NULL;
    }

    /* Virtual Root Handler for Windows */
    if (strcmp(path, "/") == 0) {
        return portable_dir_win32_enum_drives();
    }

    len = strlen(path);
    win_path = (char *)malloc(len + 4);
    if (!win_path) return NULL; /* Check malloc return */

    /* Normalize POSIX-style root drive paths (/c/dir -> C:\dir) */
    if (len >= 2 && path[0] == '/' &&
        ((path[1] >= 'a' && path[1] <= 'z') || (path[1] >= 'A' && path[1] <= 'Z')) &&
        (path[2] == '/' || path[2] == '\0')) {
        
        win_path[0] = path[1];
        win_path[1] = ':';
        win_path[2] = '\\';
        if (path[2] == '/') {
            strcpy(win_path + 3, path + 3);
        } else {
            win_path[3] = '\0';
        }
    } else {
        strcpy(win_path, path);
    }

    /* Standardize internal forward slashes to backslashes for Win32 API */
    for (i = 0; win_path[i] != '\0'; i++) {
        if (win_path[i] == '/') {
            win_path[i] = '\\';
        }
    }

    /* Append wildcard pattern search (\*) */
    cur_len = strlen(win_path);
    if (cur_len > 0 && win_path[cur_len - 1] == '\\') {
        strcat(win_path, "*");
    } else {
        strcat(win_path, "\\*");
    }

    wpath = portable_dir_utf8_to_utf16(win_path);
    free(win_path);
    if (!wpath) return NULL;

    hFind = FindFirstFileW(wpath, &find_data);
    free(wpath);

    if (hFind == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    sd = (portable_dir_t *)malloc(sizeof(portable_dir_t));
    if (!sd) {
        FindClose(hFind);
        return NULL; /* Check malloc return */
    }
    sd->items = NULL;
    sd->count = 0;

    capacity = 0;

    do {
        portable_dir_entry *entry;
        ULARGE_INTEGER file_size;

        /* Skip current (.) and parent (..) directory navigation entries */
        if (wcscmp(find_data.cFileName, L".") == 0 || wcscmp(find_data.cFileName, L"..") == 0) {
            continue;
        }

        if (sd->count >= capacity) {
            size_t new_cap = (capacity == 0) ? 16 : capacity * 2;
            portable_dir_entry *new_items = (portable_dir_entry *)realloc(sd->items, new_cap * sizeof(portable_dir_entry));
            if (!new_items) {
                /* Memory leak prevention: cleanup on realloc failure */
                FindClose(hFind);
                portable_dir_delete(sd);
                return NULL;
            }
            sd->items = new_items;
            capacity = new_cap;
        }

        entry = &sd->items[sd->count];
        entry->name = portable_dir_utf16_to_utf8(find_data.cFileName);
        if (!entry->name) {
            FindClose(hFind);
            portable_dir_delete(sd);
            return NULL;
        }

        entry->is_dir = (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
        
        file_size.LowPart = find_data.nFileSizeLow;
        file_size.HighPart = find_data.nFileSizeHigh;
        entry->size = entry->is_dir ? 0 : (uint64_t)file_size.QuadPart;

        entry->time_create = portable_dir_filetime_to_epoch(&find_data.ftCreationTime);
        entry->time_mod = portable_dir_filetime_to_epoch(&find_data.ftLastWriteTime);

        sd->count++;
    } while (FindNextFileW(hFind, &find_data) != 0);

    FindClose(hFind);
    return sd;
}

#endif /* PORTABLE_DIR_PLATFORM_WIN32 */

/* ------------------------------------------------------------------------- */
/*                            PLATFORM: POSIX                                */
/* ------------------------------------------------------------------------- */
#ifdef PORTABLE_DIR_PLATFORM_POSIX

portable_dir_t *portable_dir_new(const char *path) {
    DIR *dir;
    int dir_fd;
    portable_dir_t *sd;
    size_t capacity;
    struct dirent *entry_ptr;

    if (!path) return NULL;

    /* Reject paths containing backslashes */
    if (strchr(path, '\\') != NULL) {
        return NULL;
    }

    dir = opendir(path);
    if (!dir) return NULL;

    dir_fd = dirfd(dir);

    sd = (portable_dir_t *)malloc(sizeof(portable_dir_t));
    if (!sd) {
        closedir(dir);
        return NULL; /* Check malloc return */
    }
    sd->items = NULL;
    sd->count = 0;

    capacity = 0;
    entry_ptr = NULL;

    while ((entry_ptr = readdir(dir)) != NULL) {
        size_t name_len;
        portable_dir_entry *entry;
        struct stat st;

        /* Skip current (.) and parent (..) directory navigation entries */
        if (strcmp(entry_ptr->d_name, ".") == 0 || strcmp(entry_ptr->d_name, "..") == 0) {
            continue;
        }

        if (sd->count >= capacity) {
            size_t new_cap = (capacity == 0) ? 16 : capacity * 2;
            portable_dir_entry *new_items = (portable_dir_entry *)realloc(sd->items, new_cap * sizeof(portable_dir_entry));
            if (!new_items) {
                /* Memory leak prevention: cleanup on realloc failure */
                closedir(dir);
                portable_dir_delete(sd);
                return NULL;
            }
            sd->items = new_items;
            capacity = new_cap;
        }

        entry = &sd->items[sd->count];
        name_len = strlen(entry_ptr->d_name);
        entry->name = (char *)malloc(name_len + 1);
        if (!entry->name) {
            closedir(dir);
            portable_dir_delete(sd);
            return NULL; /* Check malloc return */
        }
        strcpy(entry->name, entry_ptr->d_name);

        /* Query metadata safely using fstatat relative to open directory FD */
        if (dir_fd != -1 && fstatat(dir_fd, entry_ptr->d_name, &st, AT_SYMLINK_NOFOLLOW) == 0) {
            entry->is_dir = S_ISDIR(st.st_mode) ? 1 : 0;
            entry->size = entry->is_dir ? 0 : (uint64_t)st.st_size;
            entry->time_create = (uint64_t)st.st_ctime;
            entry->time_mod = (uint64_t)st.st_mtime;
        } else {
            /* Fallback parsing if fstatat fails */
            entry->is_dir = (entry_ptr->d_type == DT_DIR) ? 1 : 0;
            entry->size = 0;
            entry->time_create = 0;
            entry->time_mod = 0;
        }

        sd->count++;
    }

    closedir(dir);
    return sd;
}

#endif /* PORTABLE_DIR_PLATFORM_POSIX */

/* ------------------------------------------------------------------------- */
/*                          COMMON CLEANUP LOGIC                             */
/* ------------------------------------------------------------------------- */

void portable_dir_delete(portable_dir_t *sd) {
    size_t i;

    if (!sd) return;

    if (sd->items) {
        for (i = 0; i < sd->count; i++) {
            if (sd->items[i].name) {
                free(sd->items[i].name);
                sd->items[i].name = NULL;
            }
        }
        free(sd->items);
        sd->items = NULL;
    }
    free(sd);
}

#endif /* PORTABLE_DIR_IMPLEMENTATION */

// A Project Design by Arshad Latti with help of Gemini 2.5 and implemented by Gemini 2.5