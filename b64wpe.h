#ifndef B64WPE_H
#define B64WPE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Base 64 Web Path Encoding (B64WPE) API
 */
char * b64wpe_encode_malloc(const uint32_t * str, size_t str_len, size_t * out_len);
uint32_t * b64wpe_decode_malloc(const char * str, size_t str_len, size_t * out_len);

#ifdef __cplusplus
}
#endif

#endif /* B64WPE_H */

/* ============================================================================
 * IMPLEMENTATION
 * ============================================================================ */
#ifdef B64WPE_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>

/*
 * Base62 Symbol Table
 */
static const char B64WPE_ALPHABET[62] = {
    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    'a','b','c','d','e','f','g','h','i','j','k','l','m',
    'n','o','p','q','r','s','t','u','v','w','x','y','z',
    '0','1','2','3','4','5','6','7','8','9'
};

/*
 * Tier numerical boundary offsets (inclusive ranges):
 */
static const uint64_t B64WPE_TIER_OFFSETS[7] = {
    0ULL,
    0ULL,
    62ULL,
    3906ULL,
    242234ULL,
    15018570ULL,
    931151402ULL
};

static inline int b64wpe_char_to_val(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    return -1;
}

static inline size_t b64wpe_get_tier(uint64_t val) {
    if (val <= 61ULL) return 1;
    if (val <= 3905ULL) return 2;
    if (val <= 242233ULL) return 3;
    if (val <= 15018569ULL) return 4;
    if (val <= 931151401ULL) return 5;
    return 6;
}

static inline int b64wpe_is_safe_literal(uint32_t cp) {
    return ((cp >= 'A' && cp <= 'Z') ||
            (cp >= 'a' && cp <= 'z') ||
            (cp >= '0' && cp <= '9') ||
            cp == '.' || cp == '/');
}

char * b64wpe_encode_malloc(const uint32_t * str, size_t str_len, size_t * out_len) {
    if (!str && str_len > 0) {
        if (out_len) *out_len = 0;
        return NULL;
    }

    size_t cap = str_len * 10 + 16;
    char *out = (char *)malloc(cap);
    if (!out) {
        if (out_len) *out_len = 0;
        return NULL;
    }

    size_t out_pos = 0;
    size_t i = 0;

    while (i < str_len) {
        uint32_t cp = str[i];

        /* Rule 1: Literal Space (' ') -> '_' */
        if (cp == ' ') {
            out[out_pos++] = '_';
            i++;
            continue;
        }

        /* Rule 2: Safe Literals [A-Za-z0-9./] passed through unchanged */
        if (b64wpe_is_safe_literal(cp)) {
            out[out_pos++] = (char)cp;
            i++;
            continue;
        }

        /* Rule 3: Escape Delimiter ('-') -> '--' */
        if (cp == '-') {
            out[out_pos++] = '-';
            out[out_pos++] = '-';
            i++;
            continue;
        }

        /* Rule 4 & 5: Bijective Tier Offsets & Streaming Vectorization */
        size_t tier = b64wpe_get_tier(cp);
        size_t run_len = 1;
        while (i + run_len < str_len) {
            uint32_t next_cp = str[i + run_len];
            if (next_cp == ' ' || next_cp == '-' || b64wpe_is_safe_literal(next_cp)) {
                break;
            }
            if (b64wpe_get_tier(next_cp) != tier) {
                break;
            }
            run_len++;
        }

        if (out_pos + run_len * tier + 16 > cap) {
            cap = (cap + run_len * tier + 16) * 2;
            char *new_out = (char *)realloc(out, cap);
            if (!new_out) {
                free(out);
                if (out_len) *out_len = 0;
                return NULL;
            }
            out = new_out;
        }

        if (run_len > 1) {
            /* Streaming Vectorized Block: -_[Tier][Payload]- */
            out[out_pos++] = '-';
            out[out_pos++] = '_';
            out[out_pos++] = (char)('0' + tier);

            for (size_t r = 0; r < run_len; r++) {
                uint64_t val = str[i + r] - B64WPE_TIER_OFFSETS[tier];
                char buf[8];
                for (size_t p = 0; p < tier; p++) {
                    buf[tier - 1 - p] = B64WPE_ALPHABET[val % 62];
                    val /= 62;
                }
                memcpy(&out[out_pos], buf, tier);
                out_pos += tier;
            }
            out[out_pos++] = '-';
        } else {
            /* Single Token Tier Block: -[Payload]- */
            out[out_pos++] = '-';
            uint64_t val = cp - B64WPE_TIER_OFFSETS[tier];
            char buf[8];
            for (size_t p = 0; p < tier; p++) {
                buf[tier - 1 - p] = B64WPE_ALPHABET[val % 62];
                val /= 62;
            }
            memcpy(&out[out_pos], buf, tier);
            out_pos += tier;
            out[out_pos++] = '-';
        }

        i += run_len;
    }

    out[out_pos] = '\0';
    if (out_len) {
        *out_len = out_pos;
    }
    return out;
}

uint32_t * b64wpe_decode_malloc(const char * str, size_t str_len, size_t * out_len) {
    if (!str || !out_len) return NULL;

    uint32_t *out = (uint32_t *)malloc((str_len + 1) * sizeof(uint32_t));
    if (!out) {
        *out_len = 0;
        return NULL;
    }

    size_t out_pos = 0;
    size_t i = 0;

    while (i < str_len) {
        char c = str[i];

        /* Rule 1: Space translation '_' -> ' ' */
        if (c == '_') {
            out[out_pos++] = ' ';
            i++;
            continue;
        }

        if (c == '-') {
            if (i + 1 < str_len && str[i + 1] == '-') {
                /* Rule 3 / Double Hyphen Rule: '--' -> '-' */
                out[out_pos++] = '-';
                i += 2;
                continue;
            }

            if (i + 1 < str_len && str[i + 1] == '_') {
                /* Vectorized Engine Block Parsing */
                i += 2;
                if (i >= str_len) break;

                size_t tier = str[i++] - '0';
                if (tier < 1 || tier > 6) {
                    free(out);
                    *out_len = 0;
                    return NULL;
                }

                while (i < str_len && str[i] != '-') {
                    if (i + tier > str_len) {
                        free(out);
                        *out_len = 0;
                        return NULL;
                    }
                    uint64_t val = 0;
                    for (size_t p = 0; p < tier; p++) {
                        int v = b64wpe_char_to_val(str[i + p]);
                        if (v < 0) {
                            free(out);
                            *out_len = 0;
                            return NULL;
                        }
                        val = val * 62 + (uint64_t)v;
                    }
                    out[out_pos++] = (uint32_t)(val + B64WPE_TIER_OFFSETS[tier]);
                    i += tier;
                }
                if (i < str_len && str[i] == '-') {
                    i++;
                }
                continue;
            }

            /* Single Token Tier Parsing */
            i++;
            size_t start = i;
            while (i < str_len && str[i] != '-') {
                i++;
            }
            size_t tier = i - start;
            if (tier < 1 || tier > 6) {
                free(out);
                *out_len = 0;
                return NULL;
            }

            uint64_t val = 0;
            for (size_t p = 0; p < tier; p++) {
                int v = b64wpe_char_to_val(str[start + p]);
                if (v < 0) {
                    free(out);
                    *out_len = 0;
                    return NULL;
                }
                val = val * 62 + (uint64_t)v;
            }
            out[out_pos++] = (uint32_t)(val + B64WPE_TIER_OFFSETS[tier]);

            if (i < str_len && str[i] == '-') {
                i++;
            }
            continue;
        }

        /* Safe Literals [A-Za-z0-9./] */
        out[out_pos++] = (uint32_t)c;
        i++;
    }

    *out_len = out_pos;
    return out;
}

#endif /* B64WPE_IMPLEMENTATION */

// b64wpe Design by Arshad Latti with help of Gemini 2.5 and implemented by Gemini 2.5