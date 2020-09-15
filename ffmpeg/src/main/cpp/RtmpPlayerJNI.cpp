//
// Created by loki on 2020/8/26.
//

#include "cn_idu_ffmpeg_RSFFmpeg.h"
#include "RtmpPlayer.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_cn_idu_ffmpeg_RSFFmpeg_newRtmpPlayer(JNIEnv *env, jclass clazz, jstring url, jint width,
                                          jint height) {
    int ret = -1;
    const char *url_ = env->GetStringUTFChars(url, 0);
    auto player = new RtmpPlayer();
    ret = player->init(url_, width, height);
    env->ReleaseStringUTFChars(url, url_);
    if (ret == 0) {
        auto handle = (jlong) (player);
        return handle;
    }
    return ret;
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_idu_ffmpeg_RSFFmpeg_runRtmpPlayer(JNIEnv *env, jclass clazz, jlong handle,
                                          jobject callback) {
    auto player = (RtmpPlayer *) handle;
    player->play(env, callback);
}