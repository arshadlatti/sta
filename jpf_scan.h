#ifndef JPF_SCAN_H
#define JPF_SCAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// Error Codes
typedef enum {
    JPF_SUCCESS           =  0,
    JPF_ERR_FILE_OPEN     = -1,
    JPF_ERR_FILE_READ     = -2,
    JPF_ERR_OUT_OF_MEMORY = -3,
    JPF_ERR_INVALID_PARAM = -4,
    JPF_ERR_NO_JPEGS      = -5
} jpf_error_t;

// Result Location Container (Offset & Byte Size)
typedef struct {
    int start; // File byte offset of 0xFFD8
    int size;  // Total byte size including 0xFFD9
} jpf_page_loc_t;

/**
 * Scans a PDF file and allocates an array containing offset and size for valid JPEGs.
 * 
 * @param pdf_filename Path to the PDF file.
 * @param out_page_count Output pointer for total valid JPEG images found.
 * @param error_code Output pointer for jpf_error_t status.
 * @return Pointer to dynamically allocated jpf_page_loc_t array (caller frees with free()), or NULL on failure.
 */
jpf_page_loc_t * jpf_scan_malloc(const char * pdf_filename, size_t * out_page_count, int * error_code);

#ifdef JPF_SCAN_IMPLEMENTATION

// Helper: Fast memmem equivalent for buffer searching
static const unsigned char * jpf_internal_memmem(const unsigned char *haystack, size_t haystack_len,
                                                const unsigned char *needle, size_t needle_len) {
    if (!haystack || !needle || needle_len == 0 || haystack_len < needle_len) return NULL;
    for (size_t i = 0; i <= haystack_len - needle_len; i++) {
        if (haystack[i] == needle[0] && memcmp(haystack + i, needle, needle_len) == 0) {
            return haystack + i;
        }
    }
    return NULL;
}

// Helper: Parse positive integer from string
static int jpf_internal_parse_int(const char *str) {
    int val = 0;
    while (*str >= '0' && *str <= '9') {
        val = val * 10 + (*str - '0');
        str++;
    }
    return val;
}

// Helper: Extract /Length [int] from a PDF stream dictionary snippet
static int jpf_internal_extract_stream_length(const unsigned char *dict_start, size_t dict_len) {
    const unsigned char *length_key = jpf_internal_memmem(dict_start, dict_len, (const unsigned char *)"/Length", 7);
    if (!length_key) return -1;

    const unsigned char *p = length_key + 7;
    const unsigned char *end = dict_start + dict_len;

    // Skip whitespace and potential indirect object syntax
    while (p < end && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')) p++;

    if (p < end && *p >= '0' && *p <= '9') {
        return jpf_internal_parse_int((const char *)p);
    }
    return -1;
}

// Main API Implementation
jpf_page_loc_t * jpf_scan_malloc(const char * pdf_filename, size_t * out_page_count, int * error_code) {
    if (out_page_count) *out_page_count = 0;

    if (!pdf_filename || !out_page_count || !error_code) {
        if (error_code) *error_code = JPF_ERR_INVALID_PARAM;
        return NULL;
    }

    FILE *f = fopen(pdf_filename, "rb");
    if (!f) {
        *error_code = JPF_ERR_FILE_OPEN;
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long file_size_long = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (file_size_long <= 0) {
        fclose(f);
        *error_code = JPF_ERR_FILE_READ;
        return NULL;
    }

    size_t file_size = (size_t)file_size_long;
    unsigned char *buf = (unsigned char *)malloc(file_size);
    if (!buf) {
        fclose(f);
        *error_code = JPF_ERR_OUT_OF_MEMORY;
        return NULL;
    }

    if (fread(buf, 1, file_size, f) != file_size) {
        free(buf);
        fclose(f);
        *error_code = JPF_ERR_FILE_READ;
        return NULL;
    }
    fclose(f);

    size_t capacity = 8;
    size_t count = 0;
    jpf_page_loc_t *locations = (jpf_page_loc_t *)malloc(capacity * sizeof(jpf_page_loc_t));
    if (!locations) {
        free(buf);
        *error_code = JPF_ERR_OUT_OF_MEMORY;
        return NULL;
    }

    const unsigned char *cursor = buf;
    size_t remaining = file_size;

    while (remaining > 0) {
        // Locate PDF 'stream' keyword
        const unsigned char *stream_kw = jpf_internal_memmem(cursor, remaining, (const unsigned char *)"stream", 6);
        if (!stream_kw) break;

        size_t stream_kw_offset = (size_t)(stream_kw - buf);

        // Advance cursor past 'stream' keyword and newline (\r\n or \n)
        const unsigned char *data_start = stream_kw + 6;
        if ((size_t)(data_start - buf) < file_size && *data_start == '\r') data_start++;
        if ((size_t)(data_start - buf) < file_size && *data_start == '\n') data_start++;

        size_t raw_data_offset = (size_t)(data_start - buf);

        // Check if raw data starts with JPEG SOI marker (0xFF, 0xD8)
        if (raw_data_offset + 2 <= file_size && data_start[0] == 0xFF && data_start[1] == 0xD8) {

            // Search backwards for stream dictionary bounds '<<'
            size_t dict_search_start = (stream_kw_offset > 512) ? (stream_kw_offset - 512) : 0;
            size_t dict_len = stream_kw_offset - dict_search_start;
            int stream_len = jpf_internal_extract_stream_length(buf + dict_search_start, dict_len);

            size_t actual_jpeg_size = 0;

            if (stream_len > 0 && (raw_data_offset + (size_t)stream_len <= file_size)) {
                // Verify EOI marker (0xFF, 0xD9) near expected length end
                const unsigned char *expected_end = data_start + stream_len;
                if (expected_end[-2] == 0xFF && expected_end[-1] == 0xD9) {
                    actual_jpeg_size = (size_t)stream_len;
                } else if (expected_end[-3] == 0xFF && expected_end[-2] == 0xD9) { // Handle potential trailing newline inside stream
                    actual_jpeg_size = (size_t)stream_len - 1;
                }
            }

            // Fallback scan for EOI marker (0xFF, 0xD9) if length was omitted or invalid
            if (actual_jpeg_size == 0) {
                const unsigned char *endstream_kw = jpf_internal_memmem(data_start, file_size - raw_data_offset, (const unsigned char *)"endstream", 9);
                if (endstream_kw) {
                    size_t search_window = (size_t)(endstream_kw - data_start);
                    for (size_t i = search_window; i >= 2; i--) {
                        if (data_start[i - 2] == 0xFF && data_start[i - 1] == 0xD9) {
                            actual_jpeg_size = i;
                            break;
                        }
                    }
                }
            }

            // Record valid JPEG location
            if (actual_jpeg_size > 0) {
                if (count >= capacity) {
                    capacity *= 2;
                    jpf_page_loc_t *new_locs = (jpf_page_loc_t *)realloc(locations, capacity * sizeof(jpf_page_loc_t));
                    if (!new_locs) {
                        free(locations);
                        free(buf);
                        *error_code = JPF_ERR_OUT_OF_MEMORY;
                        return NULL;
                    }
                    locations = new_locs;
                }

                locations[count].start = (int)raw_data_offset;
                locations[count].size = (int)actual_jpeg_size;
                count++;

                // Fast forward cursor past this stream payload
                cursor = data_start + actual_jpeg_size;
                remaining = file_size - (size_t)(cursor - buf);
                continue;
            }
        }

        // Advance cursor past current 'stream' keyword
        cursor = stream_kw + 6;
        remaining = file_size - (size_t)(cursor - buf);
    }

    free(buf);

    if (count == 0) {
        free(locations);
        *error_code = JPF_ERR_NO_JPEGS;
        return NULL;
    }

    *out_page_count = count;
    *error_code = JPF_SUCCESS;
    return locations;
}

#endif // JPF_SCAN_IMPLEMENTATION

#endif // JPF_SCAN_H

// jpf_scan Design by Arshad Latti with help of Gemini and implemented by Gemini