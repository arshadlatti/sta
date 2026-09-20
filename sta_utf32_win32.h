#ifndef STA_UTF32_WIN32_H
#define STA_UTF32_WIN32_H

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * PUBLIC DECLARATIONS & API
 * ============================================================================ */
/*
#define STA_UTF32_WIN32_IMPLEMENTATION
#include "sta_utf32_win32.h"
*/

/**
 * Converts a UTF-32 buffer to a null-terminated Win32 LPWSTR (UTF-16LE).
 * Useful for SetWindowTextW, DrawTextW, shell APIs, etc.
 * 
 * Note: Does NOT write a BOM (unnecessary for Windows API calls).
 * Free the returned pointer with free().
 */
LPWSTR UnicodeFromUtf32_malloc(const uint32_t * str, size_t str_len, size_t * out_wlen);

/**
 * Converts a Win32 LPCWSTR (UTF-16LE) to a null-terminated UTF-32 buffer.
 * Useful for GetWindowTextW, GetDlgItemTextW, drag-and-drop paths, etc.
 * 
 * out_len receives the count of uint32_t codepoints (excluding trailing 0).
 * Free the returned pointer with free().
 */
uint32_t * Utf32FromUnicode_malloc(LPCWSTR str, size_t * out_len);

// A Project by Arshad Latti with help of "Gemini 3.6 flash"
#ifdef __cplusplus
}
#endif

#endif // _WIN32 || _WIN64
#endif // STA_UTF32_WIN32_H

/* ============================================================================
 * IMPLEMENTATION
 * ============================================================================ */

#ifdef STA_UTF32_WIN32_IMPLEMENTATION
#undef STA_UTF32_WIN32_IMPLEMENTATION

#if defined(_WIN32) || defined(_WIN64)

#ifdef __cplusplus
extern "C" {
#endif

LPWSTR UnicodeFromUtf32_malloc(const uint32_t * str, size_t str_len, size_t * out_wlen)
{
    if (!str) {
        if (out_wlen) *out_wlen = 0;
        return NULL;
    }

    // Allocate max possible memory:
    // Each UTF-32 codepoint produces at most 2 WCHARs (surrogate pair).
    // +1 WCHAR for trailing L'\0'.
    size_t max_wchars = (str_len * 2) + 1;
    LPWSTR out_buf = (LPWSTR)malloc(max_wchars * sizeof(WCHAR));
    if (!out_buf) {
        if (out_wlen) *out_wlen = 0;
        return NULL;
    }

    size_t out_idx = 0;
size_t i;
    for ( i = 0; i < str_len; i++) {
        uint32_t cp = str[i];

        if (cp == 0) break; // Early exit on embedded null terminator

        if (cp <= 0xD7FF || (cp >= 0xE000 && cp <= 0xFFFF)) {
            // BMP code point (1 WCHAR)
            out_buf[out_idx++] = (WCHAR)cp;
        }
        else if (cp >= 0x10000 && cp <= 0x10FFFF) {
            // Supplementary plane (Surrogate pair: 2 WCHARs)
            cp -= 0x10000;
            out_buf[out_idx++] = (WCHAR)(0xD800 + (cp >> 10));   // High surrogate
            out_buf[out_idx++] = (WCHAR)(0xDC00 + (cp & 0x3FF));  // Low surrogate
        }
        else {
            // Invalid code point -> U+FFFD (Unicode replacement character)
            out_buf[out_idx++] = (WCHAR)0xFFFD;
        }
    }

    // Double-byte Null Termination
    out_buf[out_idx] = L'\0';

    if (out_wlen) {
        *out_wlen = out_idx;
    }

    // Shrink allocation to actual used size (+1 for L'\0')
    LPWSTR shrunk = (LPWSTR)realloc(out_buf, (out_idx + 1) * sizeof(WCHAR));
    return shrunk ? shrunk : out_buf;
}

uint32_t * Utf32FromUnicode_malloc(LPCWSTR str, size_t * out_len)
{
    if (!str) {
        if (out_len) *out_len = 0;
        return NULL;
    }

    // Measure null-terminated WCHAR length if non-empty
    size_t wlen = wcslen(str);

    // Max UTF-32 code points is at most wlen + 1 trailing null
    uint32_t * out_buf = (uint32_t *)malloc((wlen + 1) * sizeof(uint32_t));
    if (!out_buf) {
        if (out_len) *out_len = 0;
        return NULL;
    }

    size_t out_idx = 0;
size_t i;
    for ( i = 0; i < wlen; i++) {
        WCHAR w1 = str[i];

        // High Surrogate check
        if (w1 >= 0xD800 && w1 <= 0xDBFF) {
            if (i + 1 < wlen) {
                WCHAR w2 = str[i + 1];
                // Check if following item is a valid Low Surrogate
                if (w2 >= 0xDC00 && w2 <= 0xDFFF) {
                    uint32_t cp = 0x10000 + (((uint32_t)(w1 - 0xD800) << 10) | (uint32_t)(w2 - 0xDC00));
                    out_buf[out_idx++] = cp;
                    i++; // Skip low surrogate WCHAR
                    continue;
                }
            }
            // Unpaired high surrogate -> replacement character
            out_buf[out_idx++] = 0xFFFD;
        }
        else if (w1 >= 0xDC00 && w1 <= 0xDFFF) {
            // Unpaired low surrogate -> replacement character
            out_buf[out_idx++] = 0xFFFD;
        }
        else {
            // Standard BMP character
            out_buf[out_idx++] = (uint32_t)w1;
        }
    }

    // UTF-32 Null Termination
    out_buf[out_idx] = 0;

    if (out_len) {
        *out_len = out_idx;
    }

    // Shrink allocation
    uint32_t * shrunk = (uint32_t *)realloc(out_buf, (out_idx + 1) * sizeof(uint32_t));
    return shrunk ? shrunk : out_buf;
}

#ifdef __cplusplus
}
#endif

#endif // _WIN32 || _WIN64
#endif // STA_UTF32_WIN32_IMPLEMENTATION