#ifndef STA_ALLOCATOR_H
#define STA_ALLOCATOR_H

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct a_allocator_s a_allocator;

/* Allocation function prototype passing self allocator instance */
typedef void* (*sta_alloc_fn)(const a_allocator* allocator, size_t size);

/* Free function prototype passing self allocator instance */
typedef void (*sta_free_fn)(const a_allocator* allocator, void* ptr);

/* Terminate/Cleanup function prototype passing self allocator instance */
typedef void (*sta_term_fn)(a_allocator* allocator);

/* Allocator Interface Struct */
struct a_allocator_s {
    sta_alloc_fn alloc;
    sta_free_fn  free;
    sta_term_fn  term;
    void*        ctx;
};

/* Helper allocation wrapper */
static inline void* a_allocator_alloc(const a_allocator* allocator, size_t size) {
    if (allocator && allocator->alloc) {
        return allocator->alloc(allocator, size);
    }
    return NULL;
}

/* Helper free wrapper */
static inline void a_allocator_free(const a_allocator* allocator, void* ptr) {
    if (allocator && allocator->free) {
        allocator->free(allocator, ptr);
    }
}

/* Helper termination wrapper */
static inline void a_allocator_term(a_allocator* allocator) {
    if (allocator && allocator->term) {
        allocator->term(allocator);
    }
}

/* Internal stdlib wrappers */
static inline void* sta_internal_stdc_alloc(const a_allocator* allocator, size_t size) {
    (void)allocator;
    return malloc(size);
}

static inline void sta_internal_stdc_free(const a_allocator* allocator, void* ptr) {
    (void)allocator;
    free(ptr);
}

static inline void sta_internal_stdc_term(a_allocator* allocator) {
    (void)allocator;
    /* No-op for standard malloc/free wrapper */
}

/* Initializes an existing a_allocator instance using standard malloc/free */
static inline void a_allocator_init_std(a_allocator* allocator) {
    if (allocator) {
        allocator->alloc = sta_internal_stdc_alloc;
        allocator->free  = sta_internal_stdc_free;
        allocator->term  = sta_internal_stdc_term;
        allocator->ctx   = NULL;
    }
}

#ifdef __cplusplus
}
#endif

#endif /* STA_ALLOCATOR_H */

// A Project Design by Arshad Latti with help of Gemini and implemented by Gemini