//
//  Wall.cpp
//  ParticlePlayground
//
//  Created by 苏莫离 on 2019/5/15.
//

#include "Wall.hpp"
#include <math.h>

#include "ShaderTool.h"
#include "GLMatrixState.hpp"
#include "DataConstant.hpp"

using namespace std;


Wall::Wall(float wallsLength) {
    this -> wallsLength = wallsLength;
    initVertexData(wallsLength); // 初始化顶点数据
    initShader(); // 初始化着色器
}

/// 矩形：由大量的小三角形组成
void Wall::initVertexData(float wallsLength) {
    vCount = 6;
    static float alVertix[6 * 3] = {
        -wallsLength, 0, -wallsLength,
         wallsLength, 0,  wallsLength,
        -wallsLength, 0,  wallsLength,
        -wallsLength, 0, -wallsLength,
         wallsLength, 0, -wallsLength,
         wallsLength, 0,  wallsLength,
    };
    mVertexBuffer = alVertix;
    
    static float normalVertix[6 * 3] = {0,1,0, 0,1,0, 0,1,0, 0,1,0, 0,1,0, 0,1,0};
    mNormalBuffer = normalVertix;
    
    static float texCoorVertix[6 * 2] = {0,0, 1,1, 0,1, 0,0, 1,0, 1,1};
    mTexCoorBuffer = texCoorVertix;
}

void Wall::initShader() {
    //基于顶点着色器与片元着色器创建渲染管线
    mProgram = ShaderTool::createProgram("VertexShader_brazier", "FragmentShader_brazier");
    maNormalHandle= glGetAttribLocation(mProgram, "aNormal");  //获取顶点颜色属性引用
    muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    maLightLocationHandle = glGetUniformLocation(mProgram, "uLightLocation");
    maTexCoorHandle = glGetAttribLocation(mProgram, "aTexCoor");
    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    maCameraHandle = glGetUniformLocation(mProgram, "uCamera");
}

void Wall::drawSelf(GLuint texId) {
    glUseProgram(mProgram);//指定使用某套着色器程序
    
    glUniformMatrix4fv(muMVPMatrixHandle, 1, GL_FALSE, GLMatrixState::getFinalMatrix()); //将最终变换矩阵传入shader程序
    glUniformMatrix4fv(muMMatrixHandle, 1, GL_FALSE, GLMatrixState::getMMatrix()); //将位置、旋转变换矩阵传入着色器程序
    glUniform3fv(maLightLocationHandle, 1, GLMatrixState::lightPositionFB); //将光源位置传入着色器程序
    glUniform3fv(maCameraHandle, 1, GLMatrixState::cameraFB); //将摄像机位置传入着色器程序
    glVertexAttribPointer(maPositionHandle,3, GL_FLOAT, GL_FALSE,3 * sizeof(float), mVertexBuffer); /// 指定顶点数据
    glVertexAttribPointer(maNormalHandle,3, GL_FLOAT, GL_FALSE,3 * sizeof(float), mNormalBuffer);   /// 指定法向量数据
    glVertexAttribPointer(maTexCoorHandle,2, GL_FLOAT, GL_FALSE,2 * sizeof(float), mTexCoorBuffer); /// 指定纹理数据
    
    glEnableVertexAttribArray(maPositionHandle); //启用顶点位置数据数组
    glEnableVertexAttribArray(maNormalHandle);
    glEnableVertexAttribArray(maTexCoorHandle);
    
    //绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    
    //绘制纹理矩形
    glDrawArrays(GL_TRIANGLES, 0, vCount);
}





WallsForwDraw::WallsForwDraw() {
    wall = new Wall(DataConstant::wallsLength);
}

void WallsForwDraw::drawSelf() {
    
    //绘制第一面墙-底
    GLMatrixState::pushMatrix();
    GLMatrixState::translate(0, 0, 0);
    wall -> drawSelf(DataConstant::textureIDs[0]);
    GLMatrixState::popMatrix();
    
    //绘制第二面墙-上
    GLMatrixState::pushMatrix();
    GLMatrixState::translate(0, 2 * DataConstant::wallsLength, 0);
    wall -> drawSelf(DataConstant::textureIDs[1]);
    GLMatrixState::popMatrix();
    
    //绘制第三面墙-左
    GLMatrixState::pushMatrix();
    GLMatrixState::translate(-DataConstant::wallsLength, DataConstant::wallsLength, 0);
    GLMatrixState::rotate(90, 0, 0, 1);
    GLMatrixState::rotate(-90, 0, 1, 0);
    wall -> drawSelf(DataConstant::textureIDs[2]);
    GLMatrixState::popMatrix();
    
    //绘制第四面墙-右
    GLMatrixState::pushMatrix();
    GLMatrixState::translate(DataConstant::wallsLength, DataConstant::wallsLength, 0);
    GLMatrixState::rotate(-90, 0, 0, 1);
    GLMatrixState::rotate(90, 0, 1, 0);
    wall -> drawSelf(DataConstant::textureIDs[3]);
    GLMatrixState::popMatrix();
    
    //绘制第五面墙-前
    GLMatrixState::pushMatrix();
    GLMatrixState::translate(0, DataConstant::wallsLength,DataConstant::wallsLength);
    GLMatrixState::rotate(90, 1, 0, 0);
    wall -> drawSelf(DataConstant::textureIDs[4]);
    GLMatrixState::popMatrix();
    
    //绘制第六面墙-后
    GLMatrixState::pushMatrix();
    GLMatrixState::translate(0, DataConstant::wallsLength,-DataConstant::wallsLength);
    GLMatrixState::rotate(90, 1, 0, 0);
    GLMatrixState::rotate(180, 0, 0, 1);
    wall -> drawSelf(DataConstant::textureIDs[5]);
    GLMatrixState::popMatrix();
}
