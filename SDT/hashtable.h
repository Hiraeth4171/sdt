#ifndef _SDT_HASHTABLE_LIB_H
#define _SDT_HASHTABLE_LIB_H
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "./linkedlist.h"


typedef struct {
    void* id;
    size_t (*hash_function)(void*, size_t);
    int (*item_cmpr)(void*, void*); // item, item_id
    void (*item_free)(void*);
    size_t length;
    size_t item_size;
    SDTLinkedListNode** items;
} SDTHashtable;

/*
 * TODO:
 *  - SDTStack
 *  - SDTBSTree
 *
 * */

static inline SDTHashtable* sdt_hashtable_init(size_t size, size_t item_size, size_t (*hash_function)(void*, size_t), int (*item_cmp)(void*, void*), void (*item_free)(void*)) {
    SDTHashtable* _hashtable = (SDTHashtable*)malloc(sizeof(SDTHashtable));
    _hashtable->items = (SDTLinkedListNode**) malloc(size * sizeof(SDTLinkedListNode*));
    for (size_t i = 0; i < size; ++i) _hashtable->items[i] = NULL;
    _hashtable->length =  size;
    _hashtable->item_size =  item_size;
    _hashtable->hash_function = hash_function;
    _hashtable->item_cmpr = item_cmp;
    _hashtable->item_free = item_free;
    return _hashtable;
}

static inline void sdt_hashtable_add(SDTHashtable* hashtable, void* item_id, void* item) {
    size_t i = hashtable->hash_function(item_id, hashtable->length);
    if (hashtable->items[i] != NULL) { // add a safegaurd
        SDTLinkedListNode* cur = hashtable->items[i];
        while (cur->next != NULL) cur = cur->next;
        cur->next = (SDTLinkedListNode*)malloc(sizeof(SDTLinkedListNode));
        *(cur->next) = (SDTLinkedListNode){.value = (void*)item, .next = NULL};
        return;
    }
    hashtable->items[i] = (SDTLinkedListNode*)malloc(sizeof(SDTLinkedListNode));
    *hashtable->items[i] = (SDTLinkedListNode){.value = (void*)item, .next = NULL};
}

static inline void sdt_hashtable_remove(SDTHashtable* hashtable, void* item_id) {
    size_t i = hashtable->hash_function(item_id, hashtable->length);
    if (hashtable->items[i] != NULL) { // add a safegaurd
        SDTLinkedListNode* cur = hashtable->items[i], *prev = cur;
        while (cur != NULL) {
            if(cur->value == NULL) return; // this doesn't safegaurd much here.
            if (hashtable->item_cmpr(cur->value, item_id) == 0) {
                hashtable->item_free(cur->value);
                if(cur->next != NULL) prev->next = cur->next;
                if (prev == cur) hashtable->items[i] = NULL;
                free(cur);
                break;
            }
            prev = cur;
            cur = cur->next;
        }
    }
}

static inline void* sdt_hashtable_get(SDTHashtable* hashtable, void* item_id) {
    if (hashtable == NULL) return NULL;
    size_t i = hashtable->hash_function(item_id, hashtable->length);
    if (hashtable->items[i] != NULL) { // add a safegaurd
        SDTLinkedListNode* cur = hashtable->items[i];
        while (cur != NULL) {
            if(cur->value == NULL) return NULL; // this doesn't safegaurd much here.
            if (hashtable->item_cmpr(cur->value, item_id) == 0) return cur->value;
            cur = cur->next;
        }
    }
    return NULL;
}

static inline void sdt_hashtable_free(SDTHashtable** hashtable, void (*free_callback)(void*)) {
    if (hashtable == NULL || *hashtable == NULL) return;
    if ((*hashtable)->items == NULL) goto free_ptr;
    for (size_t i = 0; i < (*hashtable)->length; ++i) {
        if ((*hashtable)->items[i] == NULL) continue;
        SDTLinkedListNode* cur = (*hashtable)->items[i];
        while (cur != NULL) {
            free_callback(cur->value);
            SDTLinkedListNode* tmp = cur;
            cur = cur->next;
            free(tmp);
        }
    }
    free((*hashtable)->items);
free_ptr:
    free(*hashtable);
}

static inline void sdt_hashtable_print(SDTHashtable* hashtable, void (*print_callback)(void*)) {
    for (size_t i = 0; i < hashtable->length; ++i) {
        SDTLinkedListNode* cur = hashtable->items[i];
#if defined(_WIN32) || defined(_WIN64)
        printf("%llu : ", i);
#elif defined(__linux__)
        printf("%lu : ", i);
#endif
        while (cur != NULL && cur->value != NULL) {
            print_callback(cur->value);
            printf(" -> ");
            cur = cur->next;
        }
        printf("END\n");
    }
}

#endif
