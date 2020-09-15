package cn.idu.ffmpeg;

public interface InfoCallback {
    void onInfoCallback(byte[] data, int width, int height, int format);
}
