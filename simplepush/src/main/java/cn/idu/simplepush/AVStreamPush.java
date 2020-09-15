package cn.idu.simplepush;

import android.hardware.Camera;

import cn.readsense.module.camera1.CameraView;

public class AVStreamPush extends BasePush {
    private VideoInfo videoInfo;
    private VideoPush videoPush;
    private AudioInfo audioInfo;
    private AudioPush audioPush;
    private NativePlayer player;
    private CameraView cameraView;
    private String url;


    public AVStreamPush(CameraView cameraView, String url) {
        this.cameraView = cameraView;
        this.url = url;
        prepare();
    }


    private void prepare() {
        player = new NativePlayer();

        videoInfo = new VideoInfo(640, 480, Camera.CameraInfo.CAMERA_FACING_BACK);

        videoPush = new VideoPush(cameraView, player, videoInfo);

        audioInfo = new AudioInfo();

        audioPush = new AudioPush(audioInfo, player);
    }

    @Override
    public void startPush() {
        audioPush.startPush();
        videoPush.startPush();
        player.startPush(url);
    }

    @Override
    public void stopPush() {
        audioPush.stopPush();
        videoPush.stopPush();
        player.stopPush();
    }

    @Override
    public void release() {
        audioPush.release();
        videoPush.release();
        player.release();
    }
}
