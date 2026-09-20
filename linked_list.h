#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>

/* =========================================================================
 * 1. STANDARD VALUE / STRUCT LINKED LIST MACROS
 * ========================================================================= */

#define linked_list_h(NAME, TYPE)                                              \
typedef void (*NAME##_term_f)(TYPE *elem);                                     \
                                                                               \
typedef struct NAME##_node {                                                   \
    TYPE data;                                                                 \
    struct NAME##_node *prev;                                                  \
    struct NAME##_node *next;                                                  \
} NAME##_node;                                                                 \
                                                                               \
typedef struct NAME {                                                          \
    NAME##_node *head;                                                         \
    NAME##_node *tail;                                                         \
    size_t count;                                                              \
    int is_error;                                                              \
    NAME##_term_f term_function;                                               \
} NAME;                                                                        \
                                                                               \
/* Creates a new linked list. */                                               \
NAME* NAME##_new(NAME##_term_f term_function);                                 \
                                                                               \
/* Frees all list nodes and the list container itself. */                      \
void NAME##_delete(NAME *list);                                                \
                                                                               \
/* Appends a value to the tail of the list. */                                 \
NAME##_node* NAME##_add(NAME *list, const TYPE *x);                            \
NAME##_node* NAME##_add_v(NAME *list, TYPE x);                                 \
                                                                               \
/* Prepends a value to the head of the list. */                                \
NAME##_node* NAME##_add_first(NAME *list, const TYPE *x);                      \
NAME##_node* NAME##_add_first_v(NAME *list, TYPE x);                           \
                                                                               \
/* Inserts a value before a target node. */                                    \
NAME##_node* NAME##_add_before(NAME *list, NAME##_node *target, const TYPE *x);\
NAME##_node* NAME##_add_before_v(NAME *list, NAME##_node *target, TYPE x);      \
                                                                               \
/* Inserts a value after a target node. */                                     \
NAME##_node* NAME##_add_after(NAME *list, NAME##_node *target, const TYPE *x); \
NAME##_node* NAME##_add_after_v(NAME *list, NAME##_node *target, TYPE x);       \
                                                                               \
/* Removes a specific node from the list and frees its resources. */           \
int NAME##_remove(NAME *list, NAME##_node *node);  \
/* Returns a pointer to the element data at the specified 0-based index. */    \
TYPE* NAME##_get_at(const NAME *list, size_t index);

#define linked_list_c(NAME, TYPE)                                              \
NAME* NAME##_new(NAME##_term_f term_function) {                                \
    NAME *list = (NAME*)malloc(sizeof(NAME));                                  \
    if (!list) return NULL;                                                    \
    list->head = NULL;                                                         \
    list->tail = NULL;                                                         \
    list->count = 0;                                                           \
    list->is_error = 0;                                                        \
    list->term_function = term_function;                                       \
    return list;                                                               \
}                                                                              \
                                                                               \
void NAME##_delete(NAME *list) {                                               \
    NAME##_node *curr;                                                         \
    NAME##_node *next_node;                                                    \
    if (!list) return;                                                         \
    curr = list->head;                                                         \
    while (curr) {                                                             \
        next_node = curr->next;                                                \
        if (list->term_function) {                                             \
            list->term_function(&curr->data);                                  \
        }                                                                      \
        free(curr);                                                            \
        curr = next_node;                                                      \
    }                                                                          \
    free(list);                                                                \
}                                                                              \
                                                                               \
NAME##_node* NAME##_add_first(NAME *list, const TYPE *x) {                     \
    NAME##_node *node;                                                         \
    if (!list || !x) return NULL;                                              \
    node = (NAME##_node*)malloc(sizeof(NAME##_node));                          \
    if (!node) {                                                               \
        list->is_error = 1;                                                    \
        if (list->term_function) {                                             \
            list->term_function((TYPE*)x);                                     \
        }                                                                      \
        return NULL;                                                           \
    }                                                                          \
    node->data = *x;                                                           \
    node->prev = NULL;                                                         \
    node->next = list->head;                                                   \
    if (list->head) {                                                          \
        list->head->prev = node;                                               \
    } else {                                                                   \
        list->tail = node;                                                     \
    }                                                                          \
    list->head = node;                                                         \
    list->count++;                                                             \
    return node;                                                               \
}                                                                              \
                                                                               \
NAME##_node* NAME##_add_first_v(NAME *list, TYPE x) {                          \
    return NAME##_add_first(list, &x);                                         \
}                                                                              \
                                                                               \
NAME##_node* NAME##_add(NAME *list, const TYPE *x) {                           \
    NAME##_node *node;                                                         \
    if (!list || !x) return NULL;                                              \
    if (!list->tail) {                                                         \
        return NAME##_add_first(list, x);                                      \
    }                                                                          \
    node = (NAME##_node*)malloc(sizeof(NAME##_node));                          \
    if (!node) {                                                               \
        list->is_error = 1;                                                    \
        if (list->term_function) {                                             \
            list->term_function((TYPE*)x);                                     \
        }                                                                      \
        return NULL;                                                           \
    }                                                                          \
    node->data = *x;                                                           \
    node->next = NULL;                                                         \
    node->prev = list->tail;                                                   \
    list->tail->next = node;                                                   \
    list->tail = node;                                                         \
    list->count++;                                                             \
    return node;                                                               \
}                                                                              \
                                                                               \
NAME##_node* NAME##_add_v(NAME *list, TYPE x) {                                \
    return NAME##_add(list, &x);                                               \
}                                                                              \
                                                                               \
NAME##_node* NAME##_add_before(NAME *list, NAME##_node *target, const TYPE *x) {\
    NAME##_node *node;                                                         \
    if (!list || !target || !x) return NULL;                                   \
    if (target == list->head) {                                                \
        return NAME##_add_first(list, x);                                      \
    }                                                                          \
    node = (NAME##_node*)malloc(sizeof(NAME##_node));                          \
    if (!node) {                                                               \
        list->is_error = 1;                                                    \
        if (list->term_function) {                                             \
            list->term_function((TYPE*)x);                                     \
        }                                                                      \
        return NULL;                                                           \
    }                                                                          \
    node->data = *x;                                                           \
    node->prev = target->prev;                                                 \
    node->next = target;                                                       \
    if (target->prev) {                                                        \
        target->prev->next = node;                                             \
    }                                                                          \
    target->prev = node;                                                       \
    list->count++;                                                             \
    return node;                                                               \
}                                                                              \
                                                                               \
NAME##_node* NAME##_add_before_v(NAME *list, NAME##_node *target, TYPE x) {    \
    return NAME##_add_before(list, target, &x);                                \
}                                                                              \
                                                                               \
NAME##_node* NAME##_add_after(NAME *list, NAME##_node *target, const TYPE *x)  \
{                                                                              \
    NAME##_node *node;                                                         \
    if (!list || !target || !x) return NULL;                                   \
    if (target == list->tail) {                                                \
        return NAME##_add(list, x);                                            \
    }                                                                          \
    node = (NAME##_node*)malloc(sizeof(NAME##_node));                          \
    if (!node) {                                                               \
        list->is_error = 1;                                                    \
        if (list->term_function) {                                             \
            list->term_function((TYPE*)x);                                     \
        }                                                                      \
        return NULL;                                                           \
    }                                                                          \
    node->data = *x;                                                           \
    node->next = target->next;                                                 \
    node->prev = target;                                                       \
    if (target->next) {                                                        \
        target->next->prev = node;                                             \
    }                                                                          \
    target->next = node;                                                       \
    list->count++;                                                             \
    return node;                                                               \
}                                                                              \
                                                                               \
NAME##_node* NAME##_add_after_v(NAME *list, NAME##_node *target, TYPE x) {     \
    return NAME##_add_after(list, target, &x);                                 \
}                                                                              \
                                                                               \
int NAME##_remove(NAME *list, NAME##_node *node) {                             \
    if (!list || !node) return 0;                                              \
    if (node->prev) {                                                          \
        node->prev->next = node->next;                                         \
    } else {                                                                   \
        list->head = node->next;                                               \
    }                                                                          \
    if (node->next) {                                                          \
        node->next->prev = node->prev;                                         \
    } else {                                                                   \
        list->tail = node->prev;                                               \
    }                                                                          \
    if (list->term_function) {                                                 \
        list->term_function(&node->data);                                      \
    }                                                                          \
    free(node);                                                                \
    list->count--;                                                             \
    return 1;                                                                  \
} \
TYPE* NAME##_get_at(const NAME *list, size_t index) {                          \
    size_t i;                                                                  \
    NAME##_node *curr;                                                         \
    if (!list || index >= list->count) {                                       \
        return NULL;                                                           \
    }                                                                          \
    if (index < list->count / 2) {                                             \
        curr = list->head;                                                     \
        for (i = 0; i < index; i++) {                                          \
            curr = curr->next;                                                 \
        }                                                                      \
    } else {                                                                   \
        curr = list->tail;                                                     \
        for (i = list->count - 1; i > index; i--) {                            \
            curr = curr->prev;                                                 \
        }                                                                      \
    }                                                                          \
    return &curr->data;                                                        \
}

#define linked_list_hc(NAME, TYPE)                                             \
linked_list_h(NAME, TYPE)                                                      \
linked_list_c(NAME, TYPE)


/* =========================================================================
 * 2. POINTER LINKED LIST MACROS
 * ========================================================================= */

#define pointer_linked_list_h(NAME, TYPE)                                      \
typedef void (*NAME##_delete_f)(TYPE *elem);                                   \
                                                                               \
typedef struct NAME##_node {                                                   \
    TYPE *data;                                                                \
    struct NAME##_node *prev;                                                  \
    struct NAME##_node *next;                                                  \
} NAME##_node;                                                                 \
                                                                               \
typedef struct NAME {                                                          \
    NAME##_node *head;                                                         \
    NAME##_node *tail;                                                         \
    size_t count;                                                              \
    int is_error;                                                              \
    NAME##_delete_f delete_function;                                           \
} NAME;                                                                        \
                                                                               \
/* Creates a new pointer linked list. */                                       \
NAME* NAME##_new(NAME##_delete_f delete_function);                             \
                                                                               \
/* Frees all list nodes, managed items, and the list container itself. */      \
void NAME##_delete(NAME *list);                                                \
                                                                               \
/* Appends a pointer to the tail of the list. */                               \
NAME##_node* NAME##_add(NAME *list, TYPE *x);                                  \
                                                                               \
/* Prepends a pointer to the head of the list. */                              \
NAME##_node* NAME##_add_first(NAME *list, TYPE *x);                            \
                                                                               \
/* Inserts a pointer before a target node. */                                  \
NAME##_node* NAME##_add_before(NAME *list, NAME##_node *target, TYPE *x);      \
                                                                               \
/* Inserts a pointer after a target node. */                                   \
NAME##_node* NAME##_add_after(NAME *list, NAME##_node *target, TYPE *x);       \
                                                                               \
/* Removes a specific node from the list and frees its data pointer. */        \
int NAME##_remove(NAME *list, NAME##_node *node);\
/* Returns a pointer to the stored data pointer (TYPE**) at index. */          \
TYPE** NAME##_get_at(const NAME *list, size_t index);


#define pointer_linked_list_c(NAME, TYPE)                                      \
NAME* NAME##_new(NAME##_delete_f delete_function) {                            \
    NAME *list = (NAME*)malloc(sizeof(NAME));                                  \
    if (!list) return NULL;                                                    \
    list->head = NULL;                                                         \
    list->tail = NULL;                                                         \
    list->count = 0;                                                           \
    list->is_error = 0;                                                        \
    list->delete_function = delete_function;                                   \
    return list;                                                               \
}                                                                              \
                                                                               \
void NAME##_delete(NAME *list) {                                               \
    NAME##_node *curr;                                                         \
    NAME##_node *next_node;                                                    \
    if (!list) return;                                                         \
    curr = list->head;                                                         \
    while (curr) {                                                             \
        next_node = curr->next;                                                \
        if (list->delete_function && curr->data) {                             \
            list->delete_function(curr->data);                                 \
        }                                                                      \
        free(curr);                                                            \
        curr = next_node;                                                      \
    }                                                                          \
    free(list);                                                                \
}                                                                              \
                                                                               \
NAME##_node* NAME##_add_first(NAME *list, TYPE *x) {                           \
    NAME##_node *node;                                                         \
    if (!list) return NULL;                                                    \
    node = (NAME##_node*)malloc(sizeof(NAME##_node));                          \
    if (!node) {                                                               \
        list->is_error = 1;                                                    \
        if (list->delete_function && x) {                                      \
            list->delete_function(x);                                          \
        }                                                                      \
        return NULL;                                                           \
    }                                                                          \
    node->data = x;                                                            \
    node->prev = NULL;                                                         \
    node->next = list->head;                                                   \
    if (list->head) {                                                          \
        list->head->prev = node;                                               \
    } else {                                                                   \
        list->tail = node;                                                     \
    }                                                                          \
    list->head = node;                                                         \
    list->count++;                                                             \
    return node;                                                               \
}                                                                              \
                                                                               \
NAME##_node* NAME##_add(NAME *list, TYPE *x) {                                 \
    NAME##_node *node;                                                         \
    if (!list) return NULL;                                                    \
    if (!list->tail) {                                                         \
        return NAME##_add_first(list, x);                                      \
    }                                                                          \
    node = (NAME##_node*)malloc(sizeof(NAME##_node));                          \
    if (!node) {                                                               \
        list->is_error = 1;                                                    \
        if (list->delete_function && x) {                                      \
            list->delete_function(x);                                          \
        }                                                                      \
        return NULL;                                                           \
    }                                                                          \
    node->data = x;                                                            \
    node->next = NULL;                                                         \
    node->prev = list->tail;                                                   \
    list->tail->next = node;                                                   \
    list->tail = node;                                                         \
    list->count++;                                                             \
    return node;                                                               \
}                                                                              \
                                                                               \
NAME##_node* NAME##_add_before(NAME *list, NAME##_node *target, TYPE *x) {     \
    NAME##_node *node;                                                         \
    if (!list || !target) return NULL;                                         \
    if (target == list->head) {                                                \
        return NAME##_add_first(list, x);                                      \
    }                                                                          \
    node = (NAME##_node*)malloc(sizeof(NAME##_node));                          \
    if (!node) {                                                               \
        list->is_error = 1;                                                    \
        if (list->delete_function && x) {                                      \
            list->delete_function(x);                                          \
        }                                                                      \
        return NULL;                                                           \
    }                                                                          \
    node->data = x;                                                            \
    node->prev = target->prev;                                                 \
    node->next = target;                                                       \
    if (target->prev) {                                                        \
        target->prev->next = node;                                             \
    }                                                                          \
    target->prev = node;                                                       \
    list->count++;                                                             \
    return node;                                                               \
}                                                                              \
                                                                               \
NAME##_node* NAME##_add_after(NAME *list, NAME##_node *target, TYPE *x) {      \
    NAME##_node *node;                                                         \
    if (!list || !target) return NULL;                                         \
    if (target == list->tail) {                                                \
        return NAME##_add(list, x);                                            \
    }                                                                          \
    node = (NAME##_node*)malloc(sizeof(NAME##_node));                          \
    if (!node) {                                                               \
        list->is_error = 1;                                                    \
        if (list->delete_function && x) {                                      \
            list->delete_function(x);                                          \
        }                                                                      \
        return NULL;                                                           \
    }                                                                          \
    node->data = x;                                                            \
    node->next = target->next;                                                 \
    node->prev = target;                                                       \
    if (target->next) {                                                        \
        target->next->prev = node;                                             \
    }                                                                          \
    target->next = node;                                                       \
    list->count++;                                                             \
    return node;                                                               \
}                                                                              \
                                                                               \
int NAME##_remove(NAME *list, NAME##_node *node) {                             \
    if (!list || !node) return 0;                                              \
    if (node->prev) {                                                          \
        node->prev->next = node->next;                                         \
    } else {                                                                   \
        list->head = node->next;                                               \
    }                                                                          \
    if (node->next) {                                                          \
        node->next->prev = node->prev;                                         \
    } else {                                                                   \
        list->tail = node->prev;                                               \
    }                                                                          \
    if (list->delete_function && node->data) {                                 \
        list->delete_function(node->data);                                     \
    }                                                                          \
    free(node);                                                                \
    list->count--;                                                             \
    return 1;                                                                  \
} \
TYPE** NAME##_get_at(const NAME *list, size_t index) {                         \
    size_t i;                                                                  \
    NAME##_node *curr;                                                         \
    if (!list || index >= list->count) {                                       \
        return NULL;                                                           \
    }                                                                          \
    if (index < list->count / 2) {                                             \
        curr = list->head;                                                     \
        for (i = 0; i < index; i++) {                                          \
            curr = curr->next;                                                 \
        }                                                                      \
    } else {                                                                   \
        curr = list->tail;                                                     \
        for (i = list->count - 1; i > index; i--) {                            \
            curr = curr->prev;                                                 \
        }                                                                      \
    }                                                                          \
    return &curr->data;                                                        \
}


#define pointer_linked_list_hc(NAME, TYPE)                                     \
pointer_linked_list_h(NAME, TYPE)                                              \
pointer_linked_list_c(NAME, TYPE)

#endif /* LINKED_LIST_H */

// linked_list Design by Arshad Latti with help of Gemini and implemented by Gemini