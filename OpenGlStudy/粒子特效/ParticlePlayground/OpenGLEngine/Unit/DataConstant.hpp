//
//  DataConstant.hpp
//  ParticlePlayground
//
//  Created by 苏莫离 on 2019/5/15.
//

#ifndef DataConstant_hpp
#define DataConstant_hpp

#include <stdio.h>
#import <OpenGLES/ES3/gl.h>
#import <simd/simd.h>

///  本案例中一共有 4 种粒子特效，每一种粒子特效系统中采用的各项参数是不同的
///  具体包括起始颜色、终止颜色、混合因子、混合方式、最大允许生命期、粒子发射的 x 左右范围、每批 激活的粒子数量以及粒子 y 方向升腾的速度等。

/// 4 种不同的火焰效果
const static int kParticleEffectCount = 4;

class DataConstant {
private:
    constexpr const static float distancesFireXZ = 1.0f;
    constexpr const static float distancesBrazierXZ = 1.0f;
public:
    
    static GLint textureIDs[6];
    static int currentIndex;
    
    constexpr const static float wallsLength = 30.0f;
    constexpr const static float positionFireXZ[4][2] = {
        { 1.0f, -DataConstant::distancesFireXZ},
        { 0.35, -DataConstant::distancesFireXZ},
        {-0.35,-DataConstant::distancesFireXZ},
        {-1.0f,-DataConstant::distancesFireXZ}
    };
    constexpr const static float positionBrazierXZ[4][2] = {
        {DataConstant::distancesBrazierXZ,DataConstant::distancesBrazierXZ},
        {DataConstant::distancesBrazierXZ,-DataConstant::distancesBrazierXZ},
        {-DataConstant::distancesBrazierXZ,DataConstant::distancesBrazierXZ},
        {-DataConstant::distancesBrazierXZ,-DataConstant::distancesBrazierXZ}
    };
    constexpr const static GLint SRC_BLEND[kParticleEffectCount] = { // 源混合因子
        GL_SRC_ALPHA,   // 普通火焰
        GL_ONE,         // 白亮火焰
        GL_SRC_ALPHA,   // 普通烟
        GL_ONE,         // 纯黑烟
    };
    
    constexpr const static GLint DST_BLEND[kParticleEffectCount] = { // 目标混合因子
        GL_ONE,
        GL_ONE,
        GL_ONE_MINUS_SRC_ALPHA,
        GL_ONE,
    };
    constexpr const static GLint BLEND_FUNC[kParticleEffectCount] = { // 混合方式
        GL_FUNC_ADD,
        GL_FUNC_ADD,
        GL_FUNC_ADD,
        GL_FUNC_REVERSE_SUBTRACT,
    };
    
    constexpr const static vector_float4 startColor[kParticleEffectCount] = { // 粒子起始颜色
        {0.7569f,0.2471f,0.1176f,1.0f},     // 普通火焰
        {0.7569f,0.2471f,0.1176f,1.0f},     // 白亮火焰
        {0.6f,0.6f,0.6f,1.0f},              // 普通烟
        {0.6f,0.6f,0.6f,1.0f},              // 纯黑烟
    };
    constexpr const static vector_float4 endColor[kParticleEffectCount] = { // 粒子终止颜色
        {0.0f,0.0f,0.0f,0.0f},              // 普通火焰
        {0.0f,0.0f,0.0f,0.0f},              // 白亮火焰
        {0.0f,0.0f,0.0f,0.0f},              // 普通烟
        {0.0f,0.0f,0.0f,0.0f},              // 纯黑烟
    };
    
    constexpr const static float uy[4] = {0.05f, 0.05f, 0.04f, 0.04f}; //粒子y方向升腾的速度
    constexpr const static float radis[4] = {0.5f, 0.5f, 0.8f, 0.8f}; //单个粒子半径
    constexpr const static float maxLiftSpan[4] = {5.0f, 5.0f, 6.0f, 6.0f}; //粒子最大生命期
    constexpr const static float lifeSpanStep[4] = {0.07f, 0.07f, 0.07f, 0.07f}; //粒子生命期步进
    constexpr const static float xRange[4] = {0.5f, 0.5f, 0.5f, 0.5f}; //粒子发射的x左右范围
    constexpr const static float yRange[4] = {0.3f, 0.3f, 0.15f, 0.15f}; //粒子发射的y上下范围
    constexpr const static int count[4] = {340, 340, 99, 99}; // 总粒子数
    constexpr const static int groupCount[4] = {4, 4, 1, 1};
    constexpr const static int threadSleep[4] = {60, 60, 30, 30}; //粒子更新物理线程的休眠时间(ms)
    
    static vector_float3 getCrossProduct(float x1,float y1,float z1,float x2,float y2,float z2){
        float A=y1*z2-y2*z1;
        float B=z1*x2-z2*x1;
        float C=x1*y2-x2*y1;
        return (vector_float3){A,B,C};
    }
    
    static vector_float3 vectorNormal(vector_float3 vector){
        float module = (float)sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
        return (vector_float3){vector[0]/module,vector[1]/module,vector[2]/module};
    }
};

#endif 
