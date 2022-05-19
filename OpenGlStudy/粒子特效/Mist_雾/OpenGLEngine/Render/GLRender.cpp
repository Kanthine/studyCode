//
//  GLRender.cpp
//  Mist
//
//  Created by 苏莫离 on 2019/10/17.
//

#include "GLRender.hpp"
#include "GLMatrixState.hpp"
#import "GLResourceManager.h"

GLRender::GLRender() {}

void GLRender::Initialize(int width, int height) { //初始化函数
    glGenRenderbuffers(1, &m_renderbuffer);                 // 创建一个渲染缓冲
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);    // 将该渲染缓冲区对象绑定到管线上
    glGenRenderbuffers(1, &m_depthRenderbuffer);            // 创建一个作为深度缓冲的渲染缓冲
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderbuffer); // 将该深度缓冲区对象绑定到管线上
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,width, height);//设置缓冲类型及尺寸
    glGenFramebuffers(1, &m_framebuffer);                    //创建一个帧缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);        //将该帧染缓冲区对象绑定到管线上
    //将第一个创建的渲染缓冲绑定到帧缓冲上，作为其颜色附件
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,m_renderbuffer);
    //将第二个创建的深度缓冲绑定到帧缓冲上，作为其深度附件
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, m_depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);    ///将该渲染缓冲区对象绑定到管线上
    glEnable(GL_CULL_FACE);//启用背面剪裁
    glEnable(GL_DEPTH_TEST);//启用深度测试
    viewSizeChange(width, height);
    GLMatrixState::setInitStack();   //初始化矩阵

    vector<vector<float>> yArray = GLResourceManager::loadLandforms("land");
    mountion = new Mountion(yArray);
}

void GLRender::viewSizeChange(int width, int height) {
    glViewport(0, 0, width, height);//设置视口
    ratio = (float)width/height;//计算宽高比
    changeCameraParam(0.1, 1000, 0, 60, 0, 0);
}

void GLRender::changeCameraParam(float zNear, float zFar, float cx, float cz, float tx, float tz) {
    GLMatrixState::setProjectFrustum(-ratio, ratio, -1, 1, zNear, zFar);
    GLMatrixState::setCamera(cx,CAMERA_Y,cz,tx,CAMERA_Y-5,tz,0,1,0); //设置摄像机矩阵
}

void GLRender::Render() const {
    glClearColor(0,0,0, 1.0f);//设置背景颜色
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT); //清除深度缓冲与颜色缓冲
    
    // 绘制墙体
    GLMatrixState::pushMatrix(); //保护现场
    mountion -> drawSelf();
    GLMatrixState::popMatrix();//恢复现场
}


