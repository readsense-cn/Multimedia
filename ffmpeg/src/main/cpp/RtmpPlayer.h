//
// Created by loki on 2020/8/26.
//

#ifndef MULTIMEDIA_RTMPPLAYER_H
#define MULTIMEDIA_RTMPPLAYER_H

#include <jni.h>
#include "common.h"

//'1': Use H.264 Bitstream Filter
#define USE_H264BSF 0

class RtmpPlayer {

private:
    AVFormatContext *ic = NULL;
    char in_path[100];
    int ret, video_index = -1, frame_index = 0;
    int video_width, video_height;
    AVDictionary *format_opts, *codec_opts;

public:
    RtmpPlayer();

    ~RtmpPlayer();

    int init(const char *string, int i, int i1);

    int play(_JNIEnv *pEnv, _jobject *pJobject);

};


#endif //MULTIMEDIA_RTMPPLAYER_H
