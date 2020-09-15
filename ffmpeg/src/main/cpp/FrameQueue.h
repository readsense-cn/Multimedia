//
// Created by loki on 2020/9/9.
//

#ifndef MULTIMEDIA_FRAMEQUEUE_H
#define MULTIMEDIA_FRAMEQUEUE_H


#include "include/libavutil/frame.h"

typedef struct Frame {
    AVFrame *frame;
    double pts;
    double duration;
} Frame;

class FrameQueue {

public:
    FrameQueue(int max_size);

    ~FrameQueue();

    int size;
    int max_size;


};


#endif //MULTIMEDIA_FRAMEQUEUE_H
