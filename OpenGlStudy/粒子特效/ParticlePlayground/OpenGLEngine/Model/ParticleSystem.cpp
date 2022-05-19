//
//  ParticleSystem.cpp
//  ParticlePlayground
//
//  Created by 苏莫离 on 2019/5/15.
//

#include "ParticleSystem.hpp"
#include "ShaderTool.h"
#include "GLMatrixState.hpp"
#include "DataConstant.hpp"
#include <thread>
#include <unistd.h>

using namespace std;

static inline float getRandom() {
    return rand() % 999999999 / 999999999.0;
}

#pragma mark - ParticleForDraw : 绘制粒子群

/// 绘制粒子群
ParticleForDraw::ParticleForDraw(float halfSize) {
    this -> halfSize = halfSize;
    initShader();
}

void ParticleForDraw::setVertexData(vector_float4 *points, int length) {
    mVertexBuffer = points;
    vCount = length;
}

void ParticleForDraw::initShader() {
    mProgram = ShaderTool::createProgram("VertexShader", "FragmentShader");
    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    muLifeSpan = glGetUniformLocation(mProgram, "maxLifeSpan");
    muBj = glGetUniformLocation(mProgram, "bj");
    muStartColor = glGetUniformLocation(mProgram, "startColor");
    muEndColor = glGetUniformLocation(mProgram, "endColor");
}

void ParticleForDraw::drawSelf(int texId, vector_float4 startColor,vector_float4 endColor,float maxLifeSpan) {
    glUseProgram(mProgram);
    glUniformMatrix4fv(muMVPMatrixHandle, 1, GL_FALSE, GLMatrixState::getFinalMatrix());
    glUniform1f(muLifeSpan, maxLifeSpan);
    glUniform1f(muBj, halfSize * 60);
    glUniform4fv(muStartColor, 1, (GLfloat *)&startColor);
    glUniform4fv(muEndColor, 1, (GLfloat *)&endColor);
    glEnableVertexAttribArray(maPositionHandle);
    glVertexAttribPointer(maPositionHandle, 4, GL_FLOAT, GL_FALSE, sizeof(vector_float4), mVertexBuffer); // 将顶点坐标数据送入渲染管线
    glDrawArrays(GL_POINTS, 0, vCount); // 执行绘制
}


#pragma mark - ParticleSystem : 粒子系统的总控制

ParticleSystem::ParticleSystem(int index, ParticleForDraw *fpfd) {
    this -> positionX = DataConstant::positionFireXZ[index][0];  //初始化此粒子系统绘制位置的 x 坐标
    this -> positionZ = DataConstant::positionFireXZ[index][1];
    this -> fireCount = DataConstant::count[index];
    this -> startColor = DataConstant::startColor[index];  //初始化粒子的起始颜色
    this -> endColor = DataConstant::endColor[index];
    this -> srcBlend = DataConstant::SRC_BLEND[index];
    this -> dstBlend = DataConstant::DST_BLEND[index];
    this -> blendFunc = DataConstant::BLEND_FUNC[DataConstant::currentIndex];
    this -> maxLifeSpan= DataConstant::maxLiftSpan[DataConstant::currentIndex];
    this -> lifeSpanStep = DataConstant::lifeSpanStep[DataConstant::currentIndex];
    this -> groupCount = DataConstant::groupCount[DataConstant::currentIndex];
    this -> sleepSpan = DataConstant::threadSleep[DataConstant::currentIndex];
    this -> sx = 0;
    this -> sy = 0;
    this -> xRange = DataConstant::xRange[DataConstant::currentIndex];
    this -> yRange = DataConstant::yRange[DataConstant::currentIndex];
    this -> vx = 0;
    this -> vy = DataConstant::uy[DataConstant::currentIndex];
    this -> halfSize = DataConstant::radis[DataConstant::currentIndex];
    initPoints(); //初始化所有粒子对应的顶点数据数组
    fpfd -> setVertexData(firePoints, fireCount);
    this -> fpfd = fpfd;
    
        
    thread th1([this]{
        while (1) {
            update();
            usleep(sleepSpan);
        }
    });
    th1.detach();
}

void ParticleSystem::initPoints() {
    vector_float4 *points = new vector_float4[fireCount];
    
    /// 每个粒子对应 6 个顶点(两个三角形)：每个顶点包括 4 个属性值(x、y、vx、当前生命期)
    for(int i = 0; i < fireCount; i++) {
        
        /// 随机计算出每个粒子的发射位置 XY 坐标： 粒子的初始位置在指定的中心点位置附近随机产生
        float px = (float)(sx + xRange * (getRandom() * 2 - 1.0f));
        float py = (float)(sy + yRange * (getRandom() * 2 - 1.0f));
        
        /// 由于期望的火焰是向上逐渐收窄的，因此根据粒子初始位置偏离中心位置 x 坐标的差值确定粒子 x 方向的速度
        float vx = (sx - px) / 150.0; /// X 方向运动速度
        /// 总的来说 x 方 向速度指向中心点，速度大小与偏离中心点的距离线性相关，偏离越远，速度越大
        points[i] = (vector_float4){px, py, vx, 10.0f};
    }
    
    /// 第一批要发射的各个粒子，将这些粒子的生命期设置为生命期步进值，将粒子激活
    for(int j = 0; j < groupCount;j++) {
        points[j].w = lifeSpanStep; //设置粒子第1个点的生命期，不为10表示粒子处于活跃状态
    }
    /// 从后面的片元着色器中可以看到，当粒子生命期为 10.0 时，表示粒子处于未激活状态，是不会被绘制出来的;
    /// 当粒子生命期不为 10.0 时，表示粒子处于活跃状态，会被绘制出来。
    this -> firePoints = points;
}

/// 绘制此粒子系统中所有粒子
/// 其详细步骤为:先关闭深度检测、开启混合、设置混合方法及混合因子，再执行 平移变换、旋转变换、保护现场、绘制所有粒子，最后恢复现场、开启深度检测以及关闭混合
void ParticleSystem::drawSelf(GLuint texId) {
    glDisable(GL_DEPTH_TEST);   /// 关闭深度测试
    glEnable(GL_BLEND);         /// 开启混合
    glBlendEquation(blendFunc); /// 设置混合方式
    glBlendFunc(srcBlend,dstBlend); /// 设置混合因子
    
    /// 由于本案例中粒子系统产生的特效实际是 2D 的，因此在绘制粒子系统之前需要 提示 执行相应的旋转变换，将粒子系统旋转到正对摄像机的角度。
    GLMatrixState::translate(positionX, 0, positionZ);
    GLMatrixState::scale(0.3, 0.3, 0.3);
    GLMatrixState::rotate(yAngle, 0, 1, 0);
    GLMatrixState::pushMatrix();
    fpfd -> drawSelf(texId, startColor, endColor, maxLifeSpan); /// 绘制粒子群
    GLMatrixState::popMatrix();
    
    glEnable(GL_DEPTH_TEST);    /// 开启深度测试
    glDisable(GL_BLEND);        /// 关闭混合
}

/// 该方法主要负责更新整个粒子系统中所有粒子的基本属性值
void ParticleSystem::update() {
    if(uCount >= (fireCount / groupCount)) { //计数器值超过总粒子数时重新计数
        uCount = 0;
    }
        
    /// 遍历所有粒子：判断当前粒子是否处于活跃状态，重新计算处于活跃状态的粒子生命期值
    /// 再判断该粒子的生命期是否大于最大生命期，
    /// 若大于最大生命期，则重新设置该粒子的基本属性值(所有属性值置为下一轮的初始值);
    /// 若不大于最大生命期，则计算粒子的下一位置坐标值
    for(int i = 0; i < fireCount; i++) {
        if(firePoints[i].w != 10.0f) { /// 当前粒子为活跃态
            firePoints[i].w += lifeSpanStep;
            if(firePoints[i].w > maxLifeSpan) { //当前生命期大于最大生命期时
                
                //计算粒子下一轮起始位置 x、y 坐标，x方向的速度
                float px=(float) (sx+xRange*(getRandom()*2-1.0f));
                float py=(float) (sy+yRange*(getRandom()*2-1.0f));
                float vx=(sx-px)/150;

                firePoints[i] = (vector_float4){px, py, vx, 10.0f};
                
            } else { //生命期小于最大生命期时——计算粒子的下一位置坐标
                
                //计算粒子对应的第一个顶点的 x、y 坐标
                firePoints[i].x += firePoints[i].z;
                firePoints[i].y += vy;
            }
        }
    }

    ///循环遍历一批粒子，并根据激活粒子的索引计数器的值来计算当前所要激活的粒子，判断该粒子是否处于未激活状态，若是，则激活该粒子。
    for(int i = 0; i < groupCount; i++) {
        if(firePoints[groupCount * uCount + i].w == 10.0f) { /// 如果粒子处于未激活态
            firePoints[groupCount * uCount + i].w = lifeSpanStep;
        }
    }
    fpfd -> setVertexData(firePoints, fireCount);
    uCount ++;  //下次激活粒子的索引
}
