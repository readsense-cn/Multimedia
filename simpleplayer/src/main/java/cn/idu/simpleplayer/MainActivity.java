package cn.idu.simpleplayer;

import android.Manifest;

import cn.idu.ffmpeg.InfoCallback;
import cn.idu.ffmpeg.RSFFmpeg;
import cn.idu.ffmpeg.RtmpPlayer;
import cn.idu.gles_helper.ShaderUtil;
import cn.idu.gles_helper.yuv.YUVPreview;
import cn.readsense.module.base.BaseCoreActivity;
import cn.readsense.module.thread.ExecutorPool;
import cn.readsense.module.util.DLog;

public class MainActivity extends BaseCoreActivity {


    @Override
    protected int getLayoutId() {
        requestPermissions(Manifest.permission.CAMERA, Manifest.permission.WRITE_EXTERNAL_STORAGE);
        return R.layout.activity_main;
    }

    YUVPreview yuvPreview;

    @Override
    protected void initView() {
        yuvPreview = findViewById(R.id.yuvpreview);
        yuvPreview.setParams(1920, 1080, 0);
//        yuvPreview.setParams(240, 160, 0);
//        byte[] data = ShaderUtil.readRaw(this, "yuv420p.yuv");
//        yuvPreview.requestRenderer(data, 800, 480, 0);

        RSFFmpeg.prepareFFMpeg();
//        RSFFmpeg.decodeMP42YUV("/sdcard/a1.mp4","/sdcard/a2.yuv");

        RtmpPlayer player = new RtmpPlayer();
        long code = -1;
//        code = player.init("rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov", 320, 480);
        code = player.init("rtmp://10.109.55.229:1935/live/room", 320, 480);
        if (code == 0) {
            ExecutorPool.exec(new Runnable() {
                @Override
                public void run() {
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    player.play(new InfoCallback() {
                        @Override
                        public void onInfoCallback(byte[] data, int width, int height, int format) {
                            DLog.d("width: " + width + " height: " + height + " format:" + format);
                            yuvPreview.requestRenderer(data, width, height, 1);
                        }
                    });
                }
            });
        }
    }
}
