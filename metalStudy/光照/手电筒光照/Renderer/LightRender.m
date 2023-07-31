//
//  LightRender.m
//  HelloTriangle
//
//  Created by wyl on 2018/8/23.
//  Copyright © 2018 Apple. All rights reserved.
//

#import "LightRender.h"
#include "GlobalType.h"
#import "AAPLMathUtilities.h"

@interface LightRender ()

{
    id<MTLDevice> _device;
    MTKView *_mtkView;
    id<MTLCommandQueue> _commandQueue;
    
    id<MTLRenderPipelineState> _pipelineState;
    MTKMesh *_modelMesh;
    MTLVertexDescriptor *_vertexDescriptor;
    
    Uniforms _uniforms;
    float _rotation;
}

@end

@implementation LightRender

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView
                                   lightType:(kLightType)lightType {
    self = [super init];
    if(self) {
        _mtkView = mtkView;
        _device = mtkView.device;
        _commandQueue = [_device newCommandQueue];
        _rotation = 0;
        [self loadMetalAsset];
        [self setLightType:lightType];
    }
    return self;
}

- (void)loadMetalAsset {
    NSError* error;
    #pragma mark Sky render pipeline setup
    {
        _vertexDescriptor = [MTLVertexDescriptor new];
        _vertexDescriptor.attributes[kVertexAttributePosition].format = MTLVertexFormatFloat3;
        _vertexDescriptor.attributes[kVertexAttributePosition].offset = 0;
        _vertexDescriptor.attributes[kVertexAttributePosition].bufferIndex = kAttributeVertexs;
        _vertexDescriptor.layouts[kAttributeVertexs].stride = 12;
        _vertexDescriptor.attributes[kVertexAttributeNormal].format = MTLVertexFormatFloat3;
        _vertexDescriptor.attributes[kVertexAttributeNormal].offset = 0;
        _vertexDescriptor.attributes[kVertexAttributeNormal].bufferIndex = kAttributeNormal;
        _vertexDescriptor.layouts[kAttributeNormal].stride = 12;
    }
    
    #pragma mark Setup sphere mesh for skybox
    {
        MTKMeshBufferAllocator *bufferAllocator = [[MTKMeshBufferAllocator alloc] initWithDevice:_device];
//        MDLMesh *sphereMDLMesh = [MDLMesh newEllipsoidWithRadii:1
//                                                 radialSegments:100
//                                               verticalSegments:50
//                                                   geometryType:MDLGeometryTypeTriangles
//                                                  inwardNormals:NO /// 法线向内
//                                                     hemisphere:NO /// 半球
//                                                      allocator:bufferAllocator];
        MDLMesh *sphereMDLMesh = [MDLMesh newBoxWithDimensions:simd_make_float3(1, 1, 1) segments:1 geometryType:MDLGeometryTypeTriangles inwardNormals:NO allocator:bufferAllocator];
        
        MDLVertexDescriptor *sphereDescriptor = MTKModelIOVertexDescriptorFromMetal(_vertexDescriptor);
        sphereDescriptor.attributes[kVertexAttributePosition].name = MDLVertexAttributePosition;
        sphereDescriptor.attributes[kVertexAttributeNormal].name   = MDLVertexAttributeNormal;

        // Set the vertex descriptor to relayout vertices
        sphereMDLMesh.vertexDescriptor = sphereDescriptor;

        _modelMesh = [[MTKMesh alloc] initWithMesh:sphereMDLMesh device:_device error:&error];
        NSAssert(_modelMesh, @"Could not create mesh: %@", error);
    }
}

- (void)updateSceneState {
    if(!_mtkView.paused) {
        _rotation += 0.01;
    }
    
    _uniforms.isDirectionLight = NO;
    _uniforms.ambient = simd_make_float3(0.1, 0.1, 0.1);
    _uniforms.diffuse = simd_make_float3(0.7, 0.7, 0.7);
    _uniforms.specular = simd_make_float3(0.3, 0.3, 0.3);
    
    _uniforms.lightLocation = simd_make_float3(0.0, -2.0, -2.0);
    _uniforms.lightDirection = simd_make_float3(0.0, 2.0, 0.0);
    _uniforms.worldMatrix = matrix4x4_rotation(_rotation, 1, 1, 0);
}

#pragma mark - Draw

- (void)drawSky:(nonnull id <MTLRenderCommandEncoder>)renderEncoder {
    
    [renderEncoder pushDebugGroup:@"Draw light"];
    [renderEncoder setRenderPipelineState:_pipelineState];
    [renderEncoder setCullMode:MTLCullModeBack];
    [renderEncoder setVertexBytes:&_uniforms length:sizeof(_uniforms) atIndex:kAttributeUniforms];
    [renderEncoder setFragmentBytes:&_uniforms length:sizeof(_uniforms) atIndex:kAttributeUniforms];
    
    for (NSUInteger bufferIndex = 0; bufferIndex < _modelMesh.vertexBuffers.count; bufferIndex++) {
        __unsafe_unretained MTKMeshBuffer *vertexBuffer = _modelMesh.vertexBuffers[bufferIndex];
        if((NSNull*)vertexBuffer != [NSNull null]) {
            [renderEncoder setVertexBuffer:vertexBuffer.buffer offset:vertexBuffer.offset atIndex:bufferIndex];
        }
    }
    
    MTKSubmesh *sphereSubmesh = _modelMesh.submeshes[0];
    [renderEncoder drawIndexedPrimitives:sphereSubmesh.primitiveType
                              indexCount:sphereSubmesh.indexCount
                               indexType:sphereSubmesh.indexType
                             indexBuffer:sphereSubmesh.indexBuffer.buffer
                       indexBufferOffset:sphereSubmesh.indexBuffer.offset];
    [renderEncoder popDebugGroup];
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    float aspect = size.width / (float)size.height;
    _uniforms.projectionMatrix = matrix_perspective_left_hand(65.0f * (M_PI / 180.0f), aspect, 0.1, 100);
    _uniforms.cameraPos = simd_make_float3(0, 0, -4.0);
    _uniforms.cameraMatrix = matrix_look_at_left_hand(_uniforms.cameraPos,
                                                      simd_make_float3(0, 0, 0),
                                                      simd_make_float3(0, 1, 0));
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

#pragma mark - setters

- (void)setLightType:(kLightType)lightType {
    _lightType = lightType;
    
    id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
    MTLRenderPipelineDescriptor *renderPipelineDescriptor = [MTLRenderPipelineDescriptor new];
    renderPipelineDescriptor.label = @"light";
    renderPipelineDescriptor.vertexDescriptor = _vertexDescriptor;
    renderPipelineDescriptor.colorAttachments[0].pixelFormat = _mtkView.colorPixelFormat;
    renderPipelineDescriptor.depthAttachmentPixelFormat = _mtkView.depthStencilPixelFormat;
    renderPipelineDescriptor.stencilAttachmentPixelFormat = _mtkView.depthStencilPixelFormat;
    renderPipelineDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"vertexRender_Compound"];
    renderPipelineDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentShader_Compound"];
    
    NSError* error;
    _pipelineState = [_device newRenderPipelineStateWithDescriptor:renderPipelineDescriptor
                                                                  error:&error];
    NSAssert(_pipelineState, @"Failed to create light render pipeline state: %@", error);
}

@end
