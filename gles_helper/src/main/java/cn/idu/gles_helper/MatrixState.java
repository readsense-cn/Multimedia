package cn.idu.gles_helper;

import android.opengl.Matrix;

public class MatrixState {

    private static float[] mProjMatrix = new float[16]; //4*4投影矩阵
    private static float[] mVMatrix = new float[16];    //摄像机位置朝向9参数矩阵
    private static float[] mMVPMatrix;                    //最终的总变换矩阵

    public static void setCamera(float eyeX, float eyeY, float eyeZ,            //摄像机位置的xyz坐标
                                 float centerX, float centerY, float centerZ,   //观察目标点xyz坐标
                                 float upX, float upY, float upZ) {             //up向量在xyz轴上的分量
        Matrix.setLookAtM(mVMatrix, 0, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
    }

    /**
     * 设置正交投影的方法
     */
    public static void setProjectOrtho(float left, float right, //near面的left，right
                                       float bottom, float top, //near面的bottom，top
                                       float near, float far) { //near面、far面与视点的距离
        Matrix.orthoM(mProjMatrix, 0, left, right, bottom, top, near, far);
    }

    /**
     * 设置正交投影的方法
     */
    public static void setProjectFrustumM(float left, float right, //near面的left，right
                                          float bottom, float top, //near面的bottom，top
                                          float near, float far) { //near面、far面与视点的距离
        Matrix.frustumM(mProjMatrix, 0, left, right, bottom, top, near, far);
    }

    /**
     * 生成物体总变换矩阵的方法
     *
     * @param spec
     * @return
     */
    public static float[] getFinalMatrix(float[] spec) {

        mMVPMatrix = new float[16];
        Matrix.multiplyMM(mMVPMatrix, 0, mVMatrix, 0, spec, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);
        return mMVPMatrix;
    }


}
