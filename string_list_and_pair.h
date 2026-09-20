/**
 * @file string_list_and_pair.h
 * @brief Single-header string list and key-value pair containers using generic linked list macros.
 */

#ifndef STRING_LIST_AND_PAIR_H
#define STRING_LIST_AND_PAIR_H

#include <stdlib.h>
#include "linked_list.h"

/* =========================================================================
 * DATA STRUCTURE DEFINITIONS
 * ========================================================================= */

/**
 * @brief Represents a single key-value configuration entry.
 */
typedef struct string_pair_entry {
    char *key;   /**< Dynamically allocated key string */
    char *value; /**< Dynamically allocated value string */
} string_pair_item_t;

/* =========================================================================
 * HEADER DECLARATIONS
 * ========================================================================= */

/**
 * @brief Instantiates the type definitions and function declarations for string_list.
 * 
 * Provides:
 * - string_list_node (Node structure containing char *data and next pointer)
 * - string_list (List container structure containing head, tail, and free callback)
 * - string_list_new(free_fn) -> Creates a new string_list instance
 * - string_list_delete(list) -> Destroys and frees all nodes and data in the list
 * - string_list_add(list, data) -> Appends a heap-allocated string pointer to the list
 * - string_list_remove(list, node) -> Removes a specific node from the list
 */
pointer_linked_list_h(string_list, char)

/**
 * @brief Instantiates the type definitions and function declarations for string_pair.
 * 
 * Provides:
 * - string_pair_node (Node structure containing string_pair_item_t *data and next pointer)
 * - string_pair (List container structure containing head, tail, and free callback)
 * - string_pair_new(free_fn) -> Creates a new string_pair instance
 * - string_pair_delete(list) -> Destroys and frees all nodes and entries in the list
 * - string_pair_add(list, data) -> Appends a string_pair_item_t pointer to the list
 * - string_pair_remove(list, node) -> Removes a specific node from the list
 */
pointer_linked_list_h(string_pair, string_pair_item_t)

/* =========================================================================
 * FUNCTION DECLARATIONS (CLEANUP CALLBACKS)
 * ========================================================================= */

/**
 * @brief Callback function to free a heap-allocated string item.
 * 
 * Passed to string_list_new() to handle automatic memory deallocation of strings.
 * 
 * @param str Pointer to the heap-allocated string to free.
 */
void sta_string_item_free(char *str);

/**
 * @brief Callback function to free a string_pair_item_t entry and its inner keys/values.
 * 
 * Passed to string_pair_new() to handle automatic recursive dynamic memory 
 * cleanup of key-value pair structures.
 * 
 * @param item Pointer to the string_pair_item_t structure to free.
 */
void sta_string_pair_item_free(string_pair_item_t *item);

/* =========================================================================
 * IMPLEMENTATIONS
 * ========================================================================= */

#ifdef STRING_LIST_AND_PAIR_IMPLEMENTATION

/* Instantiates the definition implementations for string_list operations */
pointer_linked_list_c(string_list, char)

/* Instantiates the definition implementations for string_pair operations */
pointer_linked_list_c(string_pair, string_pair_item_t)

void sta_string_item_free(char *str) {
    if (str != NULL) {
        free(str);
    }
}

void sta_string_pair_item_free(string_pair_item_t *item) {
    if (item != NULL) {
        if (item->key != NULL) {
            free(item->key);
        }
        if (item->value != NULL) {
            free(item->value);
        }
        free(item);
    }
}

#endif /* STRING_LIST_AND_PAIR_IMPLEMENTATION */

#endif /* STRING_LIST_AND_PAIR_H */

// A Project Design by Arshad Latti with help of Gemini and implemented by Gemini