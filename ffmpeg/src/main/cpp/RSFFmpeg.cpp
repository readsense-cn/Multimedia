//
// Created by loki on 2020/8/6.
//

#include <cstdio>

#include "RSFFmpeg.h"


struct URLProtocol;

extern "C"
JNIEXPORT void JNICALL
Java_cn_idu_ffmpeg_RSFFmpeg_prepareFFMpeg(JNIEnv *env, jclass clazz) {

    avcodec_register_all();
    av_register_all();
    avformat_network_init();
    avfilter_register_all();

    char info[40000] = {0};
    struct URLProtocol *pub = NULL;
    struct URLProtocol **p_temp = &pub;
    avio_enum_protocols(reinterpret_cast<void **>(p_temp), 0);

    while ((*p_temp) != NULL) {
        sprintf(info, "%s[protocols In ][%10s]\n", info, avio_enum_protocols(
                reinterpret_cast<void **>(p_temp), 0));
    }

    pub = NULL;
    avio_enum_protocols((void **) p_temp, 1);
    while ((*p_temp) != NULL) {
        sprintf(info, "%s[protocols Out][%10s]\n", info, avio_enum_protocols((void **) p_temp, 1));
    }

    AVInputFormat *if_temp = av_iformat_next(NULL);
    AVOutputFormat *of_temp = av_oformat_next(NULL);
    //Input
    while (if_temp != NULL) {
        sprintf(info, "%s[AvFormat In ][%10s]\n", info, if_temp->name);
        if_temp = if_temp->next;
    }
    //Output
    while (of_temp != NULL) {
        sprintf(info, "%s[AvFormat Out][%10s]\n", info, of_temp->name);
        of_temp = of_temp->next;
    }


    AVCodec *c_temp = av_codec_next(NULL);

    while (c_temp != NULL) {
        if (c_temp->decode != NULL) {
            sprintf(info, "%s[AVCodec Dec]", info);
        } else {
            sprintf(info, "%s[AVCodec Enc]", info);
        }
        switch (c_temp->type) {
            case AVMEDIA_TYPE_VIDEO:
                sprintf(info, "%s[Video]", info);
                break;
            case AVMEDIA_TYPE_AUDIO:
                sprintf(info, "%s[Audio]", info);
                break;
            default:
                sprintf(info, "%s[Other]", info);
                break;
        }
        sprintf(info, "%s[%10s]\n", info, c_temp->name);
        c_temp = c_temp->next;
    }


    AVFilter *f_temp = (AVFilter *) avfilter_next(NULL);
    while (f_temp != NULL) {
        sprintf(info, "%s[AVFilter %10s]\n", info, f_temp->name);
        f_temp = f_temp->next;
    }

    LOGD("%s", info);
//    LOGD("%s", avcodec_configuration());
}

extern "C"
JNIEXPORT jlong JNICALL
Java_cn_idu_ffmpeg_RSFFmpeg_initRtmpParams(JNIEnv *env, jclass clazz, jstring url, jint width,
                                           jint height) {

    const char *outUrl = env->GetStringUTFChars(url, 0);
    char output_str[500] = {0};
    strcpy(output_str, outUrl);
    env->ReleaseStringUTFChars(url, outUrl);
    LOGD("URL: %s", output_str);
    int ret;


    avCodec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!avCodec) {
        LOGE("avcodec not found!");
        return -1;
    }
    LOGD("init: end avcodec_find_encoder");
    videoCodecContext = avcodec_alloc_context3(avCodec);
    if (!videoCodecContext) {
        LOGE("avcodec alloc context failed!");
        return -1;
    }
    LOGD("init: end avcodec_alloc_context3");


    videoCodecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER; //全局参数
    videoCodecContext->codec_id = avCodec->id;
    videoCodecContext->bit_rate = 100 * 1024 * 8;//压缩后每秒视频的bit位大小 50kB
    videoCodecContext->width = width;
    videoCodecContext->height = height;
    videoCodecContext->framerate = {25, 1};
    videoCodecContext->gop_size = 50;
    videoCodecContext->max_b_frames = 0;
    videoCodecContext->qmin = 10;
    videoCodecContext->qmax = 50;
    videoCodecContext->time_base = {1, 1000000};//AUDIO VIDEO 两边时间基数要相同
    videoCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    videoCodecContext->level = 41;
//    videoCodecContext->me_method = ME_HEX;
    videoCodecContext->refs = 1;
    videoCodecContext->chromaoffset = 2;
/**
     *选项: ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow and placebo.
     * 固定码率150K,
     * 设置slow时：编码速度：245 fps 4ms
     * medium 时：编码速度：335 fps 2ms,
     * veryslow 时：编码速度：140 fps 7ms
     */

    AVDictionary *opts = NULL;
    av_dict_set(&opts, "preset", "ultrafast", 0);//编码器的速度会影响推流音视频同步,所以这里需要设置下
//    av_dict_set(&opts, "tune", "zerolatency", 0);//如果开0延迟可能会影响视频质量
    av_dict_set(&opts, "profile", "baseline", 0);//I/P帧

    outputYUVFrame = av_frame_alloc();
    outputYUVFrame->format = AV_PIX_FMT_YUV420P;
    outputYUVFrame->width = width;
    outputYUVFrame->height = height;
    ret = av_frame_get_buffer(outputYUVFrame, 32);
    if (ret != 0) {
        CommonUtil::ff_Error(ret, "av_frame_get_buffer error!");
        return -1;
    }
    ret = avcodec_open2(videoCodecContext, avCodec, &opts);
    //avcodec open failed! info: Invalid argument
    if (ret != 0) {
        CommonUtil::ff_Error(ret, "avcodec_open2 error!");
        return -1;
    }
    beginTime = av_gettime();
    LOGD("init: end avcodec_open2");


    ret = avformat_alloc_output_context2(&pFormatCtx, NULL, "flv", output_str); //RTMP
    if (ret < 0) {
        return CommonUtil::ff_Error(ret, "avformat_alloc_output_context2");
    }

    av_dump_format(pFormatCtx, 0, output_str, 1);

    videoStream = avformat_new_stream(pFormatCtx, avCodec);
    if (!videoStream) {
        LOGE("avformat_new_stream failed!");
        return -1;
    }
    ret = avcodec_parameters_from_context(videoStream->codecpar, videoCodecContext);
    if (ret < 0) {
        return CommonUtil::ff_Error(ret, "avcodec_parameters_from_context failed!");
    }
    videoStream->codecpar->codec_tag = 0;
    if (videoCodecContext->codec_type != AVMEDIA_TYPE_VIDEO) {
        LOGE("Add video stream failed!");
        return -1;
    }
    LOGD("Add video stream success!");

    ret = avio_open(&pFormatCtx->pb, output_str, AVIO_FLAG_WRITE);
    if (ret < 0) {
        return CommonUtil::ff_Error(ret, "Could not open output URL");
    }
    LOGD("init: end avio_open");


    //写文件头（Write file header）
    ret = avformat_write_header(pFormatCtx, NULL);
    if (ret < 0) {
        return CommonUtil::ff_Error(ret, "avformat_write_header failed!");
    }
    LOGD("init: end avformat_write_header");

    LOGD("initVideo end");
    free(output_str);

    return 0;
}


extern "C"
JNIEXPORT void JNICALL
Java_cn_idu_ffmpeg_RSFFmpeg_sendVideoFrame(JNIEnv *env, jclass clazz, jbyteArray data, jint y_len) {
    jbyte *i420buffer = env->GetByteArrayElements(data, NULL);
//    uint8_t *videoDstData = (uint8_t *) malloc(y_len * 3 / 2);
//    memcpy(videoDstData, i420buffer, y_len * 3 / 2);

    /**
        * 方式一：
        * I420手动填充AVFrame,需要注意ySize =width*height;
        */
    memcpy(outputYUVFrame->data[0], i420buffer, y_len);//Y
    memcpy(outputYUVFrame->data[1], i420buffer + y_len, y_len / 4);//U
    memcpy(outputYUVFrame->data[2], i420buffer + (y_len * 5 / 4), y_len / 4);//V

    outputYUVFrame->linesize[0] = videoCodecContext->width;
    outputYUVFrame->linesize[1] = videoCodecContext->width / 2;
    outputYUVFrame->linesize[2] = videoCodecContext->width / 2;
    env->ReleaseByteArrayElements(data, i420buffer, 0);
    LOGD("send: end copy");
    /**
       * 方式二:  deprecated
       */

//    avpicture_fill((AVPicture *) outputYUVFrame, videoDstData, AV_PIX_FMT_YUV420P,
//                   videoCodecContext->width,
//                   videoCodecContext->height);

    /**
     * 方式三:
     */
//    av_image_fill_arrays(outputYUVFrame->data,
//                         outputYUVFrame->linesize, videoDstData,
//                         AV_PIX_FMT_YUV420P, videoCodecContext->width,
//                         videoCodecContext->height, 1);

    outputYUVFrame->pts = av_gettime() - beginTime;
    int ret = 0;
    ret = avcodec_send_frame(videoCodecContext, outputYUVFrame);
    if (ret != 0) {
        CommonUtil::ff_Error(ret, "avcodec_send_frame failed!");
        return;
    }
    LOGD("send: end avcodec_send_frame");
    av_packet_unref(&videoPacket);

    ret = avcodec_receive_packet(videoCodecContext, &videoPacket);
    if (ret != 0) {
        CommonUtil::ff_Error(ret, "avcodec_receive_packet failed!");
        return;
    }


    //编码完成
    LOGD("编码完成 %d", videoPacket.buf->size);


    AVRational stime;
    AVRational dtime;

    videoPacket.stream_index = videoStream->index;

    stime = videoCodecContext->time_base;
    dtime = videoStream->time_base;

    videoPacket.pts = av_rescale_q(videoPacket.pts, stime, dtime);
    videoPacket.dts = av_rescale_q(videoPacket.dts, stime, dtime);
    videoPacket.duration = av_rescale_q(videoPacket.duration, stime, dtime);

    ret = av_interleaved_write_frame(pFormatCtx, &videoPacket);
    if (ret == 0) {
        LOGD("上传服务器完成");
    } else {
        CommonUtil::ff_Error(ret, "av_interleaved_write_frame failed!");
    }

}

extern "C"
JNIEXPORT void JNICALL
Java_cn_idu_ffmpeg_RSFFmpeg_releasePush(JNIEnv *env, jclass clazz) {

}