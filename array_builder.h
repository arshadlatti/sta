#ifndef ARRAY_BUILDER_H
#define ARRAY_BUILDER_H

#include <stdio.h>
#include <stdlib.h>

/* Default growth increment when builder->growth == 0 */
#define ARRAY_BUILDER_DEFAULT_GROWTH 4096

/* =========================================================================
 * 1. STANDARD VALUE / STRUCT ARRAY BUILDER MACROS
 * ========================================================================= */

#define array_builder_h(NAME, TYPE)                                            \
typedef void (*NAME##_term_f)(TYPE *elem);                                     \
                                                                               \
typedef struct NAME {                                                          \
    TYPE *vec;                                                                 \
    size_t count;                                                              \
    size_t capacity;                                                           \
    size_t growth;                                                             \
    int is_error;                                                              \
    NAME##_term_f term_function;                                               \
} NAME;                                                                        \
                                                                               \
NAME* NAME##_new(NAME##_term_f term_function);                                 \
void NAME##_delete(NAME *builder);                                             \
int NAME##_add(NAME *builder, const TYPE *x);                                  \
int NAME##_add_v(NAME *builder, TYPE x);

#define array_builder_c(NAME, TYPE)                                            \
NAME* NAME##_new(NAME##_term_f term_function) {                                \
    NAME *builder = (NAME*)malloc(sizeof(NAME));                               \
    if (!builder) return NULL;                                                 \
    builder->vec = NULL;                                                       \
    builder->count = 0;                                                        \
    builder->capacity = 0;                                                     \
    builder->growth = 0;                                                       \
    builder->is_error = 0;                                                     \
    builder->term_function = term_function;                                    \
    return builder;                                                            \
}                                                                              \
                                                                               \
void NAME##_delete(NAME *builder) {                                            \
    size_t i;                                                                  \
    if (!builder) return;                                                      \
    if (builder->vec) {                                                        \
        if (builder->term_function) {                                          \
            for (i = 0; i < builder->count; ++i) {                             \
                builder->term_function(&builder->vec[i]);                      \
            }                                                                  \
        }                                                                      \
        free(builder->vec);                                                    \
    }                                                                          \
    free(builder);                                                             \
}                                                                              \
                                                                               \
int NAME##_add(NAME *builder, const TYPE *x) {                                 \
    if (!builder) return 0;                                                    \
    if (builder->count >= builder->capacity) {                                 \
        size_t step = (builder->growth > 0) ? builder->growth                 \
                                            : ARRAY_BUILDER_DEFAULT_GROWTH;    \
        size_t new_cap = builder->capacity + step;                             \
        TYPE *new_vec = (TYPE*)realloc(builder->vec, new_cap * sizeof(TYPE));  \
        if (!new_vec) {                                                        \
            builder->is_error = 1;                                             \
            if (builder->term_function && x) {                                 \
                builder->term_function((TYPE*)x);                              \
            }                                                                  \
            return 0;                                                          \
        }                                                                      \
        builder->vec = new_vec;                                                \
        builder->capacity = new_cap;                                           \
    }                                                                          \
    builder->vec[builder->count++] = *x;                                       \
    return 1;                                                                  \
}                                                                              \
                                                                               \
int NAME##_add_v(NAME *builder, TYPE x) {                                      \
    return NAME##_add(builder, &x);                                            \
}

#define array_builder_hc(NAME, TYPE)                                           \
array_builder_h(NAME, TYPE)                                                    \
array_builder_c(NAME, TYPE)


/* =========================================================================
 * 2. POINTER ARRAY BUILDER MACROS
 * ========================================================================= */

#define pointer_array_builder_h(NAME, TYPE)                                    \
typedef void (*NAME##_delete_f)(TYPE *elem);                                   \
                                                                               \
typedef struct NAME {                                                          \
    TYPE **vec;                                                                \
    size_t count;                                                              \
    size_t capacity;                                                           \
    size_t growth;                                                             \
    int is_error;                                                              \
    NAME##_delete_f delete_function;                                           \
} NAME;                                                                        \
                                                                               \
NAME* NAME##_new(NAME##_delete_f delete_function);                             \
void NAME##_delete(NAME *builder);                                             \
int NAME##_add(NAME *builder, TYPE *x);

#define pointer_array_builder_c(NAME, TYPE)                                    \
NAME* NAME##_new(NAME##_delete_f delete_function) {                            \
    NAME *builder = (NAME*)malloc(sizeof(NAME));                               \
    if (!builder) return NULL;                                                 \
    builder->vec = NULL;                                                       \
    builder->count = 0;                                                        \
    builder->capacity = 0;                                                     \
    builder->growth = 0;                                                       \
    builder->is_error = 0;                                                     \
    builder->delete_function = delete_function;                                \
    return builder;                                                            \
}                                                                              \
                                                                               \
void NAME##_delete(NAME *builder) {                                            \
    size_t i;                                                                  \
    if (!builder) return;                                                      \
    if (builder->vec) {                                                        \
        if (builder->delete_function) {                                        \
            for (i = 0; i < builder->count; ++i) {                             \
                if (builder->vec[i]) {                                         \
                    builder->delete_function(builder->vec[i]);                 \
                }                                                              \
            }                                                                  \
        }                                                                      \
        free(builder->vec);                                                    \
    }                                                                          \
    free(builder);                                                             \
}                                                                              \
                                                                               \
int NAME##_add(NAME *builder, TYPE *x) {                                       \
    if (!builder) return 0;                                                    \
    if (builder->count >= builder->capacity) {                                 \
        size_t step = (builder->growth > 0) ? builder->growth                 \
                                            : ARRAY_BUILDER_DEFAULT_GROWTH;    \
        size_t new_cap = builder->capacity + step;                             \
        TYPE **new_vec = (TYPE**)realloc(builder->vec, new_cap * sizeof(TYPE*));\
        if (!new_vec) {                                                        \
            builder->is_error = 1;                                             \
            if (builder->delete_function && x) {                               \
                builder->delete_function(x);                                   \
            }                                                                  \
            return 0;                                                          \
        }                                                                      \
        builder->vec = new_vec;                                                \
        builder->capacity = new_cap;                                           \
    }                                                                          \
    builder->vec[builder->count++] = x;                                        \
    return 1;                                                                  \
}

#define pointer_array_builder_hc(NAME, TYPE)                                   \
pointer_array_builder_h(NAME, TYPE)                                            \
pointer_array_builder_c(NAME, TYPE)

#endif /* ARRAY_BUILDER_H */

// array_builder Design by Arshad Latti with help of Gemini and implemented by Gemini