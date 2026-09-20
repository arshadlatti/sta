/**
 * @file sta_user_input.h
 * @brief Single-header C89 library for dynamic standard input reading.
 *
 * Provides functions to read single-line and multi-line user inputs from standard
 * input (stdin) with automatic memory growth via realloc.
 *
 * Usage:
 *   #define STA_USER_INPUT_IMPLEMENTATION
 *   #include "sta_user_input.h"
 *
 *   // A Project Design by Arshad Latti with help of Gemini 2.5 Flash and implemented by Gemini 2.5 Flash
 */

#ifndef STA_USER_INPUT_H
#define STA_USER_INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================= */
/* FUNCTION DECLARATIONS                                                     */
/* ========================================================================= */

/**
 * @brief Reads a single line of text from standard input into a dynamically allocated buffer.
 *
 * Allocates an initial memory buffer of 4096 bytes and automatically grows it
 * using realloc as the input expands. Normalizes CRLF ('\r\n') to LF ('\n') and 
 * strips the trailing newline character.
 *
 * @return Pointer to a null-terminated string on success (must be freed by caller),
 *         or NULL on read error, immediate EOF, or memory allocation failure.
 */
char *get_user_input_malloc(void);

/**
 * @brief Reads multi-line text from standard input into a dynamically allocated buffer
 *        until a line matching sentinel is entered.
 *
 * Accumulates input across multiple lines (preserving internal line breaks '\n') until
 * a single line matching the sentinel string is encountered. Normalizes CRLF.
 *
 * @param sentinel Line string that terminates reading (e.g., "" for double Enter, or "/end").
 *                 If sentinel is NULL, defaults to empty line ("").
 *
 * @return Pointer to a null-terminated string on success (must be freed by caller),
 *         or NULL on read error, immediate EOF, or memory allocation failure.
 */
char *get_user_input_multiline_malloc(const char *sentinel);

#ifdef __cplusplus
}
#endif

#endif /* STA_USER_INPUT_H */

/* ========================================================================= */
/* IMPLEMENTATION SECTION                                                    */
/* ========================================================================= */

#ifdef STA_USER_INPUT_IMPLEMENTATION

char *get_user_input_malloc(void) {
    size_t capacity = 4096;
    size_t length = 0;
    int c;
    char *buffer = (char *)malloc(capacity);
    char *new_buffer = NULL;

    /* Check initial allocation failure */
    if (!buffer) {
        return NULL;
    }

    while ((c = fgetc(stdin)) != EOF && c != '\n') {
        if (c == '\r') {
            continue; /* Normalize windows CRLF */
        }

        /* Check buffer capacity (reserve 1 byte for null terminator) */
        if (length + 1 >= capacity) {
            capacity *= 2;
            new_buffer = (char *)realloc(buffer, capacity);
            if (!new_buffer) {
                /* Free existing block to prevent memory leak */
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }
        buffer[length++] = (char)c;
    }

    /* Handle immediate EOF without read characters */
    if (c == EOF && length == 0) {
        free(buffer);
        return NULL;
    }

    /* Null terminate string */
    buffer[length] = '\0';
    return buffer;
}

char *get_user_input_multiline_malloc(const char *sentinel) {
    size_t capacity = 4096;
    size_t length = 0;
    size_t line_start = 0;
    int c;
    char *buffer = (char *)malloc(capacity);
    char *new_buffer = NULL;

    /* Default to empty string sentinel if NULL */
    if (!sentinel) {
        sentinel = "";
    }

    /* Check initial allocation failure */
    if (!buffer) {
        return NULL;
    }

    while ((c = fgetc(stdin)) != EOF) {
        if (c == '\r') {
            continue; /* Normalize windows CRLF */
        }

        /* Ensure space for byte + potential null terminator */
        if (length + 1 >= capacity) {
            capacity *= 2;
            new_buffer = (char *)realloc(buffer, capacity);
            if (!new_buffer) {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }

        if (c == '\n') {
            buffer[length] = '\0';

            /* Check if the line just read equals the sentinel */
            if (strcmp(buffer + line_start, sentinel) == 0) {
                /* Truncate the sentinel from the string */
                buffer[line_start] = '\0';

                /* Strip preceding newline character if present */
                if (line_start > 0 && buffer[line_start - 1] == '\n') {
                    buffer[line_start - 1] = '\0';
                }
                break;
            }

            buffer[length++] = '\n';
            line_start = length;
        } else {
            buffer[length++] = (char)c;
        }
    }

    /* Handle immediate EOF without read characters */
    if (c == EOF && length == 0) {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    return buffer;
}

#endif /* STA_USER_INPUT_IMPLEMENTATION */

// A Project Design by Arshad Latti with help of Gemini 2.5 Flash and implemented by Gemini 2.5 Flash