package cn.idu.gles_helper.yuv;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.Matrix;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import cn.idu.gles_helper.IRenderer;
import cn.idu.gles_helper.ShaderUtil;

class YUVRenderer implements IRenderer {
    public final static int YUV420P = 0;
    public final static int NV21 = 1;

    private int program = -1;
    private int positionLoc = -1;
    private int texCoordLoc = -1;
    private int yTextureLoc = -1;
    private int uTextureLoc = -1;
    private int vTextureLoc = -1;
    private int uvTextureLoc = -1;
    private int matrixLoc = -1;
    private int yuvtype = -1;
    private int type = -1;

    private float[] matrix = new float[16];

    private int imageWidth;
    private int imageHeight;

    //float类型的字节数
    private static final int BYTES_PER_FLOAT = 4;
    private FloatBuffer vertexBuffer;
    private float[] VERTEXT = {
            -1f, -1f, 0.0f, 1.0f,
            1f, -1f, 1.0f, 1.0f,
            -1f, 1f, 0.0f, 0.0f,
            1f, 1f, 1.0f, 0.0f,

            -0.5f, 0.5f,   // top left
            -0.5f, -0.5f,   // bottom left
            0.5f, -0.5f,   // bottom right
            0.5f, 0.5f
    };

    private ByteBuffer yBuffer;
    private ByteBuffer uBuffer;
    private ByteBuffer vBuffer;
    private ByteBuffer uvBuffer;

    private int yTextureId = -1;
    private int uTextureId = -1;
    private int vTextureId = -1;
    private int uvTextureId = -1;
    private byte[] yuv;

    private Context context;

    private float scale_x = 0;
    private float scale_y = 0;


    public void setParams(int width, int height, int format) {
        imageWidth = width;
        imageHeight = height;
        this.type = format;

        if (this.yuv == null) {
            this.yuv = new byte[width * height * 3 / 2];
        }

        if (this.yBuffer == null) {
            allocateBuffer();
        }

    }


    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        program = ShaderUtil.createProgram(
                ShaderUtil.loadFromAssetsFile("vertex_yuv.glsl", context.getResources()),
                ShaderUtil.loadFromAssetsFile("fragment_yuv.glsl", context.getResources())
        );
        positionLoc = GLES20.glGetAttribLocation(program, "aPosition");
        texCoordLoc = GLES20.glGetAttribLocation(program, "aTexCoord");

        yTextureLoc = GLES20.glGetUniformLocation(program, "yTexture");
        uTextureLoc = GLES20.glGetUniformLocation(program, "uTexture");
        vTextureLoc = GLES20.glGetUniformLocation(program, "vTexture");
        uvTextureLoc = GLES20.glGetUniformLocation(program, "uvTexture");
        matrixLoc = GLES20.glGetUniformLocation(program, "matrix");
        yuvtype = GLES20.glGetUniformLocation(program, "yuvtype");


        vertexBuffer = ByteBuffer.allocateDirect(VERTEXT.length * BYTES_PER_FLOAT)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer();
        vertexBuffer.put(VERTEXT);
        allocateBuffer();
    }

    public void offerContext(Context context) {
        this.context = context;
    }

    private void allocateBuffer() {
        yBuffer = ByteBuffer.allocateDirect(imageWidth * imageHeight).order(ByteOrder.nativeOrder());
        if (type == NV21) {
            uvBuffer = ByteBuffer.allocateDirect(imageWidth * imageHeight / 2).order(ByteOrder.nativeOrder());
            int[] textureObjectIds = new int[2];
            GLES20.glGenTextures(2, textureObjectIds, 0);
            yTextureId = textureObjectIds[0];
            uvTextureId = textureObjectIds[1];
        } else {
            uBuffer = ByteBuffer.allocateDirect(imageWidth * imageHeight / 4).order(ByteOrder.nativeOrder());
            vBuffer = ByteBuffer.allocateDirect(imageWidth * imageHeight / 4).order(ByteOrder.nativeOrder());
            int[] textureObjectIds = new int[3];
            GLES20.glGenTextures(3, textureObjectIds, 0);
            yTextureId = textureObjectIds[0];
            uTextureId = textureObjectIds[1];
            vTextureId = textureObjectIds[2];
        }
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0, 0, width, height);
        Matrix.setIdentityM(matrix, 0);
        scale_x = 1f * imageWidth / (float) width;
        scale_y = 1f * imageHeight / (float) height;
        Matrix.scaleM(matrix, 0, scale_x, scale_y, 1f);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
//        long time = System.currentTimeMillis();
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        GLES20.glUseProgram(program);

        vertexBuffer.position(0);
        GLES20.glEnableVertexAttribArray(positionLoc);
        GLES20.glVertexAttribPointer(
                positionLoc, 2, GLES20.GL_FLOAT, false, 4 * 4, vertexBuffer
        );


        vertexBuffer.position(2);
        GLES20.glEnableVertexAttribArray(texCoordLoc);
        GLES20.glVertexAttribPointer(
                texCoordLoc, 2, GLES20.GL_FLOAT, false, 4 * 4, vertexBuffer
        );

        GLES20.glUniformMatrix4fv(matrixLoc, 1, false, matrix, 0);

        GLES20.glUniform1i(yuvtype, type);
        if (yuv != null && yBuffer != null) {
            yBuffer.put(yuv, 0, imageWidth * imageHeight);
            yBuffer.position(0);
            textureLuminance(yBuffer, imageWidth, imageHeight, yTextureId);
            if (type == NV21) {
                uvBuffer.put(yuv, imageWidth * imageHeight, imageWidth * imageHeight / 2);
                uvBuffer.position(0);
                textureLuminanceAlpha(uvBuffer, imageWidth / 2, imageHeight / 2, uvTextureId);

                GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
                GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, yTextureId);
                GLES20.glUniform1i(yTextureLoc, 0);

                GLES20.glActiveTexture(GLES20.GL_TEXTURE1);
                GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, uvTextureId);
                GLES20.glUniform1i(uvTextureLoc, 1);
            } else if (type == YUV420P) {
                uBuffer.put(yuv, imageWidth * imageHeight, imageWidth * imageHeight / 4);
                uBuffer.position(0);
                textureLuminance(uBuffer, imageWidth / 2, imageHeight / 2, uTextureId);
                vBuffer.put(yuv, imageWidth * imageHeight * 5 / 4, imageWidth * imageHeight / 4);
                vBuffer.position(0);
                textureLuminance(vBuffer, imageWidth / 2, imageHeight / 2, vTextureId);

                GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
                GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, yTextureId);
                GLES20.glUniform1i(yTextureLoc, 0);

                GLES20.glActiveTexture(GLES20.GL_TEXTURE1);
                GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, uTextureId);
                GLES20.glUniform1i(uTextureLoc, 1);

                GLES20.glActiveTexture(GLES20.GL_TEXTURE2);
                GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, vTextureId);
                GLES20.glUniform1i(vTextureLoc, 2);
            }
        }

        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
        GLES20.glDisableVertexAttribArray(texCoordLoc);

        if (showRect) {
            vertexBuffer.position(16);
            vertexBuffer.put(rect);
            vertexBuffer.position(16);
            GLES20.glVertexAttribPointer(positionLoc, 2,
                    GLES20.GL_FLOAT, false, 0, vertexBuffer);
            GLES20.glDrawArrays(GLES20.GL_LINE_LOOP, 0, 4);
        }
        GLES20.glDisableVertexAttribArray(positionLoc);

//        System.out.println("renderer cost: " + (System.currentTimeMillis() - time));
    }


    private void textureLuminance(ByteBuffer imageData, int width, int height, int textureId) {
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureId);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexImage2D(
                GLES20.GL_TEXTURE_2D, 0,
                GLES20.GL_LUMINANCE, width, height, 0,
                GLES20.GL_LUMINANCE,
                GLES20.GL_UNSIGNED_BYTE, imageData
        );
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
    }

    private void textureLuminanceAlpha(ByteBuffer imageData, int width, int height, int textureId) {
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureId);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexImage2D(
                GLES20.GL_TEXTURE_2D, 0,
                GLES20.GL_LUMINANCE_ALPHA, width, height, 0,
                GLES20.GL_LUMINANCE_ALPHA,
                GLES20.GL_UNSIGNED_BYTE, imageData
        );
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
    }

    public void requestRenderer(byte[] data) {
        System.arraycopy(data, 0, this.yuv, 0, data.length);
        showRect = false;
    }

    public void requestRenderer(byte[] data, float[] rect) {
        System.arraycopy(data, 0, this.yuv, 0, data.length);
//        this.rect[0] = rect[0];
//        this.rect[1] = rect[1];
//
//        this.rect[4] = rect[0];
//        this.rect[5] = rect[3];
//
//        this.rect[6] = rect[2];
//        this.rect[7] = rect[3];
//
//        this.rect[2] = rect[2];
//        this.rect[3] = rect[1];
        showRect = true;
    }

    private float[] rect = new float[8];
    public boolean showRect;

}

