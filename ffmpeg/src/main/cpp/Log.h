//
// Created by loki on 2020/8/13.
//

#ifndef MULTIMEDIA_LOG_H
#define MULTIMEDIA_LOG_H

#define LOGN (void) 0

#include <android/log.h>

#define LOG_VERBOSE     1
#define LOG_DEBUG       2
#define LOG_INFO        3
#define LOG_WARNING     4
#define LOG_ERROR       5
#define LOG_FATAL       6
#define LOG_SILENT      7

#ifndef LOG_TAG
//#define LOG_TAG __FILE__
#define LOG_TAG "DFF"
#endif

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_VERBOSE
#endif

#define LOGP(level, fmt, ...) \
    __android_log_print(level, LOG_TAG, "[%s : %u]: " fmt, __PRETTY_FUNCTION__,__LINE__, ##__VA_ARGS__)

#if LOG_VERBOSE >= LOG_LEVEL
#define LOGV(fmt, ...) LOGP(ANDROID_LOG_VERBOSE, fmt, ##__VA_ARGS__)
#else
#define LOGV(...) LOGN
#endif

#if LOG_DEBUG >= LOG_LEVEL
#define LOGD(fmt, ...) LOGP(ANDROID_LOG_DEBUG, fmt, ##__VA_ARGS__)
#else
#define LOGD(...) LOGN
#endif

#if LOG_INFO >= LOG_LEVEL
#define LOGI(fmt, ...) LOGP(ANDROID_LOG_INFO, fmt, ##__VA_ARGS__)
#else
#define LOGI(...) LOGN
#endif

#if LOG_WARNING >= LOG_LEVEL
#define LOGW(fmt, ...) LOGP(ANDROID_LOG_WARNING, fmt, ##__VA_ARGS__)
#else
#define LOGW(...) LOGN
#endif


#if LOG_ERROR >= LOG_LEVEL
#define LOGE(fmt, ...) LOGP(ANDROID_LOG_ERROR, fmt, ##__VA_ARGS__)
#else
#define LOGE(...) LOGN
#endif


#if LOG_FATAL >= LOG_LEVEL
#define LOGF(fmt, ...) LOGP(ANDROID_LOG_FATAL, fmt, ##__VA_ARGS__)
#else
#define LOGF(...) LOGN
#endif

#endif //MULTIMEDIA_LOG_H
