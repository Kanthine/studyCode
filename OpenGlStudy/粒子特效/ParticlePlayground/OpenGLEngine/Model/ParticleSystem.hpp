//
//  ParticleSystem.hpp
//  ParticlePlayground
//
//  Created by 苏莫离 on 2019/5/15.
//

#ifndef ParticleSystem_hpp
#define ParticleSystem_hpp

#include <iostream>
#import <OpenGLES/ES3/gl.h>
#import <simd/simd.h>

using namespace std;
class ParticleForDraw {
    GLuint mProgram;
    GLuint muMVPMatrixHandle;
    GLuint muLifeSpan;
    GLuint muBj;
    GLuint muStartColor;
    GLuint muEndColor;
    GLuint maPositionHandle;    
    const GLvoid *mVertexBuffer;//顶点坐标数据缓冲
    int vCount = 0;
    float halfSize; /// 粒子半径
public:
    ParticleForDraw(float halfSize);
    void setVertexData(vector_float4 *points, int length);
    void initShader();
    void drawSelf(int texId, vector_float4 startColor, vector_float4 endColor,float maxLifeSpan);
};



/// 火焰粒子系统的总控制
///  主要实现了对所有粒子位置的计算以及该位置所对应的 6 个顶点坐标值的计算
///  同时还实现了定时更新粒子位置以及根据摄像机位置计算火焰朝向等
class ParticleSystem {
    bool flag = true;
    float yAngle = 0;
    float halfSize;
    float positionX;
    float positionZ;
    
    int uCount = 1;

    
    vector_float4 startColor; /// RGBA
    vector_float4 endColor;
    int srcBlend;
    int dstBlend;
    int blendFunc;
    float maxLifeSpan;
    float lifeSpanStep;
    int sleepSpan;
    int groupCount;
    float sx;
    float sy;
    float xRange;
    float yRange;
    float vx;
    float vy;
    
    vector_float4 *firePoints;
    int fireCount;
    
    ParticleForDraw *fpfd;

    void initPoints();
public:
    ParticleSystem(int index, ParticleForDraw *fpfd);
    void drawSelf(GLuint texId);
    void update();
};

#endif 
