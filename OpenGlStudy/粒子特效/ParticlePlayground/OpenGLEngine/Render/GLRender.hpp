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
#include "Wall.hpp"
#include "ParticleSystem.hpp"
#include "TextureObj.hpp"
#include "GLRenderInterface.hpp"

class GLRender : public GLRenderInterface {
public:
    //声明该类的方法
    GLRender();
    void Initialize(int width, int height);
    void viewSizeChange(int width, int height);

    void Render() const;
    
    void changeCameraParam(float zNear, float zFar, float fov, float zPos);
    void OnFingerUp(float locationx,float locationy);
    void OnFingerDown(float locationx,float locationy);
    void OnFingerMove(float previousx,float previousy,float currentx,float current);
private:
    const float PI = 3.1415926f/180.0f;

    GLuint m_framebuffer;//创建一个帧缓冲区对象
    GLuint m_renderbuffer;//创建一个渲染缓冲区对象
    GLuint m_depthRenderbuffer;
    
    GLuint textureIdFire;    // 系统火焰分配的纹理id
    GLuint textureIdbrazier; // 系统火盆分配的纹理id
    bool flag = true; //线程循环的标志位
    
    float direction=0;  // 视线方向
    vector_float3 cameraLoction = {0, 18, 20}; /// 摄像机坐标
    vector_float3 cameraTarget  = {0, 5, 20};  /// 摄像机目标
    vector_float3 cameraUP      = {            /// 摄像机 UP
        0,
        abs((cameraLoction.x * cameraTarget.x +
             cameraLoction.z * cameraTarget.z -
             cameraLoction.x * cameraLoction.x -
             cameraLoction.z * cameraLoction.z) / (cameraTarget.y - cameraLoction.y)),
        -cameraLoction.z
    };
    
    float DEGREE_SPAN=(float)(3.0/180.0f* PI); // 摄像机每次转动的角度
    float Offset=20;
    float preX; // 触控点x坐标
    float preY; // 触控点y坐标
    float x;
    float y;
    
    float zNear;
    float zFar;
    float fov;
    float zPos;
    float ratio;
    
    ParticleSystem *systemList[4];
    ParticleForDraw *drawList[4];
    WallsForwDraw *wallDraw;
    TextureObj *brazier;
};

struct GLRenderInterface* CreateRenderer() {
    return new GLRender();
}

#endif
