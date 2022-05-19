//
//  GLRender.hpp
//  PointSprite
//
//  Created by 苏莫离 on 2019/10/17.
//

#ifndef GLRender_hpp
#define GLRender_hpp

#include <iostream>
#import <OpenGLES/ES3/gl.h>
#include "PointSprite.hpp"
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
        
    GLuint texId;
    PointSprite *pointSprite; //地球对象的指针
};

struct GLRenderInterface* CreateRenderer() {
    return new GLRender();
}

#endif
