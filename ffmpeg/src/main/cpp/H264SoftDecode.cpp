//
// Created by loki on 2020/9/2.
//

#include "H264SoftDecode.h"

H264SoftDecode::H264SoftDecode(AVStream *pStream) {
    avStream = pStream;

//    AVCodecParserContext *parser;
//    parser = av_parser_init(avCodec->id);

    //获取视频解码器
    avCodec = avcodec_find_decoder(avStream->codecpar->codec_id);
    if (!avCodec) {
        LOGE("avcodec_find_decoder failed!");
        return;
    }
    //获得解码器上下文
    avCodecContext = avcodec_alloc_context3(avCodec);
    if (!avCodecContext) {
        LOGE("avcodec_alloc_context3 failed!");
        return;
    }
    code = avcodec_parameters_to_context(avCodecContext, avStream->codecpar);
    if (code < 0) {
        CommonUtil::ff_Error(code, "avcodec_parameters_to_context failed!");
        return;
    }
    avCodecContext->pkt_timebase = pStream->time_base;
    avCodecContext->codec_id = avCodec->id;

    //TODO fast?
    if (1)
        avCodecContext->flags2 |= AV_CODEC_FLAG2_FAST;

    code = avcodec_open2(avCodecContext, avCodec, 0);
    if (code != 0) {
        CommonUtil::ff_Error(code, "avcodec_open2 failed!");
        return;
    }

    AVRational frame_rate = avStream->avg_frame_rate;
    double fps = av_q2d(frame_rate);
    LOGD("H264SoftDecode fps: %f", fps);
    code = 0;
}

H264SoftDecode::~H264SoftDecode() {
    avcodec_free_context(&avCodecContext);
}

void H264SoftDecode::decode(AVFrame *pFrame, AVPacket *packet, _JNIEnv *pEnv, _jobject *pJobject) {
    if (y_len == 0) {
        y_len = avCodecContext->width * avCodecContext->height;
        yuv = pEnv->NewByteArray(y_len * 3 / 2);
    }
    clock_t time_start = clock();
    int ret = avcodec_send_packet(avCodecContext, packet);
    LOGD("avcodec_send_packet ret: %d", ret);
    av_packet_unref(packet);
    int count = 0;
    while (ret >= 0) {
        ret = avcodec_receive_frame(avCodecContext, pFrame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
//            CommonUtil::ff_Error(ret, "avcodec_receive_frame failed");
        } else {
            count++;
//            LOGD("decode frame done: %d", count);
            LOGD("decode end: width:%d, height:%d, fmt:%d", pFrame->width,
                 pFrame->height, pFrame->format);

            LOGD("decode end: pts:%lld", pFrame->pts);

            pEnv->SetByteArrayRegion(yuv, 0, y_len,
                                     reinterpret_cast<const jbyte *>(pFrame->data[0]));
            pEnv->SetByteArrayRegion(yuv, y_len, y_len / 4,
                                     reinterpret_cast<const jbyte *>(pFrame->data[1]));
            pEnv->SetByteArrayRegion(yuv, y_len + y_len / 4, y_len / 4,
                                     reinterpret_cast<const jbyte *>(pFrame->data[2]));
            jclass clazz = pEnv->GetObjectClass(pJobject);
            jmethodID callback = pEnv->GetMethodID(clazz, "onInfoCallback",
                                                   "([BIII)V");
            pEnv->CallVoidMethod(pJobject, callback, yuv, pFrame->width, pFrame->height,
                                 pFrame->format);
            pEnv->DeleteLocalRef(clazz);
        }
    }
    clock_t time_finish = clock();
    LOGD("avcodec_receive_frame: %d, [Time   ]: %f", count,
         (float) (time_finish - time_start) * 1000.0 / CLOCKS_PER_SEC);
}


