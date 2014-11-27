/* Copyright 2013 Bliksem Labs.
 * See the LICENSE file at the top-level directory of this distribution and at
 * https://github.com/bliksemlabs/rrrr/
 */

/* list.h */

#include "list.h"

#include <stdint.h>
#include <stdlib.h>

void LinkedList_init (LinkedList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

LinkedList *LinkedList_new () {
    LinkedList *list = malloc (sizeof(LinkedList));
    LinkedList_init (list);
    return list;
}

void LinkedList_destroy (LinkedList **list) {
    free (*list);
    *list = NULL;
}

/* Add to head */
void LinkedList_push (LinkedList *list, void *payload) {
    ListNode *node = malloc (sizeof(ListNode));
    node->payload = payload;
    node->next = list->head;
    list->head = node;
    if (list->tail == NULL) list->tail = node;
    list->size += 1;
}

/* Add to tail */
void LinkedList_enqueue (LinkedList *list, void *payload) {
    ListNode *node = malloc (sizeof(ListNode));
    node->payload = payload;
    node->next = NULL;
    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    list->size += 1;
}

/* Remove from head of list */
void *LinkedList_pop (LinkedList *list) {
    if (list->head != NULL) {
        void *payload = list->head->payload;
        ListNode *old_head = list->head;
        list->head = list->head->next;
        free (old_head);
        list->size -= 1;
        return payload;
    }

    return NULL;
}

