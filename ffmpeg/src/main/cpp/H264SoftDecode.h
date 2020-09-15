//
// Created by loki on 2020/9/2.
//

#ifndef MULTIMEDIA_H264SOFTDECODE_H
#define MULTIMEDIA_H264SOFTDECODE_H

#include "common.h"

class H264SoftDecode {
public:
    H264SoftDecode(AVStream *pStream);

    ~H264SoftDecode();

    AVStream *avStream;
    AVCodecContext *avCodecContext;
    AVCodec *avCodec;
    int code = -1;

    jbyteArray yuv;
    int y_len = 0;

    void decode(AVFrame *pFrame, AVPacket *packet, _JNIEnv *pEnv, _jobject *pJobject);
};


#endif //MULTIMEDIA_H264SOFTDECODE_H
