@import simd;
@import ModelIO;
@import MetalKit;

#include <stdlib.h>
#import "AAPLRenderer.h"
#import "AAPLShaderTypes.h"

@interface AAPLRenderer ()

{
    id<MTLCommandQueue> _commandQueue;
    id <MTLRenderPipelineState> _pointPipelineState;
    id<MTLTexture> _pointTexture;
    MTLRenderPassDescriptor *_finalRenderPassDescriptor;
    
    MTKMesh *_pointMesh;
    MTLVertexDescriptor *_pointVertexDescriptor;
    
    vector_float2 _viewportSize;
    int64_t _frameTime;
}

@end

@implementation AAPLRenderer

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)view {
    self = [super init];
    if(self) {
        view.delegate = self;
        _device = view.device;
        _view = view;
        [self loadAssets];
        [self loadMetal];
    }
    return self;
}

- (void)loadMetal {
    NSError* error;
    id <MTLLibrary> shaderLibrary = [_device newDefaultLibrary];
    NSAssert(shaderLibrary, @"Failed to load Metal shader library");
    _view.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
    _frameTime = 0;
    #pragma mark Fairy billboard render pipeline setup
    {
        id <MTLFunction> fairyVertexFunction = [shaderLibrary newFunctionWithName:@"vertexShaderFP"];
        id <MTLFunction> fairyFragmentFunction = [shaderLibrary newFunctionWithName:@"fragmentShaderFP"];
        
        MTLRenderPipelineDescriptor *renderPipelineDescriptor = [MTLRenderPipelineDescriptor new];
        renderPipelineDescriptor.vertexDescriptor = _pointVertexDescriptor;
        renderPipelineDescriptor.label = @"Fairy Drawing";
        renderPipelineDescriptor.vertexFunction = fairyVertexFunction;
        renderPipelineDescriptor.fragmentFunction = fairyFragmentFunction;
        renderPipelineDescriptor.colorAttachments[0].pixelFormat = _view.colorPixelFormat;

        _pointPipelineState = [_device newRenderPipelineStateWithDescriptor:renderPipelineDescriptor
                                                                      error:&error];
        NSAssert(_pointPipelineState, @"Failed to create fairy render pipeline state: %@", error);
    }
    
    // Create the command queue
    _commandQueue = [_device newCommandQueue];    
    _finalRenderPassDescriptor = [MTLRenderPassDescriptor new];
    _finalRenderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 0.0);
    _finalRenderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    _finalRenderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
}

- (void)loadAssets {
    #pragma mark Setup sphere mesh for skybox
    {
        _pointVertexDescriptor = [MTLVertexDescriptor new];
        _pointVertexDescriptor.attributes[VertexInputVertex].format = MTLVertexFormatFloat3;
        _pointVertexDescriptor.attributes[VertexInputVertex].offset = 0;
        _pointVertexDescriptor.attributes[VertexInputVertex].bufferIndex = VertexInputVertex;
        _pointVertexDescriptor.layouts[VertexInputVertex].stride = 12;
        
        MTKMeshBufferAllocator *bufferAllocator = [[MTKMeshBufferAllocator alloc] initWithDevice:_device];
        MDLMesh *sphereMDLMesh = [MDLMesh newBoxWithDimensions:(vector_float3){KPointSize,KPointSize,0}
                                                      segments:(vector_uint3){KPointSize / 2, KPointSize / 2,0}
                                                  geometryType:MDLGeometryTypeLines
                                                 inwardNormals:NO
                                                     allocator:bufferAllocator];
        NSError *error = nil;
        MDLVertexDescriptor *sphereDescriptor = MTKModelIOVertexDescriptorFromMetal(_pointVertexDescriptor);
        sphereDescriptor.attributes[VertexInputVertex].name = MDLVertexAttributePosition;
        sphereMDLMesh.vertexDescriptor = sphereDescriptor;

        _pointMesh = [[MTKMesh alloc] initWithMesh:sphereMDLMesh
                                             device:_device
                                              error:&error];
        NSAssert(_pointMesh, @"Could not create mesh: %@", error);
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
        _pointTexture = [textureLoader newTextureWithName:@"FairyMap"
                                          scaleFactor:1.0
                                               bundle:nil
                                              options:textureLoaderOptions
                                                error:&error];

        NSAssert(_pointTexture, @"Could not load fairy texture: %@", error);

        _pointTexture.label = @"Fairy Map";
    }
}

#pragma mark - MTKViewDelegate

- (void)drawFairies:(nonnull id <MTLRenderCommandEncoder>)renderEncoder {
    
    if (_frameTime > 256) {
        _frameTime = 0;
    }
    
    _frameTime += 1;

    [renderEncoder pushDebugGroup:@"Draw Fairies"];
    [renderEncoder setRenderPipelineState:_pointPipelineState];
    [renderEncoder setVertexBytes:&_viewportSize length:sizeof(vector_float2) atIndex:VertexInputViewportSize];
    [renderEncoder setVertexBytes:&_frameTime length:sizeof(int64_t) atIndex:VertexInputFrametime];
    [renderEncoder setFragmentTexture:_pointTexture atIndex:FragmentInputTexture];
    for (NSUInteger bufferIndex = 0; bufferIndex < _pointMesh.vertexBuffers.count; bufferIndex++) {
        __unsafe_unretained MTKMeshBuffer *vertexBuffer = _pointMesh.vertexBuffers[bufferIndex];
        if((NSNull*)vertexBuffer != [NSNull null]) {
            [renderEncoder setVertexBuffer:vertexBuffer.buffer
                                    offset:vertexBuffer.offset
                                   atIndex:bufferIndex];
        }
    }
    MTKSubmesh *sphereSubmesh = _pointMesh.submeshes[0];
    [renderEncoder drawIndexedPrimitives:MTLPrimitiveTypePoint
                              indexCount:sphereSubmesh.indexCount
                               indexType:sphereSubmesh.indexType
                             indexBuffer:sphereSubmesh.indexBuffer.buffer
                       indexBufferOffset:sphereSubmesh.indexBuffer.offset];
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
