//
//  GLRender.hpp
//  Ball
//
//  Created by 苏莫离 on 2019/10/17.
//

#ifndef GLRender_hpp
#define GLRender_hpp

#include <iostream>
#import <OpenGLES/ES3/gl.h>
#include "Ball.hpp"
#include "GLRenderInterface.hpp"

class GLRender : public GLRenderInterface {
public:
    //声明该类的方法
    GLRender();
    void Initialize(int width, int height);
    void Render() const;
private:
    GLuint m_framebuffer;//创建一个帧缓冲区对象
    GLuint m_renderbuffer;//创建一个渲染缓冲区对象
    GLuint m_depthRenderbuffer;
        
    static float eAngle;//地球自转角度
    Ball *earth; //地球对象的指针
    const float PI = 3.1415926f/180.0f;
};

float GLRender::eAngle = 0;

struct GLRenderInterface* CreateRenderer() {
    return new GLRender();
}

#endif
