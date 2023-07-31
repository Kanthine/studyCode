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

static const NSUInteger AAPLNumLights = 256;
static const NSUInteger AAPLTreeLights   = 0    + 0.30 * AAPLNumLights;
static const NSUInteger AAPLGroundLights = AAPLTreeLights    + 0.40 * AAPLNumLights;
static const NSUInteger AAPLColumnLights = AAPLGroundLights  + 0.30 * AAPLNumLights;


@interface SkyRenderer ()

{
    id<MTLDevice> _device;
    MTKView *_mtkView;
    
    id<MTLCommandQueue> _commandQueue;
    
    id<MTLRenderPipelineState> _skyboxPipelineState;
    MTKMesh *_skyMesh;
    MTLVertexDescriptor *_skyVertexDescriptor;
    id <MTLTexture> _skyMap;
    
    id<MTLRenderPipelineState> _fairyPipelineState;
    id<MTLTexture> _fairyMap;
    id<MTLBuffer> _fairyBuffer;
    id<MTLBuffer> _lightsData;
    id<MTLBuffer> _lightPosition;
    NSData *_originalLightPositions;

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
    
    #pragma mark Fairy billboard render pipeline setup
    {
        id <MTLFunction> fairyVertexFunction = [defaultLibrary newFunctionWithName:@"fairy_vertex"];
        id <MTLFunction> fairyFragmentFunction = [defaultLibrary newFunctionWithName:@"fairy_fragment"];
        
        MTLRenderPipelineDescriptor *renderPipelineDescriptor = [MTLRenderPipelineDescriptor new];
        renderPipelineDescriptor.label = @"Fairy Drawing";
        renderPipelineDescriptor.vertexDescriptor = nil;
        renderPipelineDescriptor.vertexFunction = fairyVertexFunction;
        renderPipelineDescriptor.fragmentFunction = fairyFragmentFunction;
        renderPipelineDescriptor.colorAttachments[0].pixelFormat = _mtkView.colorPixelFormat;
        renderPipelineDescriptor.depthAttachmentPixelFormat = _mtkView.depthStencilPixelFormat;
        renderPipelineDescriptor.stencilAttachmentPixelFormat = _mtkView.depthStencilPixelFormat;
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
        
        
        _fairyMap = [textureLoader newTextureWithName:@"FairyMap"
                                          scaleFactor:1.0
                                               bundle:nil
                                              options:textureLoaderOptions
                                                error:&error];
        NSAssert(_fairyMap, @"Could not load fairy texture: %@", error);
        _fairyMap.label = @"Fairy Map";
    }
    
    #pragma mark Setup buffer with attributes for each point light/fairy
    {
        _lightsData = [_device newBufferWithLength:sizeof(AAPLPointLight)*AAPLNumLights options:0];
        _lightsData.label = @"LightData";
        NSAssert(_lightsData, @"Could not create lights data buffer");
        [self initLights];
    }
}

/// Initialize light positions and colors
- (void)initLights {
    AAPLPointLight *light_data = (AAPLPointLight*)[_lightsData contents];

    NSMutableData *originalLightPositions =  [[NSMutableData alloc] initWithLength:_lightPosition.length];

    _originalLightPositions = originalLightPositions;

    vector_float4 *light_position = (vector_float4*)originalLightPositions.mutableBytes;

    srandom(0x134e5348);

    for(NSUInteger lightId = 0; lightId < AAPLNumLights; lightId++) {
        float distance = 0;
        float height = 0;
        float angle = 0;
        float speed = 0;

        if(lightId < AAPLTreeLights) {
            distance = random_float(38,42);
            height = random_float(0,1);
            angle = random_float(0, M_PI*2);
            speed = random_float(0.003,0.014);
        } else if(lightId < AAPLGroundLights) {
            distance = random_float(140,260);
            height = random_float(140,150);
            angle = random_float(0, M_PI*2);
            speed = random_float(0.006,0.027);
            speed *= (random()%2)*2-1;
        } else if(lightId < AAPLColumnLights) {
            distance = random_float(365,380);
            height = random_float(150,190);
            angle = random_float(0, M_PI*2);
            speed = random_float(0.004,0.014);
            speed *= (random()%2)*2-1;
        }

        speed *= .5;
        *light_position = (vector_float4){ distance*sinf(angle),height,distance*cosf(angle),1};
        light_data->light_radius = random_float(25,35)/10.0;
        light_data->light_speed  = speed;

        int colorId = random()%3;
        if( colorId == 0) {
            light_data->light_color = (vector_float3){random_float(4,6),random_float(0,4),random_float(0,4)};
        } else if ( colorId == 1) {
            light_data->light_color = (vector_float3){random_float(0,4),random_float(4,6),random_float(0,4)};
        } else {
            light_data->light_color = (vector_float3){random_float(0,4),random_float(0,4),random_float(4,6)};
        }

        light_data++;
        light_position++;
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
    
    [self updateLights:_uniforms.worldMatrix];
}

/// Update light positions
- (void)updateLights:(matrix_float4x4)modelViewMatrix {
    AAPLPointLight *lightData = (AAPLPointLight*)((char*)[_lightsData contents]);
    vector_float4 *currentBuffer = (vector_float4*) _lightPosition.contents;
    vector_float4 *originalLightPositions =  (vector_float4 *)_originalLightPositions.bytes;

    for(int i = 0; i < AAPLNumLights; i++) {
        vector_float4 currentPosition;

        if(i < AAPLTreeLights) {
            double lightPeriod = lightData[i].light_speed * _frameNumber;
            lightPeriod += originalLightPositions[i].y;
            lightPeriod -= floor(lightPeriod);  // Get fractional part

            // Use pow to slowly move the light outward as it reaches the branches of the tree
            float r = 1.2 + 10.0 * powf(lightPeriod, 5.0);

            currentPosition.x = originalLightPositions[i].x * r;
            currentPosition.y = 200.0f + lightPeriod * 400.0f;
            currentPosition.z = originalLightPositions[i].z * r;
            currentPosition.w = 1;
        } else {
            float rotationRadians = lightData[i].light_speed * _frameNumber;
            matrix_float4x4 rotation = matrix4x4_rotation(rotationRadians, 0, 1, 0);
            currentPosition = matrix_multiply(rotation, originalLightPositions[i]);
        }
        currentPosition = matrix_multiply(modelViewMatrix, currentPosition);
        currentBuffer[i] = currentPosition;
    }
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


/// Draw the "fairies" at the center of the point lights with a 2D disk using a texture to perform
/// smooth alpha blending on the edges
- (void)drawFairies:(nonnull id <MTLRenderCommandEncoder>)renderEncoder {
    [renderEncoder pushDebugGroup:@"Draw Fairies"];
    [renderEncoder setRenderPipelineState:_fairyPipelineState];
    [renderEncoder setCullMode:MTLCullModeBack];
    [renderEncoder setVertexBytes:&_uniforms length:sizeof(_uniforms) atIndex:SkyBufferIndexUniformData];
    [renderEncoder setVertexBuffer:_lightsData offset:0 atIndex:AAPLBufferIndexLightsData];
    [renderEncoder setVertexBuffer:_lightPosition offset:0 atIndex:AAPLBufferIndexLightsPosition];
    [renderEncoder setFragmentTexture:_fairyMap atIndex:SkyTextureIndexAlpha];
    [renderEncoder drawPrimitives:MTLPrimitiveTypePoint vertexStart:0 vertexCount:AAPLNumLights];
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
        [self drawFairies:renderEncoder];
        [renderEncoder endEncoding];
        [commandBuffer presentDrawable:view.currentDrawable];
    }
    [commandBuffer commit];
}

@end
