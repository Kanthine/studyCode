@import simd;
@import ModelIO;
@import MetalKit;

#include <stdlib.h>
#import "AAPLRenderer.h"
#import "AAPLShaderTypes.h"

static const NSUInteger AAPLNumLights = 128;
static const NSUInteger AAPLNumFairyVertices = 7; // 精灵模型中的顶点数量

static const NSUInteger kGroupCount = 1;
static const float kLifeStep = 0.07f;

@interface AAPLRenderer ()

{
    CGRect _rectangleFrame;
    id<MTLCommandQueue> _commandQueue;
    id <MTLRenderPipelineState> _rectanglePipelineState;
    id <MTLRenderPipelineState> _fairyPipelineState;

    id<MTLTexture> _fairyMap;
    MTLRenderPassDescriptor *_finalRenderPassDescriptor;
    NSUInteger _frameNumber;
    id<MTLBuffer> _fairy;
    vector_float2 _viewportSize;
    PEUniform _uniform;
}

@property (nonatomic, nonnull) id<MTLBuffer> lightsInfoBuffer;

@end

@implementation AAPLRenderer

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)view {
    self = [super init];
    if(self) {
        _rectangleFrame = CGRectMake(0, 100, 50, 50);
        view.delegate = self;
        _device = view.device;
        _view = view;
        [self loadMetal];
        [self loadScene];
    }
    return self;
}

- (void)loadMetal {
    NSError* error;
    id <MTLLibrary> shaderLibrary = [_device newDefaultLibrary];
    NSAssert(shaderLibrary, @"Failed to load Metal shader library");
        
    _view.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
    
    #pragma mark Rectangle render pipeline setup
    {
        id<MTLFunction> vertexFunction = [shaderLibrary newFunctionWithName:@"vertexShader"];
        id<MTLFunction> fragmentFunction = [shaderLibrary newFunctionWithName:@"fragmentShader"];
        MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"Draw Rectangle";
        pipelineStateDescriptor.vertexFunction = vertexFunction;
        pipelineStateDescriptor.fragmentFunction = fragmentFunction;
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = _view.colorPixelFormat;
        _rectanglePipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor
                                                                 error:&error];
    }

    #pragma mark Fairy billboard render pipeline setup
    {
        id <MTLFunction> fairyVertexFunction = [shaderLibrary newFunctionWithName:@"vertexShaderFP"];
        id <MTLFunction> fairyFragmentFunction = [shaderLibrary newFunctionWithName:@"fragmentShaderFP"];
        
        MTLRenderPipelineDescriptor *renderPipelineDescriptor = [MTLRenderPipelineDescriptor new];
        
        renderPipelineDescriptor.label = @"Fairy Drawing";
        renderPipelineDescriptor.vertexDescriptor = nil;
        renderPipelineDescriptor.vertexFunction = fairyVertexFunction;
        renderPipelineDescriptor.fragmentFunction = fairyFragmentFunction;
        renderPipelineDescriptor.colorAttachments[0].pixelFormat = _view.colorPixelFormat;
        renderPipelineDescriptor.colorAttachments[0].blendingEnabled = YES;
        renderPipelineDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
        renderPipelineDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
        renderPipelineDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
        renderPipelineDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
        renderPipelineDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOne;
        renderPipelineDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOne;

        _fairyPipelineState = [_device newRenderPipelineStateWithDescriptor:renderPipelineDescriptor
                                                                      error:&error];
        NSAssert(_fairyPipelineState, @"Failed to create fairy render pipeline state: %@", error);
    }
    
    // Create the command queue
    _commandQueue = [_device newCommandQueue];    
    _finalRenderPassDescriptor = [MTLRenderPassDescriptor new];
    _finalRenderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 0.0);
    _finalRenderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    _finalRenderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
}

- (void)loadScene {
    [self loadAssets];
    [self initFairData];
}

- (void)loadAssets {
    
    #pragma mark Setup buffer with attributes for each point light/fairy
    {
        _lightsInfoBuffer = [_device newBufferWithLength:sizeof(PointInfo) * AAPLNumLights options:0];
        _lightsInfoBuffer.label = @"info: speed and color";
        NSAssert(_lightsInfoBuffer, @"Could not create lights data buffer");
    }

    #pragma mark Setup 2D circle mesh for fairy billboards
    {
        /// 单位圆等分为 AAPLNumFairyVertices 份
        /// 圆上的等分点集合
        AAPLFairyVertex fairyVertices[AAPLNumFairyVertices];
        const float angle = 2*M_PI/(float)AAPLNumFairyVertices;
        for(int vtx = 0; vtx < AAPLNumFairyVertices; vtx++) {
            int point = (vtx % 2) ? (vtx + 1) / 2 : -vtx / 2;
            vector_float2 position = {sin(point*angle), cos(point*angle)};
            fairyVertices[vtx].position = position;
        }

        _fairy = [_device newBufferWithBytes:fairyVertices length:sizeof(fairyVertices) options:0];
        _fairy.label = @"Fairy Vertices";
    }
    
    #pragma mark Load textures for non-mesh assets
    {
        MTKTextureLoader *textureLoader = [[MTKTextureLoader alloc] initWithDevice:_device];

        NSDictionary *textureLoaderOptions =
        @{
          MTKTextureLoaderOptionTextureUsage       : @(MTLTextureUsageShaderRead),
          MTKTextureLoaderOptionTextureStorageMode : @(MTLStorageModePrivate),
          };
        NSError *error = nil;
        _fairyMap = [textureLoader newTextureWithName:@"FairyMap"
                                          scaleFactor:1.0
                                               bundle:nil
                                              options:textureLoaderOptions
                                                error:&error];

        NSAssert(_fairyMap, @"Could not load fairy texture: %@", error);

        _fairyMap.label = @"Fairy Map";
    }
}

#pragma mark - 粒子状态设定

static float kSpeedY = 2;
static float kSpeedX = 250.0;

/// 获取 [-1, 1] 的随机数
inline static float getRandom() {
    float a1 = (arc4random() % 999999) / 999999.0;
    return a1;
}

/// 初始化粒子的位置、速度、颜色
- (void)initFairData {
    PointInfo *infoData = (PointInfo *)[_lightsInfoBuffer contents];
    for(int i = 0; i < AAPLNumLights; i++) { /// 256 个粒子
        float px = CGRectGetMinX(_rectangleFrame) + CGRectGetWidth(_rectangleFrame) * getRandom();
        float py = CGRectGetMinY(_rectangleFrame) + CGRectGetHeight(_rectangleFrame) * getRandom();
        float vx = (px - CGRectGetMinX(_rectangleFrame)) / kSpeedX; /// X 方向运动速度
        /// (x、y、vx、当前生命期)
        infoData[i].oldPosition = (vector_float2){px, py};
        infoData[i].position = (vector_float2){px, py};
        infoData[i].life = 10.0;
        infoData[i].rate = (vector_float2){vx, kSpeedY};
    }
    
    for (int i = 0; i < kGroupCount; i++) {
        infoData[i].life = kLifeStep;
    }
}

- (void)updateFairyData {
    PointInfo *infoData = (PointInfo *)[_lightsInfoBuffer contents];
    srandom(0x134e5348);
    
    if (_frameNumber > AAPLNumLights / kGroupCount) {
        _frameNumber = 0;
    }
    
    for(int i = 0; i < AAPLNumLights; i++) {
        if (infoData[i].life != 10) { /// 当前粒子为活跃态
            infoData[i].life += kLifeStep;
            if (infoData[i].life > 5.0) { /// 当前生命期大于最大生命期时
                /// 计算粒子下一轮起始位置 x、y 坐标，Vx
                float px = CGRectGetMinX(_rectangleFrame) + CGRectGetWidth(_rectangleFrame) * getRandom();
                float py = CGRectGetMinY(_rectangleFrame) + CGRectGetHeight(_rectangleFrame) * getRandom();
                float vx = (px - CGRectGetMinX(_rectangleFrame)) / kSpeedX; /// X 方向运动速度
                infoData[i].position = (vector_float2){px, py};
                infoData[i].oldPosition = (vector_float2){px, py};
                infoData[i].life = 10.0;
                infoData[i].rate = (vector_float2){vx, kSpeedY};
            } else { /// 生命期小于最大生命期时: 计算粒子的下一位置坐标
//                float angle = 20 / 180.0 * M_PI;
//                float tx = infoData[i].rate.x * infoData[i].life * 10, ty = 20 * sin(tx);
//                float rx = tx * cos(angle) - ty *sin(angle), ry = tx * sin(angle) + ty * cos(angle);
//                infoData[i].position.x = infoData[i].oldPosition.x + rx;
//                infoData[i].position.y = infoData[i].oldPosition.y + ry;
                
                infoData[i].position.x += infoData[i].rate.x;
                infoData[i].position.y += infoData[i].rate.y;
            }
        }
    }
    
    /// 循环遍历一批粒子，并根据激活粒子的索引计数器的值来计算当前所要激活的粒子，判断该粒子是否处于未激活状态，若是，则激活该粒子。
    for(int i = 0; i < kGroupCount; i++) {
        if(infoData[kGroupCount * _frameNumber + i].life == 10.0f) { /// 如果粒子处于未激活态
            infoData[kGroupCount * _frameNumber + i].life = kLifeStep;
        }
    }
    _frameNumber++;
}

#pragma mark - MTKViewDelegate

- (void)drawRectangle:(nonnull id <MTLRenderCommandEncoder>)renderEncoder {
    
    float x = _rectangleFrame.origin.x, y = _rectangleFrame.origin.y;
    float width = _rectangleFrame.size.width, height = _rectangleFrame.size.height;
    vector_float2 triangleVertices[6] =
    {
        { x, y},
        { x + width,  y },
        { x + width,  y + height},
        
        { x, y},
        { x,  y + height},
        { x + width,  y + height},
    };
    [renderEncoder pushDebugGroup:@"Draw Rectangle"];
    [renderEncoder setRenderPipelineState:_rectanglePipelineState];
    [renderEncoder setVertexBytes:&_viewportSize length:sizeof(vector_float2) atIndex:VertexInputViewportSize];
    [renderEncoder setVertexBytes:triangleVertices length:sizeof(triangleVertices) atIndex:VertexInputVertex];
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
    [renderEncoder popDebugGroup];
}


/// 绘制“仙女”在点灯的中心与2D磁盘使用纹理执行平滑alpha混合的边缘
- (void)drawFairies:(nonnull id <MTLRenderCommandEncoder>)renderEncoder {
    [renderEncoder pushDebugGroup:@"Draw Fairies"];
    [renderEncoder setRenderPipelineState:_fairyPipelineState];
    [renderEncoder setVertexBytes:&_viewportSize length:sizeof(vector_float2) atIndex:VertexInputViewportSize];
    [renderEncoder setVertexBytes:&_uniform length:sizeof(PEUniform) atIndex:5];
    [renderEncoder setVertexBuffer:_fairy offset:0 atIndex:VertexInputVertex];
    [renderEncoder setVertexBuffer:_lightsInfoBuffer offset:0 atIndex:VertexInputPoint];
    [renderEncoder setFragmentTexture:_fairyMap atIndex:FragmentInputTexture];
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip vertexStart:0 vertexCount:AAPLNumFairyVertices instanceCount:AAPLNumLights];
    [renderEncoder popDebugGroup];
}

- (void)drawInMTKView:(MTKView *)view  {
    [self updateFairyData];
    if (_uniform.uProgress > 1.0) {
        _uniform.uProgress = 0;
    }
    _uniform.uProgress += 0.001;
    _uniform.uTime += 0.02;
    
    id <MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"Lighting Commands";
    id<MTLTexture> drawableTexture = _view.currentDrawable.texture;
    
    if(drawableTexture) {
        _finalRenderPassDescriptor.colorAttachments[0].texture = drawableTexture;
        id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:_finalRenderPassDescriptor];
        renderEncoder.label = @"Lighting & Composition Pass";
        [self drawRectangle:renderEncoder];
        [self drawFairies:renderEncoder];
        [renderEncoder endEncoding];
    }
    
    id<MTLDrawable> currentDrawable = _view.currentDrawable;
    [commandBuffer addScheduledHandler:^(id<MTLCommandBuffer> _Nonnull commandBuffer) {
        [currentDrawable present];
    }];
    [commandBuffer commit];
}

- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size {
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
    if(view.paused) {
        [view draw];
    }
}

@end
