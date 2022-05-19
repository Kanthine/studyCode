//
//  ParticleSystem.m
//  HelloTriangle
//
//  Created by i7y on 2022/2/8.
//  Copyright © 2022 Apple. All rights reserved.
//

#import "ParticleSystem.h"
#include "AAPLShaderTypes.h"

@interface ParticleSystem ()

{
    FPVertex *_vertexsArray;
}

@property (nonatomic, assign) float yAngle;
@property (nonatomic, assign) float lifeSpanStep;
@property (nonatomic, assign) float sx;
@property (nonatomic, assign) float sy;
@property (nonatomic, assign) float xRange;
@property (nonatomic, assign) float yRange;
@property (nonatomic, assign) float vx;
@property (nonatomic, assign) float vy;

@property (nonatomic, assign) int fireCount;

@property (nonatomic, assign) int uCount;
@property (nonatomic, assign) int srcBlend;
@property (nonatomic, assign) int dstBlend;
@property (nonatomic, assign) int blendFunc;
@property (nonatomic, assign) int sleepSpan;
@property (nonatomic, assign) int groupCount;
@property (nonatomic, assign) int sleep;

@end


@implementation ParticleSystem

static inline float getRandom() {
    return arc4random() % 99999999 / 99999999.0;
}

- (instancetype)initWithIndex:(int)index device:(id<MTLDevice>)device {
    self = [super init];
    if (self) {
        self.positionXZ = [DataConstant positionFireWithIndex:index];
        self.startColor = [DataConstant startColorWithIndex:index];
        self.endColor = [DataConstant endColorWithIndex:index];
        self.lifeSpanStep = [DataConstant lifeSpanStepWithIndex:index];
        self.maxLifeSpan = [DataConstant maxLiftSpanWithIndex:index];
        self.groupCount = [DataConstant groupCountWithIndex:index];
        self.xRange = [DataConstant xRangeWithIndex:index];
        self.yRange = [DataConstant yRangeWithIndex:index];
        self.fireCount = [DataConstant fireCountWithIndex:index];
        self.sx = 0;
        self.sy = 0;
        self.vx = 0;
        self.vy = [DataConstant uyWithIndex:index];
        self.halfSize = [DataConstant radisWithIndex:index];
        self.sleep = [DataConstant threadSleepWithIndex:index];
        
        _vertexBuffer = [device newBufferWithLength:(self.vertexCount * sizeof(FPVertex)) options:MTLResourceStorageModeShared];
        [self makeVertexs];
        
        [NSThread detachNewThreadWithBlock:^{
            while (1) {
                [self updateVertexs];
                usleep(self.sleep);
            }
        }];
    }
    return self;
}

/// count 粒子数量
/// 每个粒子对应 6 个顶点(两个三角形)：每个顶点包括 4 个属性值(x、y、vx、当前生命期)
- (void)makeVertexs {
    _vertexsArray = calloc(self.vertexCount, sizeof(FPVertex));

    for(int i = 0; i < self.fireCount; i++) {
        /// 随机计算出每个粒子的发射位置 XY 坐标： 粒子的初始位置在指定的中心点位置附近随机产生
        float px = (float)(_sx + _xRange * (getRandom() * 2 - 1.0f) );
        float py = (float)(_sy + _yRange * (getRandom() * 2 - 1.0f) );
        
        /// 由于期望的火焰是向上逐渐收窄的，因此根据粒子初始位置偏离中心位置 x 坐标的差值确定粒子 x 方向的速度
        float vx = (_sx - px) / 150.0; /// X 方向运动速度
        /// 总的来说 x 方 向速度指向中心点，速度大小与偏离中心点的距离线性相关，偏离越远，速度越大
        
        /// 粒子的生命期值初始为 10，表示粒子是非活跃粒子
        _vertexsArray[i * 6 + 0].position = (vector_float4){px - _halfSize/2, py + _halfSize/2, vx, 10.0f};
        _vertexsArray[i * 6 + 1].position = (vector_float4){px - _halfSize/2, py - _halfSize/2, vx, 10.0f};
        _vertexsArray[i * 6 + 2].position = (vector_float4){px + _halfSize/2, py + _halfSize/2, vx, 10.0f};
        _vertexsArray[i * 6 + 3].position = (vector_float4){px + _halfSize/2, py + _halfSize/2, vx, 10.0f};
        _vertexsArray[i * 6 + 4].position = (vector_float4){px - _halfSize/2, py - _halfSize/2, vx, 10.0f};
        _vertexsArray[i * 6 + 5].position = (vector_float4){px + _halfSize/2, py - _halfSize/2, vx, 10.0f};
        
        _vertexsArray[i * 6 + 0].textureCoord = (vector_float2){0, 0};
        _vertexsArray[i * 6 + 1].textureCoord = (vector_float2){0, 1};
        _vertexsArray[i * 6 + 2].textureCoord = (vector_float2){1, 0};
        _vertexsArray[i * 6 + 3].textureCoord = (vector_float2){1, 0};
        _vertexsArray[i * 6 + 4].textureCoord = (vector_float2){0, 1};
        _vertexsArray[i * 6 + 5].textureCoord = (vector_float2){1, 1};
    }
    
    /// 第一批要发射的各个粒子，将这些粒子的生命期设置为生命期步进值，将粒子激活
    for(int j = 0; j < _groupCount; j++) {
        _vertexsArray[j * 6 + 0].position.w = _lifeSpanStep;  //设置粒子第1个点的生命期，不为10表示粒子处于活跃状态
        _vertexsArray[j * 6 + 1].position.w = _lifeSpanStep;  //设置粒子第2个点的生命期，不为10表示粒子处于活跃状态
        _vertexsArray[j * 6 + 2].position.w = _lifeSpanStep;  //设置粒子第3个点的生命期，不为10表示粒子处于活跃状态
        _vertexsArray[j * 6 + 3].position.w = _lifeSpanStep;  //设置粒子第4个点的生命期，不为10表示粒子处于活跃状态
        _vertexsArray[j * 6 + 4].position.w = _lifeSpanStep;  //设置粒子第5个点的生命期，不为10表示粒子处于活跃状态
        _vertexsArray[j * 6 + 5].position.w = _lifeSpanStep;  //设置粒子第6个点的生命期，不为10表示粒子处于活跃状态
    }
    /// 从后面的片元着色器中可以看到，当粒子生命期为 10.0 时，表示粒子处于未激活状态，是不会被绘制出来的;
    /// 当粒子生命期不为 10.0 时，表示粒子处于活跃状态，会被绘制出来。
    memcpy(_vertexBuffer.contents, _vertexsArray, sizeof(FPVertex) * self.vertexCount);
}

- (void)updateVertexs {
    if(_uCount >= (self.fireCount / _groupCount)) { //计数器值超过总粒子数时重新计数
        _uCount = 0;
    }
    
    /// 遍历所有粒子：判断当前粒子是否处于活跃状态，重新计算处于活跃状态的粒子生命期值
    /// 再判断该粒子的生命期是否大于最大生命期，
    /// 若大于最大生命期，则重新设置该粒子的基本属性值(所有属性值置为下一轮的初始值);
    /// 若不大于最大生命期，则计算粒子的下一位置坐标值
    for(int i = 0; i <self.fireCount; i++) {
        if(_vertexsArray[i * 6].position.w != 10.0f) { /// 当前粒子为活跃态
            _vertexsArray[i * 6 + 0].position.w += _lifeSpanStep;
            _vertexsArray[i * 6 + 1].position.w += _lifeSpanStep;
            _vertexsArray[i * 6 + 2].position.w += _lifeSpanStep;
            _vertexsArray[i * 6 + 3].position.w += _lifeSpanStep;
            _vertexsArray[i * 6 + 4].position.w += _lifeSpanStep;
            _vertexsArray[i * 6 + 5].position.w += _lifeSpanStep;
            
            if(_vertexsArray[i * 6].position.w > self.maxLifeSpan) { //当前生命期大于最大生命期时
                //计算粒子下一轮起始位置 x、y 坐标，x方向的速度
                
                float px = (float)(_sx + _xRange*(getRandom() * 2 - 1.0f));
                float py = (float)(_sy + _yRange*(getRandom() * 2 - 1.0f));
                float vx = (_sx - px) / 150;
                
                _vertexsArray[i * 6 + 0].position = (vector_float4){px - _halfSize/2, py + _halfSize/2, vx, 10.0f};
                _vertexsArray[i * 6 + 1].position = (vector_float4){px - _halfSize/2, py - _halfSize/2, vx, 10.0f};
                _vertexsArray[i * 6 + 2].position = (vector_float4){px + _halfSize/2, py + _halfSize/2, vx, 10.0f};

                _vertexsArray[i * 6 + 3].position = (vector_float4){px + _halfSize/2, py + _halfSize/2, vx, 10.0f};
                _vertexsArray[i * 6 + 4].position = (vector_float4){px - _halfSize/2, py - _halfSize/2, vx, 10.0f};
                _vertexsArray[i * 6 + 5].position = (vector_float4){px + _halfSize/2, py - _halfSize/2, vx, 10.0f};
            } else { //生命期小于最大生命期时——计算粒子的下一位置坐标
                
                //计算粒子对应的第一个顶点的 x、y 坐标
                _vertexsArray[i * 6 + 0].position.x += _vertexsArray[i * 6 + 0].position.z;
                _vertexsArray[i * 6 + 0].position.y += _vy;
                
                //计算粒子对应的第二个顶点的 x、y 坐标
                _vertexsArray[i * 6 + 1].position.x += _vertexsArray[i * 6 + 1].position.z;
                _vertexsArray[i * 6 + 1].position.y += _vy;

                _vertexsArray[i * 6 + 2].position.x += _vertexsArray[i * 6 + 2].position.z;
                _vertexsArray[i * 6 + 2].position.y += _vy;

                _vertexsArray[i * 6 + 3].position.x += _vertexsArray[i * 6 + 3].position.z;
                _vertexsArray[i * 6 + 3].position.y += _vy;

                _vertexsArray[i * 6 + 4].position.x += _vertexsArray[i * 6 + 4].position.z;
                _vertexsArray[i * 6 + 4].position.y += _vy;

                _vertexsArray[i * 6 + 5].position.x += _vertexsArray[i * 6 + 5].position.z;
                _vertexsArray[i * 6 + 5].position.y += _vy;
            }
        }
    }

    ///循环遍历一批粒子，并根据激活粒子的索引计数器的值来计算当前所要激活的粒子，判断该粒子是否处于未激活状态，若是，则激活该粒子。
    for(int i = 0; i < _groupCount; i++) {
        if(_vertexsArray[_groupCount * _uCount + i * 6].position.w == 10.0f) { /// 如果粒子处于未激活态
            _vertexsArray[_groupCount * _uCount + i * 6].position.w = _lifeSpanStep;
            _vertexsArray[_groupCount * _uCount + i * 6 + 1].position.w = _lifeSpanStep;
            _vertexsArray[_groupCount * _uCount + i * 6 + 2].position.w = _lifeSpanStep;
            _vertexsArray[_groupCount * _uCount + i * 6 + 3].position.w = _lifeSpanStep;
            _vertexsArray[_groupCount * _uCount + i * 6 + 4].position.w = _lifeSpanStep;
            _vertexsArray[_groupCount * _uCount + i * 6 + 5].position.w = _lifeSpanStep;
        }
    }
    
    memcpy(_vertexBuffer.contents, _vertexsArray, sizeof(FPVertex) * self.vertexCount);
    _uCount ++;  //下次激活粒子的索引
}

- (int)vertexCount {
    return self.fireCount * 6;
}

@end




