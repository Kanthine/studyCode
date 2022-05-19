//
//  GLMatrixState.hpp
//  PointSprite
//
//  Created by 苏莫离 on 2019/10/17.
//

#ifndef GLMatrixState_hpp
#define GLMatrixState_hpp

#include <cmath>
#include "GLMatrix.hpp"

class GLMatrixState {
    
private:
    static float currMatrix[16];
    static float mProjMatrix[16];
    static float mVMatrix[16];
    static float mMVPMatrix[16];
public:
    static GLfloat* cameraFB;
    
    //保护变换矩阵的栈
    static float mStack[10][16];
    static int stackTop;
    
    static void setInitStack() { //获取不变换初始矩阵
        GLMatrix::setIdentityM(currMatrix,0);
    }
    
    static void pushMatrix() { //保护变换矩阵
        stackTop++;
        for(int i = 0; i<16; i++) {
            mStack[stackTop][i] = currMatrix[i];
        }
    }
    
    static void popMatrix() { //恢复变换矩阵
        for(int i = 0; i < 16; i++) {
            currMatrix[i] = mStack[stackTop][i];
        }
        stackTop--;
    }
    
    static void translate(float x,float y,float z) { //设置沿xyz轴移动
        GLMatrix::translateM(currMatrix, 0, x, y, z);
    }
    
    static void rotate(float angle,float x,float y,float z) { //设置绕xyz轴移动
        GLMatrix::rotateM(currMatrix,0,angle,x,y,z);
    }
    
    static void scale(float x,float y,float z) {
        GLMatrix::scaleM(currMatrix,0, x, y, z);
    }
    
    /// 设置摄像机矩阵
    static void setCamera(float cx, float cy, float cz, /// 摄像机位置坐标
                          float tx, float ty, float tz, /// 摄像机目标坐标
                          float upx, float upy,float upz){  /// 摄像机 UP 方向
        GLMatrix::setLookAtM(mVMatrix, 0, cx, cy, cz, tx, ty, tz, upx, upy, upz);
        static GLfloat cameraLocation[3];//摄像机位置
        cameraLocation[0] = cx;
        cameraLocation[1] = cy;
        cameraLocation[2] = cz;
        cameraFB = cameraLocation;
    }
    
    /// 设置透视矩阵
    static void setProjectFrustum(float left, float right, float bottom, float top, float near, float far) {
        GLMatrix::frustumM(mProjMatrix, 0, left, right, bottom, top, near, far);
    }
    
    /// 获取总变换矩阵
    static float* getFinalMatrix() {
        GLMatrix::multiplyMM(mMVPMatrix, 0, mVMatrix, 0, currMatrix, 0);
        GLMatrix::multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);
        return mMVPMatrix;
    }
    
    /// 获取具体物体的变换矩阵
    static float* getMMatrix() {
        return &currMatrix[0];
    }
    
    //获取投影矩阵
    static float* getProjMatrix() {
        return mProjMatrix;
    }
    
    //获取摄像机朝向的矩阵
    static float* getCaMatrix() {
        return mVMatrix;
        
    }
};

#endif

