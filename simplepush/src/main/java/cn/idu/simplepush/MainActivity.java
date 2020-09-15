package cn.idu.simplepush;

import android.Manifest;

import java.sql.SQLSyntaxErrorException;

import cn.idu.ffmpeg.RSFFmpeg;
import cn.readsense.module.base.BaseCoreActivity;
import cn.readsense.module.camera1.CameraView;
import cn.readsense.module.util.DLog;

public class MainActivity extends BaseCoreActivity {

    private static final String URL = "";


    private CameraView cameraView;
    private byte[] i420;

    private int width = 640;
    private int height = 480;
    long rtmpParams = -1;

    @Override
    protected int getLayoutId() {
        requestPermissions(Manifest.permission.CAMERA, Manifest.permission.WRITE_EXTERNAL_STORAGE);
        return R.layout.activity_main;
    }

    @Override
    protected void initView() {
        cameraView = findViewById(R.id.cameraview);

        cameraView.getCameraParams().setScaleWidth(true);
        cameraView.getCameraParams().getPreviewSize().setPreviewWidth(width);
        cameraView.getCameraParams().getPreviewSize().setPreviewHeight(height);
        cameraView.addPreviewFrameCallback(new CameraView.PreviewFrameCallback() {
            @Override
            public Object analyseData(byte[] data) {
                if (rtmpParams == 0) {
                    long time = System.currentTimeMillis();
                    if (i420 == null || i420.length != data.length) i420 = new byte[data.length];
                    convertNV212I420(data, width * height, i420);
                    DLog.d("convertNV212I420 cost: " + (System.currentTimeMillis() - time));
                    time = System.currentTimeMillis();
                    RSFFmpeg.sendVideoFrame(i420, width * height);
                    DLog.d("sendVideoFrame cost: "+(System.currentTimeMillis() - time));
                }
                return null;
            }

            @Override
            public void analyseDataEnd(Object t) {

            }
        });

        addLifecycleObserver(cameraView);

        RSFFmpeg.prepareFFMpeg();
        rtmpParams = RSFFmpeg.initRtmpParams("rtmp://10.109.60.186:1935/live", width, height);
    }

    void convertNV212I420(byte[] NV21, int y_len, byte[] i420) {
        System.arraycopy(NV21, 0, i420, 0, y_len);
        int uv_len = y_len / 4;
        for (int i = 0; i < uv_len; i++) {
            i420[y_len + i] = NV21[y_len + 2 * i + 1];
            i420[y_len + uv_len + i] = NV21[y_len + 2 * i];
        }
    }
}
