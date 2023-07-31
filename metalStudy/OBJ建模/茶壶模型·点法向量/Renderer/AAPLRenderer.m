@import simd;
@import MetalKit;

#import "AAPLRenderer.h"
#import "AAPLShaderTypes.h"
#import "TeapotMesh.h"
#include "AAPLMathUtilities.h"

@interface AAPLRenderer ()
{
    MTKView *_mtkView;
    id<MTLDevice> _device;
    id<MTLRenderPipelineState> _renderPipeline;
    id<MTLCommandQueue> _commandQueue;
    id<MTLDepthStencilState> _depthState;
    MTLVertexDescriptor *_vertexDescriptor;
    TeapotMesh *_mesh;

    vector_float2 _viewportSize;
    Uniforms _uniforms;
    float _rotation;
}
@end

@implementation AAPLRenderer

- (nonnull instancetype)initWithMTKView:(nonnull MTKView *)mtkView {
    self = [super init];
    if(self) {
        mtkView.colorPixelFormat          = MTLPixelFormatBGRA8Unorm_sRGB;
        mtkView.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
        _mtkView = mtkView;
        _device = mtkView.device;
        NSAssert(_device, @"获取设备失败");
        
        [self loadAssets];
    }
    return self;
}

- (void)loadAssets {
    // Create and load assets into Metal objects including meshes and textures
    NSError *error = nil;
    
    #pragma mark Mesh vertex descriptor setup
    {

        _vertexDescriptor = [MTLVertexDescriptor new];

        // Positions.
        _vertexDescriptor.attributes[AAPLVertexAttributePosition].format = MTLVertexFormatFloat3;
        _vertexDescriptor.attributes[AAPLVertexAttributePosition].offset = 0;
        _vertexDescriptor.attributes[AAPLVertexAttributePosition].bufferIndex = AAPLBufferIndexMeshPositions;

        // Texture coordinates.
        _vertexDescriptor.attributes[AAPLVertexAttributeTexcoord].format = MTLVertexFormatFloat2;
        _vertexDescriptor.attributes[AAPLVertexAttributeTexcoord].offset = 0;
        _vertexDescriptor.attributes[AAPLVertexAttributeTexcoord].bufferIndex = AAPLBufferIndexMeshGenerics;

        // Normals.
        _vertexDescriptor.attributes[AAPLVertexAttributeNormal].format = MTLVertexFormatHalf4;
        _vertexDescriptor.attributes[AAPLVertexAttributeNormal].offset = 8;
        _vertexDescriptor.attributes[AAPLVertexAttributeNormal].bufferIndex = AAPLBufferIndexMeshGenerics;

        // Tangents
        _vertexDescriptor.attributes[AAPLVertexAttributeTangent].format = MTLVertexFormatHalf4;
        _vertexDescriptor.attributes[AAPLVertexAttributeTangent].offset = 16;
        _vertexDescriptor.attributes[AAPLVertexAttributeTangent].bufferIndex = AAPLBufferIndexMeshGenerics;

        // Bitangents
        _vertexDescriptor.attributes[AAPLVertexAttributeBitangent].format = MTLVertexFormatHalf4;
        _vertexDescriptor.attributes[AAPLVertexAttributeBitangent].offset = 24;
        _vertexDescriptor.attributes[AAPLVertexAttributeBitangent].bufferIndex = AAPLBufferIndexMeshGenerics;

        // Position Buffer Layout
        _vertexDescriptor.layouts[AAPLBufferIndexMeshPositions].stride = 12;
        _vertexDescriptor.layouts[AAPLBufferIndexMeshPositions].stepRate = 1;
        _vertexDescriptor.layouts[AAPLBufferIndexMeshPositions].stepFunction = MTLVertexStepFunctionPerVertex;

        // Generic Attribute Buffer Layout
        _vertexDescriptor.layouts[AAPLBufferIndexMeshGenerics].stride = 32;
        _vertexDescriptor.layouts[AAPLBufferIndexMeshGenerics].stepRate = 1;
        _vertexDescriptor.layouts[AAPLBufferIndexMeshGenerics].stepFunction = MTLVertexStepFunctionPerVertex;
    }
    
    {
        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
        MTLRenderPipelineDescriptor *descriptor = [[MTLRenderPipelineDescriptor alloc] init];
        descriptor.label = @"OBJ模型·渲染";
        descriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"vertexRender"];
        descriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentRender"];
        descriptor.colorAttachments[0].pixelFormat = _mtkView.colorPixelFormat;
        descriptor.vertexDescriptor = _vertexDescriptor;
        descriptor.depthAttachmentPixelFormat = _mtkView.depthStencilPixelFormat;
        descriptor.stencilAttachmentPixelFormat = _mtkView.depthStencilPixelFormat;
        
        _renderPipeline = [_device newRenderPipelineStateWithDescriptor:descriptor error:&error];
        NSAssert(_renderPipeline, @"渲染管道创建失败 : %@",error);
    }
    
    {
        MTLDepthStencilDescriptor *depthStateDesc = [[MTLDepthStencilDescriptor alloc] init];
        depthStateDesc.depthCompareFunction = MTLCompareFunctionLess;
        depthStateDesc.depthWriteEnabled    = YES;
        _depthState = [_device newDepthStencilStateWithDescriptor:depthStateDesc];
    }
    
    #pragma mark Load meshes from model file
    {
        // Create a ModelIO vertexDescriptor so that the format/layout of the ModelIO mesh vertices
        //   cah be made to match Metal render pipeline's vertex descriptor layout
        MDLVertexDescriptor *modelIOVertexDescriptor = MTKModelIOVertexDescriptorFromMetal(_vertexDescriptor);

        // Indicate how each Metal vertex descriptor attribute maps to each ModelIO attribute
        modelIOVertexDescriptor.attributes[AAPLVertexAttributePosition].name  = MDLVertexAttributePosition;
        modelIOVertexDescriptor.attributes[AAPLVertexAttributeTexcoord].name  = MDLVertexAttributeTextureCoordinate;
        modelIOVertexDescriptor.attributes[AAPLVertexAttributeNormal].name    = MDLVertexAttributeNormal;
        modelIOVertexDescriptor.attributes[AAPLVertexAttributeTangent].name   = MDLVertexAttributeTangent;
        modelIOVertexDescriptor.attributes[AAPLVertexAttributeBitangent].name = MDLVertexAttributeBitangent;

        NSURL *modelFileURL = [[NSBundle mainBundle] URLForResource:@"Teapot1" withExtension:@"obj"];
//        modelFileURL = [[NSBundle mainBundle] URLForResource:@"Teapot1" withExtension:@"obj"];

        NSAssert(modelFileURL, @"Could not find model (%@) file in bundle", modelFileURL.absoluteString);

        _mesh = [TeapotMesh newMeshesFromURL:modelFileURL
                     modelIOVertexDescriptor:modelIOVertexDescriptor
                                 metalDevice:_device
                                       error:&error].firstObject;
        NSAssert(_mesh, @"Could not create meshes from model file %@: %@", modelFileURL.absoluteString, error);
    }
    
    _commandQueue = [_device newCommandQueue];
}


- (void)drawMeshes:(nonnull id<MTLRenderCommandEncoder>)renderEncoder {
    __unsafe_unretained MTKMesh *metalKitMesh = _mesh.metalKitMesh;

    // Set mesh's vertex buffers
    for (NSUInteger bufferIndex = 0; bufferIndex < metalKitMesh.vertexBuffers.count; bufferIndex++) {
        __unsafe_unretained MTKMeshBuffer *vertexBuffer = metalKitMesh.vertexBuffers[bufferIndex];
        if((NSNull*)vertexBuffer != [NSNull null]) {
            [renderEncoder setVertexBuffer:vertexBuffer.buffer
                                    offset:vertexBuffer.offset
                                   atIndex:bufferIndex];
        }
    }
    
    for(__unsafe_unretained MTKSubmesh *metalKitSubmesh in metalKitMesh.submeshes) {
        [renderEncoder drawIndexedPrimitives:metalKitSubmesh.primitiveType
                                  indexCount:metalKitSubmesh.indexCount
                                   indexType:metalKitSubmesh.indexType
                                 indexBuffer:metalKitSubmesh.indexBuffer.buffer
                           indexBufferOffset:metalKitSubmesh.indexBuffer.offset];
    }
}

#pragma mark - MTKViewDelegate

- (void)drawInMTKView:(nonnull MTKView *)view {
    MTLRenderPassDescriptor *descritor = view.currentRenderPassDescriptor;
    if (descritor == nil) return;
    
    [self updateGameState];
    
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"命令缓冲区";
    
    id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:descritor];
    renderEncoder.label = @"命令编码器";
    [renderEncoder setRenderPipelineState:_renderPipeline];
    [renderEncoder setVertexBytes:&_uniforms length:sizeof(_uniforms) atIndex:AAPLVertexInputIndexUniforms];
    [renderEncoder setDepthStencilState:_depthState];
    [renderEncoder setStencilReferenceValue:1];
    [self drawMeshes:renderEncoder];
    [renderEncoder endEncoding];
    [commandBuffer presentDrawable:view.currentDrawable];
    [commandBuffer commit];
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
    
    _uniforms.cameraMatrix = matrix_look_at_left_hand(simd_make_float3(0, 0, 100.0), simd_make_float3(0, 0, 0.0), simd_make_float3(0, 1, 0.0));
    _uniforms.projectionMatrix = matrix_perspective_left_hand(0.32 * M_PI, _viewportSize.x / _viewportSize.y, 1.0, 500.0f);
}

#pragma mark - private method

- (void)updateGameState {
    _uniforms.worldMatrix = matrix4x4_rotation(_rotation, 1, 1, 0);
    _rotation += 0.005f;
    
    _uniforms.isDirectionLight = NO;
    _uniforms.ambient = simd_make_float3(0.1, 0.1, 0.1);
    _uniforms.diffuse = simd_make_float3(0.7, 0.7, 0.7);
    _uniforms.specular = simd_make_float3(0.3, 0.3, 0.3);
    
    _uniforms.lightLocation = simd_make_float3(0, 100, 100);
    _uniforms.lightDirection = simd_make_float3(0, 1, -2);
}

@end
