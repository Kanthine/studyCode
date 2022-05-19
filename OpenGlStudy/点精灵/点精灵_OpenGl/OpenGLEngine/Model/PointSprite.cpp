//
//  PointSprite.cpp
//  PointSprite
//
//  Created by 苏莫离 on 2019/10/17.
//

#include "PointSprite.hpp"
#import <simd/simd.h>

#include "ShaderTool.h"
#include "GLMatrixState.hpp"

PointSprite::PointSprite() {
    initVertexData(); // 初始化顶点数据
    initShader();     // 初始化着色器
}

void PointSprite::initVertexData() {
    vCount = 9;
    static float size = 1.5;
    static vector_float3 vertices[9] = {
        {0, 0, 0,},
        {0, size * 2, 0,},
        {size, size / 2, 0,},
        {-size/3, size, 0,},
        {-size*0.4f, -size*0.4f, 0,},
        {-size, -size, 0,},
        {size*0.2f, -size*0.7f, 0,},
        {size/2, -size*3/2, 0,},
        {-size*4/5, -size*3/2, 0,}
    };
    mVertexBuffer = vertices;
}

void PointSprite::initShader() {
    mProgram = ShaderTool::createProgram("VertexShader", "FragmentShader");
    /// 获取程序中顶点位置属性引用
    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    /// 获取程序中总变换矩阵引用
    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
}

void PointSprite::drawSelf(int texId) {
    glUseProgram(mProgram);//指定使用某套着色器程序
    glUniformMatrix4fv(muMVPMatrixHandle, 1, GL_FALSE, GLMatrixState::getFinalMatrix());
    glVertexAttribPointer(maPositionHandle,3, GL_FLOAT, GL_FALSE,sizeof(vector_float3), mVertexBuffer); //将顶点位置数据送入渲染管线
    glEnableVertexAttribArray(maPositionHandle); //启用顶点位置数据数组
    glEnable(GL_TEXTURE_2D); //开启纹理
    glActiveTexture(GL_TEXTURE0);    //激活纹理
    glBindTexture(GL_TEXTURE_2D, texId);//绑定纹理
    glDrawArrays(GL_POINTS, 0, vCount);
}
