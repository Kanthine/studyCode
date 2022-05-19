@import simd;
@import ModelIO;
@import MetalKit;

#include <stdlib.h>
#import "AAPLRenderer.h"
#import "AAPLShaderTypes.h"

static const NSUInteger AAPLNumLights = 128;


@interface AAPLRenderer ()

{
    id<MTLCommandQueue> _commandQueue;
    id <MTLRenderPipelineState> _fairyPipelineState;

    MTLRenderPassDescriptor *_finalRenderPassDescriptor;
    NSUInteger _frameNumber;
    vector_float2 _viewportSize;
    PEUniform _uniform;
}

@property (nonatomic, nonnull) id<MTLBuffer> lightsInfoBuffer;

@end

@implementation AAPLRenderer

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)view {
    self = [super init];
    if(self) {
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
        float px = 100 ;
        float py = 100;
        float vx = (px - 100) / kSpeedX; /// X 方向运动速度
        /// (x、y、vx、当前生命期)
        infoData[i].oldPosition = (vector_float2){px, py};
        infoData[i].position = (vector_float2){px, py};
        infoData[i].rate = (vector_float2){vx, kSpeedY};
    }

}

- (void)updateFairyData {
    PointInfo *infoData = (PointInfo *)[_lightsInfoBuffer contents];
    srandom(0x134e5348);
}

#pragma mark - MTKViewDelegate

/// 绘制“仙女”在点灯的中心与2D磁盘使用纹理执行平滑alpha混合的边缘
- (void)drawFairies:(nonnull id <MTLRenderCommandEncoder>)renderEncoder {
    [renderEncoder pushDebugGroup:@"Draw Fairies"];
    [renderEncoder setRenderPipelineState:_fairyPipelineState];
    [renderEncoder setVertexBytes:&_viewportSize length:sizeof(vector_float2) atIndex:VertexInputViewportSize];
    [renderEncoder setVertexBytes:&_uniform length:sizeof(PEUniform) atIndex:5];
    [renderEncoder setVertexBuffer:_lightsInfoBuffer offset:0 atIndex:VertexInputPoint];
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip vertexStart:0 vertexCount:AAPLNumLights];
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
