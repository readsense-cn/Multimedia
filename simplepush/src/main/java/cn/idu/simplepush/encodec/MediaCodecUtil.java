package cn.idu.simplepush.encodec;

import android.media.MediaCodecInfo;
import android.media.MediaFormat;

public class MediaCodecUtil {
    private static final MediaCodecUtil ourInstance = new MediaCodecUtil();
    private final int width = 640;
    private final int height = 480;

    public static MediaCodecUtil getInstance() {
        return ourInstance;
    }

    private MediaCodecUtil() {

        MediaFormat mediaFormat = MediaFormat.createVideoFormat(MediaFormat.MIMETYPE_VIDEO_AVC, width, height);
        mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420SemiPlanar);
        // 马率
        mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, width * height * 5);
        // 调整码率的控流模式
        mediaFormat.setInteger(MediaFormat.KEY_BITRATE_MODE, MediaCodecInfo.EncoderCapabilities.BITRATE_MODE_VBR);
        // 设置帧率
        mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, 30);
        // 设置 I 帧间隔
        mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 1);
    }


}
