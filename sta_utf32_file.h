#ifndef STA_UTF32_FILE_H
#define STA_UTF32_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <wchar.h>

/* Dependent base library included by user design */
#include "sta_utf32_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * PUBLIC DECLARATIONS & API
 * ============================================================================ */

/* Load file from disk and convert to dynamically allocated UTF-32 array */
uint32_t * a_utf32_load_file_malloc(const char * path, size_t * out_len, int * opt_out_bom_type);
uint32_t * w_utf32_load_file_malloc(const wchar_t * path, size_t * out_len, int * opt_out_bom_type);

/* Convert UTF-32 input to UTF-8 (with BOM) and save to disk */
int a_utf32_save_file(const char * path, const uint32_t * text, size_t text_len);
int w_utf32_save_file(const wchar_t * path, const uint32_t * text, size_t text_len);

#ifdef __cplusplus
}
#endif

#endif /* STA_UTF32_FILE_H */

#ifdef STA_UTF32_FILE_IMPLEMENTATION
#define STA_UTF32_BASE_IMPLEMENTATION
#endif
#ifndef STA_UTF32_BASE_H
#define STA_UTF32_BASE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * PUBLIC DECLARATIONS & API
 * ============================================================================ */
/*
#define STA_UTF32_BASE_IMPLEMENTATION
#include "sta-main/sta_utf32_base.h"
*/

// BOM Types
enum {
    BOM_NONE = 0,
    BOM_UTF8,
    BOM_UTF16_LE,
    BOM_UTF16_BE,
    BOM_UTF32_LE,
    BOM_UTF32_BE
};

uint32_t * utf32_from_memory_malloc(const char * str, size_t str_len, size_t * out_len, int * opt_out_bom_type);

// Omit BOM (Good for POSIX APIs, printf, network sockets, JSON)
char * text_from_utf32_malloc(uint32_t * str, size_t str_len, size_t * out_str_len);
uint8_t * utf8_text_from_utf32_malloc(uint32_t * str, size_t str_len, size_t * out_str_len, int b_write_bom);

// With BOM for file save
uint8_t * utf8_from_utf32_malloc(uint32_t * str, size_t str_len, size_t * out_str_len);

// A Project by Arshad Latti with help of "Gemini 3.6 flash" 
#ifdef __cplusplus
}
#endif

#endif /* STA_UTF32_BASE_H */

/* ============================================================================
 * IMPLEMENTATION
 * ============================================================================ */

#ifdef STA_UTF32_BASE_IMPLEMENTATION
#undef STA_UTF32_BASE_IMPLEMENTATION

#ifdef __cplusplus
extern "C" {
#endif

uint8_t * utf8_text_from_utf32_malloc(uint32_t * str, size_t str_len, size_t * out_str_len, int b_write_bom)
{
    if (!str) {
        if (out_str_len) *out_str_len = 0;
        return NULL;
    }

    // 1. Allocate maximum possible memory needed.
    // Each UTF-32 code point becomes at most 4 UTF-8 bytes.
    // +3 bytes for the UTF-8 BOM (0xEF, 0xBB, 0xBF)
    // +1 byte for the null-terminator.
    size_t max_bytes = (str_len * 4) + 3 + 1;
    uint8_t * out_buf = (uint8_t *)malloc(max_bytes);
    if (!out_buf) {
        if (out_str_len) *out_str_len = 0;
        return NULL;
    }

    size_t out_idx = 0;

    // 2. Write UTF-8 BOM (3 bytes)
    if (b_write_bom) {
        out_buf[out_idx++] = 0xEF;
        out_buf[out_idx++] = 0xBB;
        out_buf[out_idx++] = 0xBF;
    }

    // 3. Encode UTF-32 code points to UTF-8
    size_t i;
    for (i = 0; i < str_len; i++) {
        uint32_t cp = str[i];

        if (cp == 0) break; // Stop early if embedded null terminator is encountered

        if (cp <= 0x7F) {
            // 1-byte ASCII sequence
            out_buf[out_idx++] = (uint8_t)cp;
        } 
        else if (cp <= 0x7FF) {
            // 2-byte sequence
            out_buf[out_idx++] = (uint8_t)(0xC0 | (cp >> 6));
            out_buf[out_idx++] = (uint8_t)(0x80 | (cp & 0x3F));
        } 
        else if (cp <= 0xFFFF) {
            // 3-byte sequence
            if (cp >= 0xD800 && cp <= 0xDFFF) {
                // Invalid surrogate range codepoint -> output replacement char U+FFFD
                out_buf[out_idx++] = 0xEF;
                out_buf[out_idx++] = 0xBF;
                out_buf[out_idx++] = 0xBD;
            } else {
                out_buf[out_idx++] = (uint8_t)(0xE0 | (cp >> 12));
                out_buf[out_idx++] = (uint8_t)(0x80 | ((cp >> 6) & 0x3F));
                out_buf[out_idx++] = (uint8_t)(0x80 | (cp & 0x3F));
            }
        } 
        else if (cp <= 0x10FFFF) {
            // 4-byte sequence
            out_buf[out_idx++] = (uint8_t)(0xF0 | (cp >> 18));
            out_buf[out_idx++] = (uint8_t)(0x80 | ((cp >> 12) & 0x3F));
            out_buf[out_idx++] = (uint8_t)(0x80 | ((cp >> 6) & 0x3F));
            out_buf[out_idx++] = (uint8_t)(0x80 | (cp & 0x3F));
        } 
        else {
            // Out of valid Unicode range (> U+10FFFF) -> output replacement char U+FFFD
            out_buf[out_idx++] = 0xEF;
            out_buf[out_idx++] = 0xBF;
            out_buf[out_idx++] = 0xBD;
        }
    }

    // 4. Null-terminate the output buffer
    out_buf[out_idx] = '\0';

    // 5. Output length excludes the final '\0' byte
    if (out_str_len) {
        *out_str_len = out_idx;
    }

    // 6. Shrink buffer to actual used size (+1 byte for '\0')
    uint8_t * shrunk_buf = (uint8_t *)realloc(out_buf, out_idx + 1);
    return shrunk_buf ? shrunk_buf : out_buf;
}

uint8_t * utf8_from_utf32_malloc(uint32_t * str, size_t str_len, size_t * out_str_len)
{
    return utf8_text_from_utf32_malloc(str, str_len, out_str_len, 1);
}

// Omit BOM (Good for POSIX APIs, printf, network sockets, JSON)
char * text_from_utf32_malloc(uint32_t * str, size_t str_len, size_t * out_str_len)
{
    return (char*)utf8_text_from_utf32_malloc(str, str_len, out_str_len, 0);
}

uint32_t * utf32_from_memory_malloc(const char * str, size_t str_len, size_t * out_len, int * opt_out_bom_type) {
    if (!str) {
        if (out_len) *out_len = 0;
        return NULL;
    }

    const uint8_t *bytes = (const uint8_t *)str;
    size_t offset = 0;
    int detected_bom = BOM_NONE;

    // 1. Detect Byte Order Mark (BOM)
    if (str_len >= 4 && bytes[0] == 0xFF && bytes[1] == 0xFE && bytes[2] == 0x00 && bytes[3] == 0x00) {
        detected_bom = BOM_UTF32_LE;
        offset = 4;
    } else if (str_len >= 4 && bytes[0] == 0x00 && bytes[1] == 0x00 && bytes[2] == 0xFE && bytes[3] == 0xFF) {
        detected_bom = BOM_UTF32_BE;
        offset = 4;
    } else if (str_len >= 3 && bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF) {
        detected_bom = BOM_UTF8;
        offset = 3;
    } else if (str_len >= 2 && bytes[0] == 0xFF && bytes[1] == 0xFE) {
        detected_bom = BOM_UTF16_LE;
        offset = 2;
    } else if (str_len >= 2 && bytes[0] == 0xFE && bytes[1] == 0xFF) {
        detected_bom = BOM_UTF16_BE;
        offset = 2;
    }

    if (opt_out_bom_type) {
        *opt_out_bom_type = detected_bom;
    }

    const uint8_t *src = bytes + offset;
    size_t src_len = (str_len >= offset) ? (str_len - offset) : 0;

    // Allocate maximum possible capacity
    uint32_t *out_buf = (uint32_t *)malloc((src_len + 1) * sizeof(uint32_t));
    if (!out_buf) {
        if (out_len) *out_len = 0;
        return NULL;
    }

    size_t out_count = 0;
    size_t i = 0;

    // 2. Decode based on detected type
    if (detected_bom == BOM_UTF32_LE || detected_bom == BOM_UTF32_BE) {
        while (i + 3 < src_len) {
            uint32_t cp = 0;
            if (detected_bom == BOM_UTF32_LE) {
                cp = (uint32_t)src[i] | ((uint32_t)src[i+1] << 8) |
                     ((uint32_t)src[i+2] << 16) | ((uint32_t)src[i+3] << 24);
            } else {
                cp = ((uint32_t)src[i] << 24) | ((uint32_t)src[i+1] << 16) |
                     ((uint32_t)src[i+2] << 8) | (uint32_t)src[i+3];
            }
            if (cp == 0) break;
            out_buf[out_count++] = cp;
            i += 4;
        }
    } 
    else if (detected_bom == BOM_UTF16_LE || detected_bom == BOM_UTF16_BE) {
        while (i + 1 < src_len) {
            uint16_t w1 = 0;
            if (detected_bom == BOM_UTF16_LE) {
                w1 = (uint16_t)src[i] | ((uint16_t)src[i+1] << 8);
            } else {
                w1 = ((uint16_t)src[i] << 8) | (uint16_t)src[i+1];
            }
            i += 2;

            if (w1 == 0) break;

            if (w1 >= 0xD800 && w1 <= 0xDBFF) {
                if (i + 1 < src_len) {
                    uint16_t w2 = 0;
                    if (detected_bom == BOM_UTF16_LE) {
                        w2 = (uint16_t)src[i] | ((uint16_t)src[i+1] << 8);
                    } else {
                        w2 = ((uint16_t)src[i] << 8) | (uint16_t)src[i+1];
                    }
                    if (w2 >= 0xDC00 && w2 <= 0xDFFF) {
                        i += 2;
                        uint32_t cp = (((uint32_t)(w1 & 0x03FF) << 10) | (w2 & 0x03FF)) + 0x10000;
                        out_buf[out_count++] = cp;
                        continue;
                    }
                }
                out_buf[out_count++] = 0xFFFD; 
            } else {
                out_buf[out_count++] = w1;
            }
        }
    } 
    else {
        while (i < src_len) {
            uint32_t cp = 0;
            uint8_t c = src[i];

            if (c <= 0x7F) {
                cp = c;
                i += 1;
            } else if ((c & 0xE0) == 0xC0 && i + 1 < src_len) {
                cp = ((c & 0x1F) << 6) | (src[i+1] & 0x3F);
                i += 2;
            } else if ((c & 0xF0) == 0xE0 && i + 2 < src_len) {
                cp = ((c & 0x0F) << 12) | ((src[i+1] & 0x3F) << 6) | (src[i+2] & 0x3F);
                i += 3;
            } else if ((c & 0xF8) == 0xF0 && i + 3 < src_len) {
                cp = ((c & 0x07) << 18) | ((src[i+1] & 0x3F) << 12) | 
                     ((src[i+2] & 0x3F) << 6) | (src[i+3] & 0x3F);
                i += 4;
            } else {
                cp = 0xFFFD;
                i += 1;
            }
            out_buf[out_count++] = cp;
        }
    }

    out_buf[out_count] = 0;

    if (out_len) {
        *out_len = out_count;
    }

    uint32_t *shrunk_buf = (uint32_t *)realloc(out_buf, (out_count + 1) * sizeof(uint32_t));
    return shrunk_buf ? shrunk_buf : out_buf;
}

#ifdef __cplusplus
}
#endif

#endif /* STA_UTF32_BASE_IMPLEMENTATION */

/* ============================================================================
 * IMPLEMENTATION
 * ============================================================================ */

#ifdef STA_UTF32_FILE_IMPLEMENTATION
#undef STA_UTF32_FILE_IMPLEMENTATION

#ifdef __cplusplus
extern "C" {
#endif

/* Internal file I/O helpers */
static char * sta_internal_read_file_bytes(FILE * f, size_t * out_size) {
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    if (sz < 0) return NULL;
    fseek(f, 0, SEEK_SET);

    char * buf = (char *)malloc((size_t)sz + 1);
    if (!buf) return NULL;

    size_t read_bytes = fread(buf, 1, (size_t)sz, f);
    buf[read_bytes] = '\0';

    if (out_size) *out_size = read_bytes;
    return buf;
}

static int sta_internal_write_file_bytes(FILE * f, const uint8_t * bytes, size_t len) {
    if (!f || !bytes) return 0;
    size_t written = fwrite(bytes, 1, len, f);
    return written == len;
}

/* ----------------------------------------------------------------------------
 * Load APIs
 * ---------------------------------------------------------------------------- */

uint32_t * a_utf32_load_file_malloc(const char * path, size_t * out_len, int * opt_out_bom_type) {
    if (!path) return NULL;

    FILE * f = fopen(path, "rb");
    if (!f) return NULL;

    size_t raw_len = 0;
    char * raw_buf = sta_internal_read_file_bytes(f, &raw_len);
    fclose(f);

    if (!raw_buf) return NULL;

    uint32_t * result = utf32_from_memory_malloc(raw_buf, raw_len, out_len, opt_out_bom_type);
    free(raw_buf);
    return result;
}

uint32_t * w_utf32_load_file_malloc(const wchar_t * path, size_t * out_len, int * opt_out_bom_type) {
    if (!path) return NULL;

#if defined(_WIN32)
    FILE * f = _wfopen(path, L"rb");
#else
    FILE * f = wfopen(path, L"rb");
#endif
    if (!f) return NULL;

    size_t raw_len = 0;
    char * raw_buf = sta_internal_read_file_bytes(f, &raw_len);
    fclose(f);

    if (!raw_buf) return NULL;

    uint32_t * result = utf32_from_memory_malloc(raw_buf, raw_len, out_len, opt_out_bom_type);
    free(raw_buf);
    return result;
}

/* ----------------------------------------------------------------------------
 * Save APIs (Writes UTF-8 with BOM using base library conversion)
 * ---------------------------------------------------------------------------- */

int a_utf32_save_file(const char * path, const uint32_t * text, size_t text_len) {
    if (!path || !text) return 0;

    size_t utf8_len = 0;
    uint8_t * utf8_bytes = utf8_from_utf32_malloc((uint32_t *)text, text_len, &utf8_len);
    if (!utf8_bytes) return 0;

    FILE * f = fopen(path, "wb");
    if (!f) {
        free(utf8_bytes);
        return 0;
    }

    int success = sta_internal_write_file_bytes(f, utf8_bytes, utf8_len);
    fclose(f);
    free(utf8_bytes);

    return success;
}

int w_utf32_save_file(const wchar_t * path, const uint32_t * text, size_t text_len) {
    if (!path || !text) return 0;

    size_t utf8_len = 0;
    uint8_t * utf8_bytes = utf8_from_utf32_malloc((uint32_t *)text, text_len, &utf8_len);
    if (!utf8_bytes) return 0;

#if defined(_WIN32)
    FILE * f = _wfopen(path, L"wb");
#else
    FILE * f = wfopen(path, L"wb");
#endif
    if (!f) {
        free(utf8_bytes);
        return 0;
    }

    int success = sta_internal_write_file_bytes(f, utf8_bytes, utf8_len);
    fclose(f);
    free(utf8_bytes);

    return success;
}

#ifdef __cplusplus
}
#endif

#endif /* STA_UTF32_FILE_IMPLEMENTATION */
