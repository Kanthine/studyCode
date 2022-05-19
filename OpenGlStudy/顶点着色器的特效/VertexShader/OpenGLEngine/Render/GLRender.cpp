//
//  GLRender.cpp
//  Ball
//
//  Created by 苏莫离 on 2019/10/17.
//

#include "GLRender.hpp"
#include "GLMatrixState.hpp"
#import "GLResourceManager.h"

GLRender::GLRender() {
    
}

void GLRender::Initialize(int width, int height) {//初始化函数
    glGenRenderbuffers(1, &m_renderbuffer);                    //创建一个渲染缓冲
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);    //绑定上述渲染缓冲
    glGenRenderbuffers(1, &m_depthRenderbuffer);            //创建一个作为深度缓冲的渲染缓冲
    //绑定上述作为深度缓冲的渲染缓冲
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,width, height);//设置缓冲类型及尺寸
    glGenFramebuffers(1, &m_framebuffer);                    //创建一个帧缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);        //绑定上述帧缓冲
    //将第一个创建的渲染缓冲绑定到帧缓冲上，作为其颜色附件
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,m_renderbuffer);
    //将第二个创建的深度缓冲绑定到帧缓冲上，作为其深度附件
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, m_depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);    //绑定作为颜色附件的渲染缓冲
    
    glViewport(0, 0, width, height);//设置视口
    glEnable(GL_CULL_FACE);//启用背面剪裁
    glEnable(GL_DEPTH_TEST);//启用深度测试
    float ratio = (float) width/height;//计算宽高比
    GLMatrixState::setProjectFrustum(-ratio, ratio, -1, 1, 4, 100); //设置投影矩阵
    GLMatrixState::setCamera(0, 0, 5, 0, 0, 0, 0, 1.0, 0.0); //设置摄像机矩阵
    GLMatrixState::setInitStack();   //初始化矩阵

    textureId = GLResourceManager::initTexture("flagTest");  //加载纹理
    earth = new TextureRect();//新建地球对象
}

void GLRender::Render() const {
    glClearColor(0.0f, 0.0f, 0.1f, 1);//设置背景颜色
    //清除深度缓冲与颜色缓冲
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    
    GLMatrixState::pushMatrix(); //保护现场
    GLMatrixState::translate(0, 0, -1);
    GLMatrixState::rotate(yAngle, 0, 1, 0);
    GLMatrixState::rotate(xAngle, 1, 0, 0);
    earth->drawSelf(textureId, 0);//绘制地球
    GLMatrixState::popMatrix();//恢复现场
}


void GLRender::OnFingerDown(float locationx,float locationy){}
void GLRender::OnFingerUp(float locationx,float locationy){}
void GLRender::OnFingerMove(float previousx,float previousy,float currentx,float currenty) {
    
    float dx = currentx - previousx; //计算触控点X位移
    float dy = currenty - previousy; //计算触控点Y位移

    xAngle += (float)dx * 90.0f / 320.0f;
    yAngle += (float)dy * 180.0f / 320.0f;
}
