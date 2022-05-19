//
//  GLRender.hpp
//  Mist
//
//  Created by 苏莫离 on 2019/10/17.
//

#ifndef GLRender_hpp
#define GLRender_hpp

#include <iostream>
#import <OpenGLES/ES3/gl.h>
#include "Mountion.hpp"
#include "GLRenderInterface.hpp"

class GLRender : public GLRenderInterface {
public:
    //声明该类的方法
    GLRender();
    void Initialize(int width, int height);
    void viewSizeChange(int width, int height);

    void Render() const;
    
    void changeCameraParam(float zNear, float zFar, float cx, float cz, float tx, float tz);
private:
    const float PI = 3.1415926f/180.0f;

    GLuint m_framebuffer;//创建一个帧缓冲区对象
    GLuint m_renderbuffer;//创建一个渲染缓冲区对象
    GLuint m_depthRenderbuffer;
    
    float ratio;
    
    float DEGREE_SPAN=(float)(3.0/180.0f*PI);//摄像机每次转动的角度
    float CAMERA_Y = 25.0f; //摄像机Y坐标

    float x;
    float y;
    float Offset=20;
    
    
    Mountion *mountion;
};

struct GLRenderInterface* CreateRenderer() {
    return new GLRender();
}

#endif
