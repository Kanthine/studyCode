//
//  Ball.cpp
//  Ball
//
//  Created by 苏莫离 on 2019/10/17.
//

#include "Ball.hpp"
#include <math.h>

#include "ShaderTool.h"
#include "GLMatrixState.hpp"

Ball::Ball() {
    initVertexData(); // 初始化顶点数据
    initShader();     // 初始化着色器
}

/// 将角度转换为弧度
float Ball::toRadians(float angle) {
    return angle * 3.1415926f / 180;
}

/** 任何形状的 3D 物体都是用三角形拼凑而成的， 因此构建曲面物体最重要的就是找到将曲面恰当拆分成三角形的策略
 *  最基本的策略是按照一定的规则将物体按行和列两个方向进行拆分，得到很多的小四边形；然后再将每个小四边形拆分成两个三角形即可；
 *  球面首先被按照纬度 (行)和经度(列)的方向拆分成了很多的小四边形，每个小四边形又被拆分成两个小三角形
 *  这种拆分方式下，三角形中每个顶点的坐标都可以用解析几何的公式方便地计算出来，具体情况如下：
 *  x = R×cos(α)cosB; y = R×cos(α)sinB; z = R×sin(α)
 *  上述给出的是当球的半径为 R，在纬度为 α ，经度为 B 处球面上顶点坐标的计算公式。
 *  对曲面物体进行拆分时，拆分得越细，最终的绘制结果就越接近真实情况
 */
void Ball::initVertexData() {
    const float angleSpan = 10; //将球进行单位切分的角度
    float alVertixs[11664]; //36*18*6*3
    static float *alVertix;
    alVertix = alVertixs;
    for (int vAngle = -90; vAngle < 90; vAngle = vAngle + angleSpan) { //垂直方向 angleSpan 度一份
        for (int hAngle = 0; hAngle < 360; hAngle = hAngle + angleSpan) { //水平方向angleSpan度一份
            
            //纵向横向各到一个角度后计算对应的此点在球面上的坐标
            float x1 = (float) (r * cos(toRadians(vAngle)) * cos(toRadians(hAngle)));
            float y1 = (float) (r * cos(toRadians(vAngle)) * sin(toRadians(hAngle)));
            float z1 = (float) (r * sin(toRadians(vAngle)));
            
            float x2 = (float) (r * cos(toRadians(vAngle)) * cos(toRadians(hAngle + angleSpan)));
            float y2 = (float) (r * cos(toRadians(vAngle)) * sin(toRadians(hAngle + angleSpan)));
            float z2 = (float) (r * sin(toRadians(vAngle)));
            
            float x3 = (float) (r * cos(toRadians(vAngle + angleSpan)) * cos(toRadians(hAngle + angleSpan)));
            float y3 = (float) (r * cos(toRadians(vAngle + angleSpan)) * sin(toRadians(hAngle + angleSpan)));
            float z3 = (float) (r * sin(toRadians(vAngle + angleSpan)));
            
            float x4 = (float) (r * cos(toRadians(vAngle + angleSpan)) * cos(toRadians(hAngle)));
            float y4 = (float) (r * cos(toRadians(vAngle + angleSpan)) * sin(toRadians(hAngle)));
            float z4 = (float) (r * sin(toRadians(vAngle + angleSpan)));
            
            //构建第一三角形
            *alVertix = x1; alVertix++;
            *alVertix = y1; alVertix++;
            *alVertix = z1; alVertix++;
            
            *alVertix = x2; alVertix++;
            *alVertix = y2; alVertix++;
            *alVertix = z2; alVertix++;
            
            *alVertix = x4; alVertix++;
            *alVertix = y4; alVertix++;
            *alVertix = z4; alVertix++;
            
            //构建第二三角形
            *alVertix = x4; alVertix++;
            *alVertix = y4; alVertix++;
            *alVertix = z4; alVertix++;
            
            *alVertix = x2; alVertix++;
            *alVertix = y2; alVertix++;
            *alVertix = z2; alVertix++;
            
            *alVertix = x3; alVertix++;
            *alVertix = y3; alVertix++;
            *alVertix = z3; alVertix++;
        }
    }
    vCount = 11664 / 3; //顶点的数量为坐标值数量的1/3，因为一个顶点有3个坐标
    //将alVertix中的坐标值转存到一个float数组中
    int count = 11664;//坐标值数量
    static float ver[11664];//声明顶点位置数组
    for(int i = 0; i < count; i++) { //遍历所有的坐标值
        ver[i] = alVertixs[i];
    }
    mVertexBuffer = ver;
}

void Ball::initShader() {
    mProgram = ShaderTool::createProgram("VertexShader", "FragmentShader");
    /// 获取程序中顶点位置属性引用
    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    /// 获取程序中总变换矩阵引用
    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    /// 获取程序中球半径的引用
    muRHandle = glGetUniformLocation(mProgram, "uR");
}

void Ball::drawSelf() {
    GLMatrixState::rotate(xAngle, 1.0, 0, 0);
    GLMatrixState::rotate(yAngle, 0, 1.0, 0);
    GLMatrixState::rotate(zAngle, 0, 0, 1.0);
    glUseProgram(mProgram);//指定使用某套着色器程序
    glUniformMatrix4fv(muMVPMatrixHandle, 1, GL_FALSE, GLMatrixState::getFinalMatrix());
    glUniform1f(muRHandle, r * 1.0); // 将半径属性传入渲染管线
    glVertexAttribPointer(maPositionHandle,3, GL_FLOAT, GL_FALSE,3*4, mVertexBuffer); //将顶点位置数据送入渲染管线
    glEnableVertexAttribArray(maPositionHandle); //启用顶点位置数据数组
    glDrawArrays(GL_TRIANGLES, 0, vCount); //绘制球
}
