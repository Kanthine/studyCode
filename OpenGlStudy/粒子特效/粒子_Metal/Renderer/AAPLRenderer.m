@import simd;
@import ModelIO;
@import MetalKit;

#include <stdlib.h>
#import "AAPLRenderer.h"
#import "AAPLMathUtilities.h"
#import "AAPLShaderTypes.h"

static const NSUInteger AAPLNumLights = 256;
static const NSUInteger AAPLNumFairyVertices = 7; // 精灵模型中的顶点数量

/// 粒子分布范围
static const NSUInteger AAPLTreeLights   = 0                 + 0.30 * AAPLNumLights; /// 圆柱
static const NSUInteger AAPLGroundLights = AAPLTreeLights    + 0.40 * AAPLNumLights; /// 圆环
static const NSUInteger AAPLColumnLights = AAPLGroundLights  + 0.30 * AAPLNumLights; /// 分散


@interface AAPLRenderer ()

{
    id<MTLCommandQueue> _commandQueue;
    id <MTLRenderPipelineState> _fairyPipelineState;
    id<MTLTexture> _fairyMap;
    MTLRenderPassDescriptor *_finalRenderPassDescriptor;
    matrix_float4x4 _projection_matrix;
    int _frameNumber;
    id<MTLBuffer> _fairy;
}

@property (nonatomic, nonnull) id<MTLBuffer> uniformsBuffer;
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
        id <MTLFunction> fairyVertexFunction = [shaderLibrary newFunctionWithName:@"fairy_vertex"];
        id <MTLFunction> fairyFragmentFunction = [shaderLibrary newFunctionWithName:@"fairy_fragment"];

        MTLRenderPipelineDescriptor *renderPipelineDescriptor = [MTLRenderPipelineDescriptor new];

        renderPipelineDescriptor.label = @"Fairy Drawing";
        renderPipelineDescriptor.vertexDescriptor = nil;
        renderPipelineDescriptor.vertexFunction = fairyVertexFunction;
        renderPipelineDescriptor.fragmentFunction = fairyFragmentFunction;
        renderPipelineDescriptor.colorAttachments[AAPLRenderTargetLighting].pixelFormat = _view.colorPixelFormat;
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
    [self initFairyData];
}

- (void)loadAssets {
    NSError *error = nil;
    
    _uniformsBuffer = [_device newBufferWithLength:sizeof(FrameUniforms) options:0];
    
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

/// 初始化粒子的位置、速度、颜色
/// 位置：圆柱体的 {x, y, z}
- (void)initFairyData {
    PointInfo *infoData = (PointInfo *)[_lightsInfoBuffer contents];
    srandom(0x134e5348);
    for(NSUInteger lightId = 0; lightId < AAPLNumLights; lightId++) { /// 256 个粒子
        float radius = 0;
        float height = 0;
        float angle = 0;
        float speed = 0;
        
        if(lightId < AAPLTreeLights) {
            infoData -> type = PointTypeTree;
            radius = random_float(38,42);
            height = random_float(0,1);
            angle = random_float(0, M_PI*2);
            speed = random_float(0.003,0.014);
        } else if(lightId < AAPLGroundLights) {
            infoData -> type = PointTypeGround;
            radius = random_float(140,260);
            height = random_float(140,150);
            angle = random_float(0, M_PI*2);
            speed = random_float(0.006,0.027);
            speed *= (random()%2)*2-1;
        } else if(lightId < AAPLColumnLights) {
            infoData -> type = PointTypeColumn;
            radius = random_float(365,380);
            height = random_float(150,190);
            angle = random_float(0, M_PI*2);
            speed = random_float(0.004,0.014);
            speed *= (random()%2)*2-1;
        }
        
        speed *= .5;
        /// 以 Y 轴为中心线的圆柱体, XZ 切面是一个圆形
        infoData->speed = speed;
        infoData->position = (vector_float4){radius * sinf(angle), height, radius * cosf(angle), 1};
        int colorId = random()%3;
        if( colorId == 0) {
            infoData->color = (vector_float3){random_float(4,6),random_float(0,4),random_float(0,4)};
        } else if ( colorId == 1) {
            infoData->color = (vector_float3){random_float(0,4),random_float(4,6),random_float(0,4)};
        } else {
            infoData->color = (vector_float3){random_float(0,4),random_float(0,4),random_float(4,6)};
        }
        infoData++;
    }
}

- (void)updateSceneState {
    if(!_view.paused) {
        _frameNumber++;
    }
    FrameUniforms *uniforms = (FrameUniforms *)_uniformsBuffer.contents;
    uniforms -> time = _frameNumber;
    uniforms -> fairy_size = .4;
    uniforms -> projectionMatrix = _projection_matrix; /// 投影矩阵
    
    
    /// 摄像机矩阵：将摄像机旋转一定的角度
    float cameraRotationRadians = _frameNumber * 0.0025f + M_PI;
    matrix_float4x4 cameraRotationMatrix = matrix4x4_rotation(cameraRotationRadians, (vector_float3){0, 1, 0});
    matrix_float4x4 cameraMatrix = matrix_look_at_left_hand(0, 18, -50,
                                                          0, 5, 0,
                                                          0, 1, 0);
    uniforms -> cameraMatrix = matrix_multiply(cameraMatrix, cameraRotationMatrix);
    
    /// 模型矩阵( -> 世界坐标)
    matrix_float4x4 wordScaleMatrix = matrix4x4_scale(0.1, 0.1, 0.1);
    matrix_float4x4 wordTranslateMatrix = matrix4x4_translation(0, -10, 0);
    uniforms -> worldMatrix = matrix_multiply(wordTranslateMatrix, wordScaleMatrix);
}

#pragma mark - MTKViewDelegate

/// 绘制“仙女”在点灯的中心与2D磁盘使用纹理执行平滑alpha混合的边缘
- (void)drawFairies:(nonnull id <MTLRenderCommandEncoder>)renderEncoder {
    [renderEncoder pushDebugGroup:@"Draw Fairies"];
    [renderEncoder setRenderPipelineState:_fairyPipelineState];
    [renderEncoder setVertexBuffer:_fairy offset:0 atIndex:VertexInputVertex];
    [renderEncoder setVertexBuffer:_uniformsBuffer offset:0 atIndex:VertexInputUniforms];
    [renderEncoder setVertexBuffer:_lightsInfoBuffer offset:0 atIndex:VertexInputPoint];
    [renderEncoder setFragmentTexture:_fairyMap atIndex:FragmentInputTexture];
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip vertexStart:0 vertexCount:AAPLNumFairyVertices instanceCount:AAPLNumLights];
    [renderEncoder popDebugGroup];
}

- (void)drawInMTKView:(MTKView *)view  {
    [self updateSceneState];
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
    float aspect = size.width / (float)size.height;
    _projection_matrix = matrix_perspective_left_hand(65.0f * (M_PI / 180.0f), aspect, 1, 150);
    
    if(view.paused) {
        [view draw];
    }
}

@end
