package cn.idu.simplepush;

public class NativePlayer {
    public native void startPush(String url);

    public native void stopPush();

    public native void release();

    public native void setVideoOptions(int width, int height, int bitrate, int fps);

    public native void setAudioOptions(int sampleRateInHz, int channel);

    public native void sendVideoPacket(byte[] data);

    public native void sendAudioPacket(byte[] data, int len);

    static {
        System.load("iduLivePush");
    }
}
