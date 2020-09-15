//
// Created by loki on 2020/8/6.
//

#ifndef IFFMPEG_RSFFMPEG_H
#define IFFMPEG_RSFFMPEG_H

extern "C" {
#include "cn_idu_ffmpeg_RSFFmpeg.h"
#include "common.h"

AVStream *videoStream = NULL;
AVFrame *outputYUVFrame = NULL;
AVCodec *avCodec = NULL;
AVCodecContext *videoCodecContext = NULL;
int64_t beginTime;
AVPacket videoPacket = {0};
static AVFormatContext *pFormatCtx;

}


#endif //IFFMPEG_RSFFMPEG_H
