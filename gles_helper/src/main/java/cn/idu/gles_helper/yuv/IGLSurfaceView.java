package cn.idu.gles_helper.yuv;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import cn.idu.gles_helper.IRenderer;

public abstract class IGLSurfaceView<T extends GLSurfaceView.Renderer> extends GLSurfaceView {
    public T renderer;
    public Context context;

    public IGLSurfaceView(Context context) {
        this(context, null);

    }

    public IGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        this.context = context;
    }

    protected void loadRenderer(Class<T> cls) {
        if (renderer == null) {
            try {
                setEGLContextClientVersion(2);
                renderer = cls.newInstance();
                setRenderer(renderer);
                setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
            } catch (IllegalAccessException | InstantiationException e) {
                e.printStackTrace();
            }
        }
    }

}
