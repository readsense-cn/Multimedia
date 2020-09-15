package cn.idu.simplepush;

import android.hardware.Camera;

import cn.readsense.module.camera1.CameraView;

public class VideoPush extends BasePush {

    private CameraView cameraView;
    boolean isPushing;
    boolean init;
    private NativePlayer player;
    private VideoInfo videoInfo;

    public VideoPush(CameraView cameraView, NativePlayer player, VideoInfo videoInfo) {
        this.cameraView = cameraView;
        this.player = player;
        this.videoInfo = videoInfo;
    }

    @Override
    public void startPush() {
        isPushing = true;

        if (!init) {
            init = true;

            player.setVideoOptions(videoInfo.getWidth(), videoInfo.getHeight(), videoInfo.getBitrate(), videoInfo.getFps());

            cameraView.getCameraParams().setFacing(Camera.CameraInfo.CAMERA_FACING_FRONT);
            cameraView.getCameraParams().getPreviewSize().setPreviewWidth(videoInfo.getWidth());
            cameraView.getCameraParams().getPreviewSize().setPreviewHeight(videoInfo.getHeight());

            cameraView.addPreviewFrameCallback(new CameraView.PreviewFrameCallback() {
                @Override
                public Object analyseData(byte[] data) {
                    if (isPushing) {
                        player.sendVideoPacket(data);
                    }
                    return null;
                }

                @Override
                public void analyseDataEnd(Object t) {

                }
            });
            cameraView.showCameraView();
        }

    }

    @Override
    public void stopPush() {
        isPushing = false;
    }

    @Override
    public void release() {
        if (cameraView != null) cameraView.releaseCamera();
        init = false;
    }

}
