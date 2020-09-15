package cn.idu.ffmpeg;

public class RtmpPlayer {

    long handle;

    public long init(String url, int width, int height) {
        handle = RSFFmpeg.newRtmpPlayer(url, width, height);
        if (handle != -1)
            return 0;
        return -1;
    }

    public void play(InfoCallback callback) {
        RSFFmpeg.runRtmpPlayer(handle, callback);
    }

    public void unInit() {
        RSFFmpeg.releaseRtmpPlayer(handle);
    }

}
