//
//  TextureRect.cpp
//  VertexShader
//
//  Created by 苏莫离 on 2019/10/17.
//

#include "TextureRect.hpp"
#include <math.h>

#include "ShaderTool.h"
#include "GLMatrixState.hpp"

using namespace std;

TextureRect::TextureRect() {
    initVertexData(); // 初始化顶点数据
    
    // 初始化着色器
    initShader(0, "VertexShader_X");
    initShader(1, "VertexShader_Xie");
    initShader(2, "VertexShader_XY");
}

/// 矩形：由大量的小三角形组成
void TextureRect::initVertexData() {
    int cols = 12, rows = cols * 3 / 4;              /// 12 列、 16 行
    float UNIT_SIZE = WIDTH_SPAN / cols;    /// 网格单位长
    float sizew = 1.0f / cols, sizeh = 1.0f / rows;  /// 纹理单位长、宽
    
    vCount = cols * rows * 6; /// 12 * 16 * 6 = 1152
    static float alVertix[1152 * 3];
    static float alTextures[1152 * 2];
    int index = 0, indexTexture = 0;
    
    for (int j = 0; j < rows; j++) {
        float zs_Y = UNIT_SIZE * rows / 2 - j * UNIT_SIZE;
        float zs_Z = 0;
        float s = j * sizew;
        
        for (int i = 0; i < cols; i++) {
            /// 顶点坐标
            float zs_X = -WIDTH_SPAN / 2 + i * UNIT_SIZE;
            alVertix[index++] = zs_X;
            alVertix[index++] = zs_Y;
            alVertix[index++] = zs_Z;
            
            alVertix[index++] = zs_X;
            alVertix[index++] = zs_Y - UNIT_SIZE;
            alVertix[index++] = zs_Z;
            
            alVertix[index++] = zs_X + UNIT_SIZE;
            alVertix[index++] = zs_Y;
            alVertix[index++] = zs_Z;

            alVertix[index++] = zs_X + UNIT_SIZE;
            alVertix[index++] = zs_Y;
            alVertix[index++] = zs_Z;
            
            alVertix[index++] = zs_X;
            alVertix[index++] = zs_Y - UNIT_SIZE;
            alVertix[index++] = zs_Z;
            
            alVertix[index++] = zs_X + UNIT_SIZE;
            alVertix[index++] = zs_Y - UNIT_SIZE;
            alVertix[index++] = zs_Z;
            
            /// 纹理坐标
            float t = i * sizeh;
            alTextures[indexTexture++] = s;
            alTextures[indexTexture++] = t;
            
            alTextures[indexTexture++] = s;
            alTextures[indexTexture++] = t + sizeh;
            
            alTextures[indexTexture++] = s + sizew;
            alTextures[indexTexture++] = t;
            
            alTextures[indexTexture++] = s + sizew;
            alTextures[indexTexture++] = t;
            
            alTextures[indexTexture++] = s;
            alTextures[indexTexture++] = t + sizeh;
            
            alTextures[indexTexture++] = s + sizew;
            alTextures[indexTexture++] = t + sizeh;
        }
    }
    
    mVertexBuffer = alVertix;
    mTexCoorBuffer = alTextures;
}

void TextureRect::initShader(int index, const string &vartexName) {
    mProgram[index] = ShaderTool::createProgram(vartexName, "FragmentShader");
    /// 获取程序中顶点位置属性引用
    maPositionHandle[index] = glGetAttribLocation(mProgram[index], "aPosition");
    maTexCoorHandle[index]= glGetAttribLocation(mProgram[index], "aTexCoor");
    /// 获取程序中总变换矩阵引用
    muMVPMatrixHandle[index] = glGetUniformLocation(mProgram[index], "uMVPMatrix");
    maStartAngleHandle[index] = glGetUniformLocation(mProgram[index], "uStartAngle");
    muWidthSpanHandle[index] = glGetUniformLocation(mProgram[index], "uWidthSpan");
}

void TextureRect::drawSelf(int texId, float currStartAngle) {
    glUseProgram(mProgram[currIndex]);//指定使用某套着色器程序
    glUniformMatrix4fv(muMVPMatrixHandle[currIndex], 1, GL_FALSE, GLMatrixState::getFinalMatrix());
    glUniform1f(maStartAngleHandle[currIndex], currStartAngle);
    glUniform1f(muWidthSpanHandle[currIndex], WIDTH_SPAN);
    glVertexAttribPointer(maPositionHandle[currIndex],3, GL_FLOAT, GL_FALSE,3 * sizeof(float), mVertexBuffer); //将顶点位置数据送入渲染管线
    glVertexAttribPointer(maTexCoorHandle[currIndex],2, GL_FLOAT, GL_FALSE,2 * sizeof(float), mTexCoorBuffer); //将顶点位置数据送入渲染管线
    glEnableVertexAttribArray(maPositionHandle[currIndex]); //启用顶点位置数据数组
    glEnableVertexAttribArray(maTexCoorHandle[currIndex]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glDrawArrays(GL_TRIANGLES, 0, vCount);
}
