//
//  SkyRenderer.m
//  HelloTriangle
//
//  Created by wyl on 2018/8/23.
//  Copyright © 2018 Apple. All rights reserved.
//

#import "SkyRenderer.h"
#include "GlobalType.h"
#import "AAPLMathUtilities.h"

@interface SkyRenderer ()

{
    id<MTLDevice> _device;
    MTKView *_mtkView;
    
    id<MTLCommandQueue> _commandQueue;
    
    id<MTLRenderPipelineState> _skyboxPipelineState;
    MTKMesh *_skyMesh;
    MTLVertexDescriptor *_skyVertexDescriptor;
    id <MTLTexture> _skyMap;
    
    Uniforms _uniforms;
    NSUInteger _frameNumber;
}

@end


@implementation SkyRenderer

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView {
    self = [super init];
    if(self) {
        _mtkView = mtkView;
        _device = mtkView.device;
        _commandQueue = [_device newCommandQueue];
        [self loadMetalAsset];
    }
    return self;
}

- (void)loadMetalAsset {
    id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
    NSError* error;
    #pragma mark Sky render pipeline setup
    {
        _skyVertexDescriptor = [MTLVertexDescriptor new];
        _skyVertexDescriptor.attributes[SkyVertexAttributePosition].format = MTLVertexFormatFloat3;
        _skyVertexDescriptor.attributes[SkyVertexAttributePosition].offset = 0;
        _skyVertexDescriptor.attributes[SkyVertexAttributePosition].bufferIndex = SkyBufferIndexMeshPositions;
        _skyVertexDescriptor.layouts[SkyBufferIndexMeshPositions].stride = 12;
        _skyVertexDescriptor.attributes[SkyVertexAttributeNormal].format = MTLVertexFormatFloat3;
        _skyVertexDescriptor.attributes[SkyVertexAttributeNormal].offset = 0;
        _skyVertexDescriptor.attributes[SkyVertexAttributeNormal].bufferIndex = SkyBufferIndexMeshGenerics;
        _skyVertexDescriptor.layouts[SkyBufferIndexMeshGenerics].stride = 12;

        id <MTLFunction> skyboxVertexFunction = [defaultLibrary newFunctionWithName:@"skybox_vertex"];
        id <MTLFunction> skyboxFragmentFunction = [defaultLibrary newFunctionWithName:@"skybox_fragment"];

        MTLRenderPipelineDescriptor *renderPipelineDescriptor = [MTLRenderPipelineDescriptor new];
        renderPipelineDescriptor.label = @"Sky";
        renderPipelineDescriptor.vertexDescriptor = _skyVertexDescriptor;
        renderPipelineDescriptor.vertexFunction = skyboxVertexFunction;
        renderPipelineDescriptor.fragmentFunction = skyboxFragmentFunction;
        renderPipelineDescriptor.colorAttachments[0].pixelFormat = _mtkView.colorPixelFormat;
        renderPipelineDescriptor.depthAttachmentPixelFormat = _mtkView.depthStencilPixelFormat;
        renderPipelineDescriptor.stencilAttachmentPixelFormat = _mtkView.depthStencilPixelFormat;

        _skyboxPipelineState = [_device newRenderPipelineStateWithDescriptor:renderPipelineDescriptor
                                                                      error:&error];

        NSAssert(_skyboxPipelineState, @"Failed to create skybox render pipeline state: %@", error);
    }
    
    #pragma mark Setup sphere mesh for skybox
    {
        MTKMeshBufferAllocator *bufferAllocator = [[MTKMeshBufferAllocator alloc] initWithDevice:_device];
        
        /// 天空穹
        MDLMesh *sphereMDLMesh = [MDLMesh newEllipsoidWithRadii:150
                                                 radialSegments:20
                                               verticalSegments:20
                                                   geometryType:MDLGeometryTypeTriangles
                                                  inwardNormals:NO
                                                     hemisphere:NO
                                                      allocator:bufferAllocator];
        
        MDLVertexDescriptor *sphereDescriptor = MTKModelIOVertexDescriptorFromMetal(_skyVertexDescriptor);
        sphereDescriptor.attributes[SkyVertexAttributePosition].name = MDLVertexAttributePosition;
        sphereDescriptor.attributes[SkyVertexAttributeNormal].name   = MDLVertexAttributeNormal;

        // Set the vertex descriptor to relayout vertices
        sphereMDLMesh.vertexDescriptor = sphereDescriptor;

        _skyMesh = [[MTKMesh alloc] initWithMesh:sphereMDLMesh device:_device error:&error];
        NSAssert(_skyMesh, @"Could not create mesh: %@", error);
    }

    #pragma mark Load textures for non-mesh assets
    {
        MTKTextureLoader *textureLoader = [[MTKTextureLoader alloc] initWithDevice:_device];
        NSDictionary *textureLoaderOptions = @{
            MTKTextureLoaderOptionTextureUsage       : @(MTLTextureUsageShaderRead),
            MTKTextureLoaderOptionTextureStorageMode : @(MTLStorageModePrivate),
        };
        _skyMap = [textureLoader newTextureWithName:@"SkyMap"
                                        scaleFactor:1.0
                                             bundle:nil
                                            options:textureLoaderOptions
                                              error:&error];
        NSAssert(_skyMap, @"Could not load sky texture: %@", error);
        _skyMap.label = @"Sky Map";
    }
}

- (void)updateSceneState {
    if(!_mtkView.paused) {
        _frameNumber++;
    }
        
    _uniforms.cameraMatrix = (matrix_float4x4){
        {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }
    };
    
    float cameraRotationRadians = _frameNumber * 0.0025f + M_PI;
    vector_float3 cameraRotationAxis = {0, 1, 0};
    matrix_float4x4 cameraRotationMatrix = matrix4x4_rotation(cameraRotationRadians, cameraRotationAxis);
    
    float skyRotation = _frameNumber * 0.005f - (M_PI_4 * 3);
    vector_float3 skyRotationAxis = {0, 1, 0};
    matrix_float4x4 skyModelMatrix = matrix4x4_rotation(skyRotation, skyRotationAxis);
    _uniforms.worldMatrix = matrix_multiply(cameraRotationMatrix, skyModelMatrix);
}

#pragma mark - Draw

- (void)drawSky:(nonnull id <MTLRenderCommandEncoder>)renderEncoder {
    
    [renderEncoder pushDebugGroup:@"Draw Sky"];
    [renderEncoder setRenderPipelineState:_skyboxPipelineState];
    [renderEncoder setCullMode:MTLCullModeFront];
    [renderEncoder setVertexBytes:&_uniforms length:sizeof(_uniforms) atIndex:SkyBufferIndexUniformData];
    [renderEncoder setFragmentTexture:_skyMap atIndex:SkyTextureIndexBaseColor];

    for (NSUInteger bufferIndex = 0; bufferIndex < _skyMesh.vertexBuffers.count; bufferIndex++) {
        __unsafe_unretained MTKMeshBuffer *vertexBuffer = _skyMesh.vertexBuffers[bufferIndex];
        if((NSNull*)vertexBuffer != [NSNull null]) {
            [renderEncoder setVertexBuffer:vertexBuffer.buffer offset:vertexBuffer.offset atIndex:bufferIndex];
        }
    }
    
    MTKSubmesh *sphereSubmesh = _skyMesh.submeshes[0];
    [renderEncoder drawIndexedPrimitives:sphereSubmesh.primitiveType
                              indexCount:sphereSubmesh.indexCount
                               indexType:sphereSubmesh.indexType
                             indexBuffer:sphereSubmesh.indexBuffer.buffer
                       indexBufferOffset:sphereSubmesh.indexBuffer.offset];
    [renderEncoder popDebugGroup];
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    float aspect = size.width / (float)size.height;
    _uniforms.projectionMatrix = matrix_perspective_left_hand(65.0f * (M_PI / 180.0f), aspect, 1, 150);
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    [self updateSceneState];
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";
    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;

    if(renderPassDescriptor != nil) {
        id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"MyRenderEncoder";
        [self drawSky:renderEncoder];
        [renderEncoder endEncoding];
        [commandBuffer presentDrawable:view.currentDrawable];
    }
    [commandBuffer commit];
}

@end
