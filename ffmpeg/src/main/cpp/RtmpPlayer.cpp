//
// Created by loki on 2020/8/26.
//

#include "RtmpPlayer.h"
#include "H264SoftDecode.h"

int RtmpPlayer::init(const char *string, int width, int height) {

//    if (av_stristart(string, "rtmp", NULL)) {
    strcpy(in_path, string);
    video_width = width;
    video_height = height;
    return 0;
//    }
//    return -1;
}

RtmpPlayer::RtmpPlayer() {

}

int RtmpPlayer::play(_JNIEnv *pEnv, _jobject *pJobject) {
    LOGD("[in path]: %s, [width]: %d, [height]: %d", in_path, video_width, video_height);
    /*
     *
     * */
    ic = avformat_alloc_context();
    int scan_all_pmts_set = 0;

    AVInputFormat *avInputFormat = NULL;
    if (av_stristart(in_path, "rtmp", NULL))
        avInputFormat = av_find_input_format("flv");//指定该流为flv格式

//    if (!av_dict_get(format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE)) {
//        av_dict_set(&format_opts, "scan_all_pmts", "1", AV_DICT_DONT_OVERWRITE);
//        scan_all_pmts_set = 1;
//    }

//    av_dict_set(&format_opts, "probesize", "4096", 0);
//    av_dict_set(&format_opts, "max_delay", "5000000", 0);
    //There is total different meaning for 'timeout' option in rtmp
//    av_dict_set(&format_opts, "timeout", NULL, 0);
    //don't calculate real frame rate
//    av_dict_set_int(&ic->metadata, "skip-calc-frame-rate", 20, 0);
//    av_dict_set_int(&format_opts, "skip-calc-frame-rate", 20, 0);

    ret = avformat_open_input(&ic, in_path, avInputFormat, &format_opts);
    if (ret != 0) {
        return CommonUtil::ff_Error(ret, "avformat_open_input failed.");
    }
    LOGD("avformat_open_input done!");


//    AVDictionaryEntry *t;
//    if (scan_all_pmts_set)
//        av_dict_set(&format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE);
//    if ((t = av_dict_get(format_opts, "", NULL, AV_DICT_IGNORE_SUFFIX))) {
//        LOGD("Option %s not found.\n", t->key);
//    }

    ic->flags |= AVFMT_FLAG_GENPTS;
    av_format_inject_global_side_data(ic);

    ret = avformat_find_stream_info(ic, 0);
    if (ret != 0) {
        return CommonUtil::ff_Error(ret, "avformat_find_stream_info failed.");
    }
    LOGD("avformat_find_stream_info done! frames: %d", ic->nb_streams);

    for (int i = 0; i < ic->nb_streams; ++i) {
        LOGD("codec type: %d", ic->streams[i]->codecpar->codec_type);
        if (ic->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_index = i;
        }
    }
    if (video_index == -1) {
        LOGE("Couldn't find a h264 video Stream!");
        return -1;
    }
//    av_dump_format(ic, 0, in_path, 0);
    LOGD("find a video Stream, video_index: %d!", video_index);

    if (ic->pb)
        ic->pb->eof_reached = 0; // FIXME hack, ffplay maybe should not use avio_feof() to test for the end


//    int best_stream = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, video_index, -1, NULL, 0);
//    LOGD("av_find_best_stream: %d",best_stream);

    AVStream *st = ic->streams[video_index];
//    AVCodecParameters *codecpar = st->codecpar;
//    AVRational sar = av_guess_sample_aspect_ratio(ic, st, NULL);

    auto decode = new H264SoftDecode(st);

    if (decode->code != 0) {
        LOGD("init H264SoftDecode failed!");
        return -1;
    }

    AVPacket *packet = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    while (true) {
        ret = av_read_frame(ic, packet);
        if (ret == 0) {
            if (video_index == packet->stream_index) {
                LOGD("av_read_frame size: %d , pts: %lld dts: %lld", packet->size, packet->pts,
                     packet->dts);
                //执行解码
                decode->decode(frame, packet, pEnv, pJobject);
                av_frame_unref(frame);
            }
        } else {
            av_packet_unref(packet);
            break;
        }

        // 释放数据包和缓冲帧的引用，防止内存泄漏
        av_frame_unref(frame);
        av_packet_unref(packet);
    }
    av_frame_free(&frame);
    av_free(frame);
    frame = NULL;
    av_packet_free(&packet);
    av_free(packet);
    packet = NULL;
    av_dict_free(&format_opts);
    format_opts = NULL;
    free(decode);

    avformat_free_context(ic);


    return 0;
}