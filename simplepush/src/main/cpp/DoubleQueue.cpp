//
// Created by loki on 2020/8/13.
//

#include "DoubleQueue.h"
#include <malloc.h>


static node *create_node(void *pval) {
    node *pnode = NULL;
    pnode = static_cast<node *>(malloc(sizeof(node)));
    if (pnode) {
        pnode->prev = pnode->next = pnode;
        pnode->p = pval;
    }
    return pnode;
}

int create_queue() {
    phead = create_node(NULL);
    if (!phead)return -1;
    count = 0;
    return 0;
}

int queue_is_empty() {
    return count == 0;
}

int queue_size() {
    return count;
}

static node *get_node(int index) {
    if (index < 0 || index >= count)return NULL;
    node *pnode = phead->next;
    for (int i = 0; i < index; ++i) {
        pnode = pnode->next;
    }
    return pnode;
}

void *queue_get(int index) {
    node *pIndex = get_node(index);
    if (pIndex)return pIndex->p;
    return NULL;
}

void *queue_get_first() {
    return queue_get(0);
}

void *queue_get_last() {
    return queue_get(count - 1);
}

int queue_insert(int index, void *pval) {
    if (index == 0)return queue_insert_first(pval);
    node *pIndex = get_node(index);
    if (!pIndex)return -1;
    node *pnode = create_node(pval);
    if (!pnode)return -1;
    pnode->prev = pIndex->prev;
    pnode->next = pIndex;
    pnode->prev->next = pnode;
    pnode->prev = pnode;
    count++;
    return 0;
}

int queue_insert_first(void *pval) {
    node *pnode = create_node(pval);
    if (!pnode)return -1;
    pnode->prev = phead;
    pnode->next = phead->next;
    phead->next->prev = pnode;
    phead->next = pnode;
    return 0;
}

int queue_append_last(void *pval) {
    node *pnode = create_node(pval);
    if (!pnode)return -1;

    pnode->prev = phead->prev;
    pnode->next = phead;
    phead->prev->next = pnode;
    phead->prev = pnode;
    count++;
    return 0;
}

int queue_delete(int index) {
    node *pnode = get_node(index);
    if (!pnode)return -1;

    pnode->prev->next = pnode->next;
    pnode->prev = pnode->next->prev;
    free(pnode);
    count--;
    return 0;
}

int queue_delete_first() {
    return queue_delete(0);
}

int queue_delete_last() {
    return queue_delete(count - 1);
}

int destory_queue() {
    if (!phead)return -1;
    node *pnode = phead->next;
    node *ptemp = NULL;

    while (pnode != phead) {
        ptemp = pnode;
        pnode = pnode->next;
        free(ptemp);
    }
    free(phead);
    phead = NULL;
    count = 0;
    return 0;
}