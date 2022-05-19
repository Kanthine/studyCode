//
//  GLRender.cpp
//  Ball
//
//  Created by 苏莫离 on 2019/10/17.
//

#include "GLRender.hpp"
#include "GLMatrixState.hpp"
#import "GLResourceManager.h"
#include "DataConstant.hpp"

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
        
    /********************** 初始化纹理 *******************/
    string images[6] = {"wall0", "wall1", "wall2", "wall3", "wall4", "wall5"};
    for (int i = 0; i < 6; i++) {
        DataConstant::textureIDs[i] = GLResourceManager::initTexture(images[i]);  //加载纹理
    }
    textureIdbrazier = GLResourceManager::initTexture("brazier");  //加载纹理
    textureIdFire = GLResourceManager::initTexture("fire");  //加载纹理
    
    /// 创建粒子系统
    for (int i = 0; i < kParticleEffectCount; i++) {
        DataConstant::currentIndex = i;
        ParticleForDraw *draw = new ParticleForDraw(DataConstant::radis[i]);
        ParticleSystem *system = new ParticleSystem(i, draw);
        drawList[i] = draw;
        systemList[i] = system;
    }
    wallDraw = new WallsForwDraw();
    brazier = TextureLoader::loadTextureFromObjFile("brazier");
}

void GLRender::viewSizeChange(int width, int height) {
    glViewport(0, 0, width, height);//设置视口
    ratio = (float)width/height;//计算宽高比
    changeCameraParam(0.1, 300.0f, 1.0, 3.0);
}

void GLRender::Render() const {
    glClearColor(0.6f,0.3f,0.0f, 1.0f);//设置背景颜色
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT); //清除深度缓冲与颜色缓冲
    
    // 绘制墙体
    GLMatrixState::pushMatrix(); //保护现场
    wallDraw -> drawSelf();
//    GLMatrixState::translate(0, 2.5f, 0);
    
//    for(int i = 0; i < kParticleEffectCount; i++){
//        GLMatrixState::pushMatrix();
//        GLMatrixState::translate(DataConstant::positionBrazierXZ[i][0],-2.0f,DataConstant::positionBrazierXZ[i][1]);
//        if(brazier != NULL) {
//             brazier -> drawSelf(textureIdbrazier);
//        }
//        GLMatrixState::popMatrix();
//    }
    
//    GLMatrixState::translate(0, 0.65f, 3);
    for(int i = 0; i < kParticleEffectCount;i++) {
        GLMatrixState::pushMatrix();
        systemList[i] -> drawSelf(textureIdFire);
        GLMatrixState::popMatrix();
    }
    GLMatrixState::popMatrix();//恢复现场
}

void GLRender::changeCameraParam(float zNear, float zFar, float fov, float zPos) {
    
    GLMatrixState::setProjectFrustum(-0.3f*ratio, 0.3f*ratio, -1*0.3f, 1*0.3f, zNear, zFar);
    GLMatrixState::setCamera(0,0.5,0,
                             0,0.5,zPos,
                             0,1.5,0); //设置摄像机矩阵
    GLMatrixState::setInitStack();   //初始化矩阵
    GLMatrixState::setLightLocation(0, 15, 0); //初始化光源位置
}

void GLRender::OnFingerDown(float locationx,float locationy){}
void GLRender::OnFingerUp(float locationx,float locationy){}
void GLRender::OnFingerMove(float previousx,float previousy,float currentx,float currenty) {
    
//    float dx = currentx - previousx; //计算触控点X位移
//    float dy = currenty - previousy; //计算触控点Y位移
//
//    xAngle += (float)dx * 90.0f / 320.0f;
//    yAngle += (float)dy * 180.0f / 320.0f;
}
