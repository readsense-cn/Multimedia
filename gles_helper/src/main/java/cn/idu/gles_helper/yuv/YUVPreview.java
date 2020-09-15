package cn.idu.gles_helper.yuv;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import java.util.List;

import cn.idu.gles_helper.IRenderer;

public class YUVPreview extends IGLSurfaceView<YUVRenderer> {


    public YUVPreview(Context context) {
        this(context, null);
    }

    public YUVPreview(Context context, AttributeSet attrs) {
        super(context, attrs);
        loadRenderer(YUVRenderer.class);
        renderer.offerContext(context);
    }

    public void setParams(int width, int height, int format) {
        renderer.setParams(width, height, format);
    }

    public void requestRenderer(byte[] data, int width, int height, int format) {
        renderer.requestRenderer(data);
        requestRender();
    }

    public void requestRenderer(byte[] data, int width, int height, int format, float[] rect) {
        renderer.requestRenderer(data, rect);
        requestRender();
    }
}
