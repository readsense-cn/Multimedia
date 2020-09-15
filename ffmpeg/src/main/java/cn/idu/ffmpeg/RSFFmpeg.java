package cn.idu.ffmpeg;

public class RSFFmpeg {

    static {
        System.loadLibrary("ffmpegin");
    }

    public static native long initRtmpParams(String url, int width, int height);

    public static native void prepareFFMpeg();

    public static native void sendVideoFrame(byte[] data, int y_len);

    public static native void releasePush();

    public static native void decodeMP42YUV(String path1, String path2);

    public static native long newRtmpPlayer(String url, int width, int height);

    public static native void runRtmpPlayer(long handle, InfoCallback callback);

    public static native long releaseRtmpPlayer(long handle);


}
