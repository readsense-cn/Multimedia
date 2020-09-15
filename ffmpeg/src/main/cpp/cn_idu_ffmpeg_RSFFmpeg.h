/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class cn_idu_ffmpeg_RSFFmpeg */

#ifndef _Included_cn_idu_ffmpeg_RSFFmpeg
#define _Included_cn_idu_ffmpeg_RSFFmpeg
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     cn_idu_ffmpeg_RSFFmpeg
 * Method:    initRtmpParams
 * Signature: (Ljava/lang/String;II)J
 */
JNIEXPORT jlong JNICALL Java_cn_idu_ffmpeg_RSFFmpeg_initRtmpParams
        (JNIEnv *, jclass, jstring, jint, jint);

/*
 * Class:     cn_idu_ffmpeg_RSFFmpeg
 * Method:    prepareFFMpeg
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_cn_idu_ffmpeg_RSFFmpeg_prepareFFMpeg
        (JNIEnv *, jclass);

/*
 * Class:     cn_idu_ffmpeg_RSFFmpeg
 * Method:    sendVideoFrame
 * Signature: ([BI)V
 */
JNIEXPORT void JNICALL Java_cn_idu_ffmpeg_RSFFmpeg_sendVideoFrame
        (JNIEnv *, jclass, jbyteArray, jint);

/*
 * Class:     cn_idu_ffmpeg_RSFFmpeg
 * Method:    releasePush
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_cn_idu_ffmpeg_RSFFmpeg_releasePush
        (JNIEnv *, jclass);

/*
 * Class:     cn_idu_ffmpeg_RSFFmpeg
 * Method:    decodeMP42YUV
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_cn_idu_ffmpeg_RSFFmpeg_decodeMP42YUV
        (JNIEnv *, jclass, jstring, jstring);

/*
 * Class:     cn_idu_ffmpeg_RSFFmpeg
 * Method:    newRtmpPlayer
 * Signature: (Ljava/lang/String;II)J
 */
JNIEXPORT jlong JNICALL Java_cn_idu_ffmpeg_RSFFmpeg_newRtmpPlayer
        (JNIEnv *, jclass, jstring, jint, jint);

/*
 * Class:     cn_idu_ffmpeg_RSFFmpeg
 * Method:    runRtmpPlayer
 * Signature: ()J
 */
JNIEXPORT void JNICALL Java_cn_idu_ffmpeg_RSFFmpeg_runRtmpPlayer
        (JNIEnv *, jclass, jlong, jobject callback);

/*
 * Class:     cn_idu_ffmpeg_RSFFmpeg
 * Method:    releaseRtmpPlayer
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_cn_idu_ffmpeg_RSFFmpeg_releaseRtmpPlayer
        (JNIEnv *, jclass, jlong);

#ifdef __cplusplus
}
#endif
#endif
