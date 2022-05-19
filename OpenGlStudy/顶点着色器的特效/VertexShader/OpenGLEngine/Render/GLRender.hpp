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
#include "TextureRect.hpp"
#include "GLRenderInterface.hpp"

class GLRender : public GLRenderInterface {
public:
    //声明该类的方法
    GLRender();
    void Initialize(int width, int height);
    void Render() const;
    
    void OnFingerUp(float locationx,float locationy);
    void OnFingerDown(float locationx,float locationy);
    void OnFingerMove(float previousx,float previousy,float currentx,float current);
private:
    GLuint m_framebuffer;//创建一个帧缓冲区对象
    GLuint m_renderbuffer;//创建一个渲染缓冲区对象
    GLuint m_depthRenderbuffer;
        
    GLuint textureId; //系统分配的纹理id
    
    static float xAngle;
    static float yAngle;
    
    float currStartAngle = 0;
    
    TextureRect *earth; //地球对象的指针
    const float PI = 3.1415926f/180.0f;
};

float GLRender::xAngle = 0;
float GLRender::yAngle = 0;

struct GLRenderInterface* CreateRenderer() {
    return new GLRender();
}

#endif
