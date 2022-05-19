@import simd;
@import ModelIO;
@import MetalKit;

#include <stdlib.h>
#import "AAPLRenderer.h"
#import "AAPLShaderTypes.h"

static const NSUInteger kPointCount = 4;

@interface AAPLRenderer ()

{
    id<MTLCommandQueue> _commandQueue;
    id <MTLRenderPipelineState> _fairyPipelineState;
    id<MTLTexture> _fairyMap;
    MTLRenderPassDescriptor *_finalRenderPassDescriptor;
    vector_float2 _viewportSize;
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
        _lightsInfoBuffer = [_device newBufferWithLength:sizeof(vector_float2) * kPointCount options:0];
        _lightsInfoBuffer.label = @"info: speed and color";
        NSAssert(_lightsInfoBuffer, @"Could not create lights data buffer");
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
        _fairyMap = [textureLoader newTextureWithName:@"PointSprite"
                                          scaleFactor:1.0
                                               bundle:nil
                                              options:textureLoaderOptions
                                                error:&error];

        NSAssert(_fairyMap, @"Could not load fairy texture: %@", error);
        _fairyMap.label = @"Fairy Map";
    }
}

#pragma mark - 粒子状态设定

- (void)initFairData {
    vector_float2 *points = (vector_float2 *)[_lightsInfoBuffer contents];
    float size = 100;
    for(int i = 0; i < kPointCount; i++) {
        points[i] = (vector_float2){size * i, size * i};
    }
}

#pragma mark - MTKViewDelegate

- (void)drawFairies:(nonnull id <MTLRenderCommandEncoder>)renderEncoder {
    [renderEncoder pushDebugGroup:@"Draw Fairies"];
    [renderEncoder setRenderPipelineState:_fairyPipelineState];
    [renderEncoder setVertexBytes:&_viewportSize length:sizeof(vector_float2) atIndex:VertexInputViewport];
    [renderEncoder setVertexBuffer:_lightsInfoBuffer offset:0 atIndex:VertexInputPoint];
    [renderEncoder setFragmentTexture:_fairyMap atIndex:FragmentInputTexture];
    [renderEncoder drawPrimitives:MTLPrimitiveTypePoint vertexStart:0 vertexCount:kPointCount];
    [renderEncoder popDebugGroup];
}

- (void)drawInMTKView:(MTKView *)view  {
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
