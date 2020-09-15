//
// Created by loki on 2020/8/25.
//
#include "cn_idu_ffmpeg_RSFFmpeg.h"
#include "common.h"

extern "C"
JNIEXPORT void JNICALL
Java_cn_idu_ffmpeg_RSFFmpeg_decodeMP42YUV(JNIEnv *env, jclass clazz, jstring path1, jstring path2) {

    const char *path_in = env->GetStringUTFChars(path1, 0);

    LOGD("path_in: %s", path_in);
    const char *path_out = env->GetStringUTFChars(path2, 0);
    LOGD("path_out: %s", path_out);
    int ret;

    AVFormatContext *avFormatContextP;
    int i, videoIndex;
    AVCodecContext *avCodecContextP;
    AVCodec *avCodecP;
    AVFrame *avFrameP, *avFrameYUVP;
    uint8_t *out_buffer;
    AVPacket *avPacketP;
    int y_len, got_picture;
    struct SwsContext *img_convert_ctx;
    FILE *fp_yuv;
    int frame_cnt;
    clock_t time_start, time_finish;
    double time_duration = 0.0;

    avFormatContextP = avformat_alloc_context();
    ret = avformat_open_input(&avFormatContextP, path_in, NULL, NULL);
    if (ret != 0) {
        CommonUtil::ff_Error(ret, "avformat_open_input failed!");
        goto end;
    }
    ret = avformat_find_stream_info(avFormatContextP, NULL);
    if (ret != 0) {
        CommonUtil::ff_Error(ret, "avformat_find_stream_info failed!");
        goto end;
    }

    videoIndex = -1;
    for (int i = 0; i < avFormatContextP->nb_streams; ++i) {
        if (avFormatContextP->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
            break;
        }
    }
    if (videoIndex == -1) {
        LOGD("Couldn't find a video Stream!");
        goto end;
    }

    avCodecContextP = avFormatContextP->streams[videoIndex]->codec;
    avCodecP = avcodec_find_decoder(avCodecContextP->codec_id);
    if (!avCodecP) {
        LOGD("Could't find Codec.");
        goto end;
    }
    ret = avcodec_open2(avCodecContextP, avCodecP, NULL);
    if (ret != 0) {
        CommonUtil::ff_Error(ret, "avcodec_open2 failed.");
        goto end;
    }
    avFrameP = av_frame_alloc();
    avFrameYUVP = av_frame_alloc();
    out_buffer = static_cast<uint8_t *>(av_malloc(
            av_image_get_buffer_size(AV_PIX_FMT_YUV420P, avCodecContextP->width,
                                     avCodecContextP->height, 1)));
    av_image_fill_arrays(avFrameYUVP->data, avFrameYUVP->linesize, out_buffer, AV_PIX_FMT_YUV420P,
                         avCodecContextP->width, avCodecContextP->height, 1);
    avPacketP = static_cast<AVPacket *>(av_malloc(sizeof(AVPacket)));

    img_convert_ctx = sws_getContext(avCodecContextP->width, avCodecContextP->height,
                                     avCodecContextP->pix_fmt, avCodecContextP->width,
                                     avCodecContextP->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL,
                                     NULL, NULL);
    LOGD("[Input ]:%s\n"
         "[Output ]:%s\n"
         "[Format ]:%s\n"
         "[Codec ]:%s\n"
         "[Resolution ]:%dx%d\n",
         path_in,
         path_out,
         avFormatContextP->iformat->name,
         avCodecContextP->codec->name,
         avCodecContextP->width, avCodecContextP->height);

    fp_yuv = fopen(path_out, "wb+");
    if (!fp_yuv) {
        LOGD("Cannot open output file!");
        goto end;
    }
    frame_cnt = 0;
    time_start = clock();

    while ((ret = av_read_frame(avFormatContextP, avPacketP)) >= 0) {

        if (avPacketP->stream_index == videoIndex) {
            ret = avcodec_decode_video2(avCodecContextP, avFrameP, &got_picture, avPacketP);
            if (ret < 0) {
                CommonUtil::ff_Error(ret, "avcodec_decode_video2 error!");
                goto end;
            }
            if (got_picture) {
                sws_scale(img_convert_ctx, avFrameP->data, avFrameP->linesize, 0,
                          avCodecContextP->height,
                          avFrameYUVP->data, avFrameYUVP->linesize
                );
                y_len = avCodecContextP->width * avCodecContextP->height;
                fwrite(avFrameYUVP->data[0], 1, y_len, fp_yuv);
                fwrite(avFrameYUVP->data[1], 1, y_len / 4, fp_yuv);
                fwrite(avFrameYUVP->data[2], 1, y_len / 4, fp_yuv);

                frame_cnt++;
                LOGD("av_read_frame ret: %d , count:%d ,frameType:%d", ret, frame_cnt,
                     avFrameP->pict_type);
            }
        }
        av_packet_unref(avPacketP);
    }
    time_finish = clock();
    time_duration = (double) (time_finish - time_start);
    LOGD("\n[Time   ]: %f", time_duration);

    end:
    sws_freeContext(img_convert_ctx);
    fclose(fp_yuv);
    av_frame_free(&avFrameP);
    av_frame_free(&avFrameYUVP);
    avcodec_close(avCodecContextP);
    avformat_close_input(&avFormatContextP);

    env->ReleaseStringUTFChars(path1, path_in);
    env->ReleaseStringUTFChars(path2, path_out);
    LOGD("func end!");
}