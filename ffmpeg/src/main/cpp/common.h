//
// Created by loki on 2020/8/26.
//

#ifndef MULTIMEDIA_COMMON_H
#define MULTIMEDIA_COMMON_H

extern "C" {
#include "libavcodec/avcodec.h"
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavdevice/avdevice.h>
#include <libavutil/time.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavutil/avstring.h>
#include "Log.h"
#include "jni.h"

}

class CommonUtil {
public:
    static int ff_Error(int errNum, const char event[]) {
        char buf[1024] = {0};
        av_strerror(errNum, buf, sizeof(buf));
        LOGE("ff_Error %s : %d - %s", event, errNum, buf);
        return -1;
    }
};


#endif //MULTIMEDIA_COMMON_H
