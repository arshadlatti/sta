/*
 * hot_buffer.h - Strict C89/C99 Standard Library Implementation
 *
 * Usage:
 *   #define HOT_BUFFER_IMPLEMENTATION
 *   #include "hot_buffer.h"
 */

#ifndef HOT_BUFFER_H
#define HOT_BUFFER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef uint64_t u64;
typedef uint32_t u32;

typedef struct {
    uint8_t* data;
    u64 capacity;
    u64 offset;
    void* raw_ptr; /* Tracks raw malloc pointer for cleanup when aligned loading */
} hot_buffer_t;

/* Access relative index as type pointer */
#define HB_PTR(hb, index, type) ((type*)((hb)->data + (index)))

/* Access alignment header stored directly at offset 0 */
#define HB_AH(hb) (*(u32*)((hb)->data))

#ifdef __cplusplus
extern "C" {
#endif

hot_buffer_t* hot_buffer_new(u64 initial_cap);
void          hot_buffer_delete(hot_buffer_t* hb);
u64           hot_buffer_alloc(hot_buffer_t* hb, u64 count, u64 item_size, u64 align);
int           hot_buffer_save(hot_buffer_t* hb, const char* filepath);
hot_buffer_t* hot_buffer_load(const char* filepath);

#ifdef __cplusplus
}
#endif
// A Project by Arshad Latti with help of "Gemini 3.6 flash"(Implementation)
#endif /* HOT_BUFFER_H */

/* ============================================================================
   IMPLEMENTATION
   ============================================================================ */
#ifdef HOT_BUFFER_IMPLEMENTATION

#define HB_ALIGN_UP(size, align) (((size) + ((align) - 1)) & ~((align) - 1))

/* Standard C89/C99 aligned memory allocation using pure malloc */
static uint8_t* hb_malloc_aligned(u64 size, u32 align, void** out_raw_ptr) {
    u32 actual_align = (align < sizeof(void*)) ? (u32)sizeof(void*) : align;
    u64 total_size = size + actual_align + sizeof(void*);
    uint8_t* raw_ptr = (uint8_t*)malloc((size_t)total_size);
    uint8_t* aligned_ptr;
    uintptr_t unaligned_addr;

    if (!raw_ptr) {
        *out_raw_ptr = NULL;
        return NULL;
    }

    /* Shift address forward to leave space for storing raw_ptr pointer */
    unaligned_addr = (uintptr_t)(raw_ptr + sizeof(void*));
    aligned_ptr = (uint8_t*)HB_ALIGN_UP(unaligned_addr, (uintptr_t)actual_align);

    /* Store original malloc pointer immediately preceding aligned boundary */
    ((void**)aligned_ptr)[-1] = raw_ptr;

    *out_raw_ptr = raw_ptr;
    return aligned_ptr;
}

hot_buffer_t* hot_buffer_new(u64 initial_cap) {
    hot_buffer_t* hb = (hot_buffer_t*)malloc(sizeof(hot_buffer_t));
    if (!hb) return NULL;

    hb->capacity = initial_cap < 4096 ? 4096 : initial_cap;
    hb->raw_ptr = NULL;
    hb->data = (uint8_t*)malloc((size_t)hb->capacity);
    if (!hb->data) {
        free(hb);
        return NULL;
    }

    HB_AH(hb) = 4; /* Default baseline alignment */
    hb->offset = sizeof(u32);

    return hb;
}

void hot_buffer_delete(hot_buffer_t* hb) {
    if (!hb) return;
    
    /* Clean up either aligned loading buffer or standard dynamic buffer */
    if (hb->raw_ptr) {
        free(hb->raw_ptr);
    } else if (hb->data) {
        free(hb->data);
    }
    
    free(hb);
}

u64 hot_buffer_alloc(hot_buffer_t* hb, u64 count, u64 item_size, u64 align) {
    u64 total_size;
    u64 effective_align;
    u64 payload_idx;
    u64 needed;

    if (!hb) return 0;

    total_size = count * item_size;
    effective_align = (align < 1) ? 1 : align;

    /* Track highest alignment inside stream at offset 0 */
    if ((u32)effective_align > HB_AH(hb)) {
        HB_AH(hb) = (u32)effective_align;
    }

    payload_idx = HB_ALIGN_UP(hb->offset, effective_align);
    needed = payload_idx + total_size;

    if (needed > hb->capacity) {
        u64 new_cap = hb->capacity * 2;
        uint8_t* new_data;

        while (new_cap < needed) new_cap *= 2;

        new_data = (uint8_t*)realloc(hb->data, (size_t)new_cap);
        if (!new_data) return 0;

        hb->data = new_data;
        hb->capacity = new_cap;
    }

    hb->offset = needed;
    return payload_idx;
}

int hot_buffer_save(hot_buffer_t* hb, const char* filepath) {
    FILE* f;
    size_t written;

    if (!hb || !hb->data) return 0;

    f = fopen(filepath, "wb");
    if (!f) return 0;

    written = fwrite(hb->data, 1, (size_t)hb->offset, f);
    fclose(f);

    return written == (size_t)hb->offset;
}

hot_buffer_t* hot_buffer_load(const char* filepath) {
    FILE* f;
    u64 file_size;
    u32 ah = 0;
    u32 base_align;
    size_t read_bytes;
    hot_buffer_t* hb;

    f = fopen(filepath, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    file_size = (u64)ftell(f);
    fseek(f, 0, SEEK_SET);

    if (file_size < sizeof(u32)) {
        fclose(f);
        return NULL;
    }

    if (fread(&ah, sizeof(u32), 1, f) != 1) {
        fclose(f);
        return NULL;
    }
    fseek(f, 0, SEEK_SET);

    hb = (hot_buffer_t*)malloc(sizeof(hot_buffer_t));
    if (!hb) {
        fclose(f);
        return NULL;
    }

    hb->capacity = file_size;
    hb->offset = file_size;

    base_align = (ah < 8) ? 8 : ah;
    
    /* Pure stdlib alignment allocation */
    hb->data = hb_malloc_aligned(file_size, base_align, &hb->raw_ptr);

    if (!hb->data) {
        free(hb);
        fclose(f);
        return NULL;
    }

    read_bytes = fread(hb->data, 1, (size_t)file_size, f);
    fclose(f);

    if (read_bytes != (size_t)file_size) {
        free(hb->raw_ptr);
        free(hb);
        return NULL;
    }

    return hb;
}

#endif /* HOT_BUFFER_IMPLEMENTATION */