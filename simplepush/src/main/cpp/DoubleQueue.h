//
// Created by loki on 2020/8/13.
//

#ifndef MULTIMEDIA_DOUBLEQUEUE_H
#define MULTIMEDIA_DOUBLEQUEUE_H

#include "../../../../../../../../Library/Android/sdk/ndk/21.3.6528147/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/cstdio"

extern int create_queue();

extern int destory_queue();

extern int queue_is_empty();

extern int queue_size();

extern void *queue_get(int index);

extern void *queue_get_first();

extern void *queue_get_last();

extern int queue_insert(int index, void *pval);

extern int queue_insert_first(void *pval);

extern int queue_append_last(void *pval);

extern int queue_delete(int index);

extern int queue_delete_first();

extern int queue_delete_last();

typedef struct queue_node {
    struct queue_node *prev;
    struct queue_node *next;
    void *p;
} node;

static node *phead = NULL;
static int count = 0;

#endif //MULTIMEDIA_DOUBLEQUEUE_H
