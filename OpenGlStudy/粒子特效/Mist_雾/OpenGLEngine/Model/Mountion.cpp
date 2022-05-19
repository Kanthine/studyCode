//
//  Mountion.cpp
//  Mist
//
//  Created by 苏莫离 on 2019/5/15.
//

#include "Mountion.hpp"
#include <math.h>
#include "ShaderTool.h"
#include "GLMatrixState.hpp"
#import <simd/simd.h>
using namespace std;

float Mountion::toRadians(float angle) { //将角度转换为弧度
    return angle * 3.1415926f / 180;
}

Mountion::Mountion(vector<vector<float>> yArray) {
    initVertexData(yArray); // 初始化顶点数据
    initShader(); // 初始化着色器
}

/// 矩形：由大量的小三角形组成
void Mountion::initVertexData(vector<vector<float>> yArray) {
    unsigned long rows = yArray.size() - 1, cols = yArray[0].size() - 1;
    vCount = (int)(rows * cols * 6);
    float sizeWidth = 16.0f / rows, sizeHeight = 16.0f / cols;
    
    vector_float3 *vertices = new vector_float3[vCount];
    vector_float2 *textureCoors = new vector_float2[vCount];

    int verticeIndex = 0;//顶点计数器
    for(int j = 0; j < rows; j++) {
        for(int i = 0; i < cols; i++) {
            //计算当前格子左上侧点坐标
            float zsx = -unitSize * cols / 2 + i * unitSize;
            float zsz = -unitSize * rows / 2 + j * unitSize;
            
            float s = j * sizeWidth, t = i * sizeHeight;
            
            textureCoors[verticeIndex] = (vector_float2){s, t};
            vertices[verticeIndex++] = (vector_float3){zsx, yArray[j][i], zsz};
            
            textureCoors[verticeIndex] = (vector_float2){s, t + sizeHeight};
            vertices[verticeIndex++] = (vector_float3){zsx, yArray[j+1][i], zsz + unitSize};
            
            textureCoors[verticeIndex] = (vector_float2){s + sizeWidth, t};
            vertices[verticeIndex++] = (vector_float3){zsx+unitSize,yArray[j][i+1], zsz};

            textureCoors[verticeIndex] = (vector_float2){s + sizeWidth, t};
            vertices[verticeIndex++] = (vector_float3){zsx+unitSize,yArray[j][i+1], zsz};
            
            textureCoors[verticeIndex] = (vector_float2){s, t + sizeHeight};
            vertices[verticeIndex++] = (vector_float3){zsx,yArray[j+1][i], zsz+unitSize};
            
            textureCoors[verticeIndex] = (vector_float2){s + sizeWidth, t + sizeHeight};
            vertices[verticeIndex++] = (vector_float3){zsx+unitSize,yArray[j+1][i+1], zsz+unitSize};
        }
    }

    mVertexBuffer = vertices;
    mTexCoorBuffer = textureCoors;
}

void Mountion::initShader() {
    //基于顶点着色器与片元着色器创建渲染管线
    mProgram = ShaderTool::createProgram("VertexShader", "FragmentShader");
    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    maTexCoorHandle = glGetAttribLocation(mProgram, "aTexCoor");
    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
    //获取程序中摄像机位置引用的id
    muCamaraLocationHandle = glGetUniformLocation(mProgram, "uCamaraLocation");
    //获取程序中体积雾产生者平面高度引用的id
    slabYHandle = glGetUniformLocation(mProgram, "slabY");
    //获取程序中体积雾高度扰动起始角引用的id
    startAngleHandle = glGetUniformLocation(mProgram, "startAngle");
    
    //纹理
    landStartYYHandle = glGetUniformLocation(mProgram, "landStartY"); //x位置
    landYSpanHandle = glGetUniformLocation(mProgram, "landYSpan");  //x最大
}

void Mountion::drawSelf(void) {
    glUseProgram(mProgram);//指定使用某套着色器程序
    
    glUniformMatrix4fv(muMVPMatrixHandle, 1, GL_FALSE, GLMatrixState::getFinalMatrix()); //将最终变换矩阵传入shader程序
    glUniformMatrix4fv(muMMatrixHandle, 1, GL_FALSE, GLMatrixState::getMMatrix()); //将位置、旋转变换矩阵传入着色器程序
    glUniform3fv(muCamaraLocationHandle, 1, GLMatrixState::cameraFB); //将摄像机位置传入着色器程序
    
    /** 首先将体积雾所需雾平面的高度传入渲染管线，
     * 接着将用于扰动雾平面高度的正弦曲线所需起始角传入渲染管线。
     * 最后增加起始角的值，并通过取模的方式将起始角限制在 0~360 的范围内
     */
    glUniform1f(slabYHandle, TJ_GOG_SLAB_Y); //将体积雾的雾平面高度传入渲染管线
    glUniform1f(startAngleHandle, toRadians(startAngle));  //将体积雾扰动起始角传入渲染管线
    startAngle = startAngle + 90.0f;
    if (startAngle > 360.0f ) {
        startAngle -= 360.0f;
    }
    glVertexAttribPointer(maPositionHandle,3, GL_FLOAT, GL_FALSE,3 * sizeof(float), mVertexBuffer); /// 指定顶点数据
    glVertexAttribPointer(maTexCoorHandle,2, GL_FLOAT, GL_FALSE,2 * sizeof(float), mTexCoorBuffer); /// 指定纹理数据
    glEnableVertexAttribArray(maPositionHandle); //启用顶点位置数据数组
    glEnableVertexAttribArray(maTexCoorHandle);
    
    //传送相应的x参数
    glUniform1f(landStartYYHandle, 0);
    glUniform1f(landYSpanHandle, 50);
    
    //绘制纹理矩形
    glDrawArrays(GL_TRIANGLES, 0, vCount);
}
