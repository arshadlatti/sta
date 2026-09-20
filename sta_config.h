/**
 * @file sta_config.h
 * @brief Configuration file loader and saver using string_list and string_pair containers.
 */

#ifndef STA_CONFIG_H
#define STA_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "string_list_and_pair.h"

/* =========================================================================
 * FUNCTION DECLARATIONS
 * ========================================================================= */

/**
 * @brief Loads a line-separated text file into a string list.
 * 
 * @param file File path to open and read.
 * @return string_list* Pointer to dynamic linked list, or NULL on failure.
 */
string_list *config_list_load(const char *file);

/**
 * @brief Saves a string list into a line-separated text file.
 * 
 * @param file Target file path.
 * @param list Pointer to the string list.
 * @return int 0 on success, -1 on I/O error or invalid parameters.
 */
int config_list_save(const char *file, const string_list *list);

/**
 * @brief Loads key-value configuration entries from a file into a pair list.
 * 
 * @param file File path to open and read.
 * @return string_pair* Pointer to dynamic linked list, or NULL on failure.
 */
string_pair *config_pair_load(const char *file);

/**
 * @brief Saves a key-value pair list to a file.
 * 
 * @param file Target file path.
 * @param pair Pointer to the pair list.
 * @return int 0 on success, -1 on I/O error or invalid parameters.
 */
int config_pair_save(const char *file, const string_pair *pair);

/* =========================================================================
 * FUNCTION IMPLEMENTATIONS
 * ========================================================================= */

#ifdef STA_CONFIG_IMPLEMENTATION

/* Helper to strip leading and trailing whitespace in-place */
static char *sta_config_trim(char *str) {
    char *end;
    if (str == NULL) {
        return NULL;
    }
    while (*str != '\0' && isspace((unsigned char)*str)) {
        str++;
    }
    if (*str == '\0') {
        return str;
    }
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
    return str;
}

/* C89 safe duplicate string helper */
static char *sta_config_strdup(const char *src) {
    size_t len;
    char *dest;
    if (src == NULL) {
        return NULL;
    }
    len = strlen(src) + 1;
    dest = (char *)malloc(len);
    if (dest == NULL) {
        return NULL; /* Memory allocation failure check */
    }
    memcpy(dest, src, len);
    return dest;
}

string_list *config_list_load(const char *file) {
    FILE *fp;
    char line[512];
    char *trimmed;
    char *item_copy;
    string_list *list;

    if (file == NULL) {
        return NULL;
    }

    fp = fopen(file, "r");
    if (fp == NULL) {
        return NULL; /* File open error check */
    }

    list = string_list_new(sta_string_item_free);
    if (list == NULL) {
        fclose(fp);
        return NULL; /* Allocation check */
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        /* Truncation error check */
        if (strchr(line, '\n') == NULL && !feof(fp)) {
            string_list_delete(list);
            fclose(fp);
            return NULL;
        }

        trimmed = sta_config_trim(line);

        /* Skip empty lines and comment lines (# or ;) */
        if (*trimmed == '\0' || *trimmed == '#' || *trimmed == ';') {
            continue;
        }

        item_copy = sta_config_strdup(trimmed);
        if (item_copy == NULL) {
            string_list_delete(list);
            fclose(fp);
            return NULL;
        }

        if (string_list_add(list, item_copy) == NULL) {
            /* Cleanup handled automatically by string_list_add on error */
            string_list_delete(list);
            fclose(fp);
            return NULL;
        }
    }

    fclose(fp);
    return list;
}

int config_list_save(const char *file, const string_list *list) {
    FILE *fp;
    string_list_node *curr;

    if (file == NULL || list == NULL) {
        return -1;
    }

    fp = fopen(file, "w");
    if (fp == NULL) {
        return -1; /* File open check */
    }

    for (curr = list->head; curr != NULL; curr = curr->next) {
        if (curr->data != NULL) {
            if (fprintf(fp, "%s\n", curr->data) < 0) {
                fclose(fp);
                return -1; /* Write check */
            }
        }
    }

    fclose(fp);
    return 0;
}

string_pair *config_pair_load(const char *file) {
    FILE *fp;
    char line[512];
    char *trimmed;
    char *equal_sign;
    char *key;
    char *value;
    string_pair *pair_list;
    string_pair_item_t *item;

    if (file == NULL) {
        return NULL;
    }

    fp = fopen(file, "r");
    if (fp == NULL) {
        return NULL;
    }

    pair_list = string_pair_new(sta_string_pair_item_free);
    if (pair_list == NULL) {
        fclose(fp);
        return NULL;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        /* Truncation error check */
        if (strchr(line, '\n') == NULL && !feof(fp)) {
            string_pair_delete(pair_list);
            fclose(fp);
            return NULL;
        }

        trimmed = sta_config_trim(line);

        /* Skip empty and comment lines */
        if (*trimmed == '\0' || *trimmed == '#' || *trimmed == ';') {
            continue;
        }

        equal_sign = strchr(trimmed, '=');
        if (equal_sign == NULL) {
            /* Malformed line check */
            string_pair_delete(pair_list);
            fclose(fp);
            return NULL;
        }

        *equal_sign = '\0';
        key = sta_config_trim(trimmed);
        value = sta_config_trim(equal_sign + 1);

        if (strlen(key) == 0) {
            string_pair_delete(pair_list);
            fclose(fp);
            return NULL;
        }

        item = (string_pair_item_t *)malloc(sizeof(string_pair_item_t));
        if (item == NULL) {
            string_pair_delete(pair_list);
            fclose(fp);
            return NULL;
        }

        item->key = sta_config_strdup(key);
        if (item->key == NULL) {
            free(item);
            string_pair_delete(pair_list);
            fclose(fp);
            return NULL;
        }

        item->value = sta_config_strdup(value);
        if (item->value == NULL) {
            free(item->key);
            free(item);
            string_pair_delete(pair_list);
            fclose(fp);
            return NULL;
        }

        if (string_pair_add(pair_list, item) == NULL) {
            string_pair_delete(pair_list);
            fclose(fp);
            return NULL;
        }
    }

    fclose(fp);
    return pair_list;
}

int config_pair_save(const char *file, const string_pair *pair) {
    FILE *fp;
    string_pair_node *curr;

    if (file == NULL || pair == NULL) {
        return -1;
    }

    fp = fopen(file, "w");
    if (fp == NULL) {
        return -1;
    }

    for (curr = pair->head; curr != NULL; curr = curr->next) {
        if (curr->data != NULL && curr->data->key != NULL && curr->data->value != NULL) {
            if (fprintf(fp, "%s = %s\n", curr->data->key, curr->data->value) < 0) {
                fclose(fp);
                return -1;
            }
        }
    }

    fclose(fp);
    return 0;
}

#endif /* STA_CONFIG_IMPLEMENTATION */

#endif /* STA_CONFIG_H */

// A Project Design by Arshad Latti with help of Gemini and implemented by Gemini