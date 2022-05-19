@import simd;
@import ModelIO;
@import MetalKit;

#import "AAPLRenderer.h"
#import "AAPLMesh.h"
#import "AAPLMathUtilities.h"
#import "AAPLShaderTypes.h"

static const NSUInteger AAPLMaxBuffersInFlight = 3;

@implementation AAPLRenderer {
    CGSize _windowSize;
    NSUInteger _frameNum;
    
    dispatch_semaphore_t _inFlightSemaphore;
    id <MTLDevice> _device;
    id <MTLCommandQueue> _commandQueue;
    
    // 每一帧的缓冲区
    id <MTLBuffer> _frameUniformBuffers[AAPLMaxBuffersInFlight];
    
    /// 每个渲染管线都有一个片段着色器
    id <MTLRenderPipelineState> _pipelineStates;
    id <MTLDepthStencilState> _depthState;
    
    // Tile shader用于准备 imageblock 内存
    id <MTLRenderPipelineState> _clearTileStates;
    
    /// Tile shader 用于将 imageblock OIT 数据解析到最终的 framebuffer 中
    id <MTLRenderPipelineState> _resolveStates;

    /// 顶点描述符指定顶点将如何通过布局呈现到我们的管道，以及我们将如何布局我们的 ModelIO 顶点
    MTLVertexDescriptor *_mtlVertexDescriptor;
    
    /// 用于确定每帧 _uniformBufferStride，这是对AAPLMaxBuffersInFlight求模的当前帧号
    uint8_t _uniformBufferIndex;
    
    matrix_float4x4 _projectionMatrix;
    
    float _rotation; // 当前物体的弧度旋转
    
    NSArray<AAPLMesh *> *_meshes; // 网格对象的数组
    id <MTLBuffer> _oitBufferData; // 如果正在使用设备内存，保存 OIT 数据的缓冲区
}

-(nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)view {
    self = [super init];
    if(self) {
        _device = view.device;
        if(![_device supportsFeatureSet:MTLFeatureSet_iOS_GPUFamily4_v1]) {
            /// 本示例需要iOS_GPUFamily4_v1特性集提供的特性(在使用A11 gpu或更高版本的设备上提供)。
            assert(!"Sample requires GPUFamily4_v1 (introduced with A11)");
            return nil;
        }

        _inFlightSemaphore = dispatch_semaphore_create(AAPLMaxBuffersInFlight);

        [self loadMetalWithMetalKitView:view];
        [self loadAssets];
    }

    return self;
}

/// 创建并加载基本的Metal状态对象
- (void)loadMetalWithMetalKitView:(nonnull MTKView *)view {
    NSError *error;
    id <MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
    // Create and allocate uniform buffer objects.
    for(NSUInteger i = 0; i < AAPLMaxBuffersInFlight; i++) {
        // Indicate shared storage so that both the CPU and GPU can access the buffer
        const MTLResourceOptions storageMode = MTLResourceStorageModeShared;
        _frameUniformBuffers[i] = [_device newBufferWithLength:sizeof(AAPLFrameUniforms) options:storageMode];
        _frameUniformBuffers[i].label = [NSString stringWithFormat:@"FrameUniformBuffer%lu", i];
    }
    
    // Function constants for the functions
    MTLFunctionConstantValues *constantValues = [MTLFunctionConstantValues new];
    
    _mtlVertexDescriptor = [[MTLVertexDescriptor alloc] init];

    // Positions.
    _mtlVertexDescriptor.attributes[AAPLVertexAttributePosition].format = MTLVertexFormatFloat3;
    _mtlVertexDescriptor.attributes[AAPLVertexAttributePosition].offset = 0;
    _mtlVertexDescriptor.attributes[AAPLVertexAttributePosition].bufferIndex = AAPLBufferIndexMeshPositions;

    // Texture coordinates.
    _mtlVertexDescriptor.attributes[AAPLVertexAttributeTexcoord].format = MTLVertexFormatFloat2;
    _mtlVertexDescriptor.attributes[AAPLVertexAttributeTexcoord].offset = 0;
    _mtlVertexDescriptor.attributes[AAPLVertexAttributeTexcoord].bufferIndex = AAPLBufferIndexMeshGenerics;

    // Position Buffer Layout
    _mtlVertexDescriptor.layouts[AAPLBufferIndexMeshPositions].stride = 12;
    _mtlVertexDescriptor.layouts[AAPLBufferIndexMeshPositions].stepRate = 1;
    _mtlVertexDescriptor.layouts[AAPLBufferIndexMeshPositions].stepFunction = MTLVertexStepFunctionPerVertex;

    // Generic Attribute Buffer Layout
    _mtlVertexDescriptor.layouts[AAPLBufferIndexMeshGenerics].stride = 8;
    _mtlVertexDescriptor.layouts[AAPLBufferIndexMeshGenerics].stepRate = 1;
    _mtlVertexDescriptor.layouts[AAPLBufferIndexMeshGenerics].stepFunction = MTLVertexStepFunctionPerVertex;

    view.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
    view.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
    view.sampleCount = 1;
    
    // Create a reusable pipeline state
    MTLRenderPipelineDescriptor *pipelineStateDescriptor =
        [[MTLRenderPipelineDescriptor alloc] init];
    pipelineStateDescriptor.label = @"4 Layer OIT RenderPipeline";
    pipelineStateDescriptor.vertexDescriptor = _mtlVertexDescriptor;
    pipelineStateDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"vertexTransform"];
    pipelineStateDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"OITFragmentFunction_4Layer" constantValues:constantValues error:nil];
    pipelineStateDescriptor.sampleCount = view.sampleCount;
    pipelineStateDescriptor.depthAttachmentPixelFormat = view.depthStencilPixelFormat;
    pipelineStateDescriptor.stencilAttachmentPixelFormat = view.depthStencilPixelFormat;
    pipelineStateDescriptor.colorAttachments[0].pixelFormat = view.colorPixelFormat;
    // Create the various pipeline states
    // We will not actually write colors with our render pass when using or OIT methods
    //    Instead, our tile shaders will accomplish these writes
    pipelineStateDescriptor.colorAttachments[0].blendingEnabled = NO;
    pipelineStateDescriptor.colorAttachments[0].writeMask = MTLColorWriteMaskNone;

    _pipelineStates = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];
    if (!_pipelineStates) {
        NSLog(@"Failed to create pipeline state, error %@", error);
    }
    

    // Create the various tile states for setting up and resolving imageblock memory
    // because of the usage of tile render pipeline descriptors
    MTLTileRenderPipelineDescriptor *tileDesc = [MTLTileRenderPipelineDescriptor new];
    tileDesc.label = @"4 Layer OIT Resolve";
    tileDesc.tileFunction = [defaultLibrary newFunctionWithName:@"OITResolve_4Layer" constantValues:constantValues error:nil];
    tileDesc.colorAttachments[0].pixelFormat = view.colorPixelFormat;
    tileDesc.threadgroupSizeMatchesTileSize = YES;
    _resolveStates = [_device newRenderPipelineStateWithTileDescriptor:tileDesc
                                                                  options:0
                                                               reflection:nil
                                                                    error:&error];
    if (!_resolveStates) {
        NSLog(@"Failed to create tile pipeline state, error %@", error);
    }

    tileDesc.label = @"%lu Layer OIT Clear";
    tileDesc.tileFunction = [defaultLibrary newFunctionWithName:@"OITClear_4Layer" constantValues:constantValues error:nil];
    _clearTileStates = [_device newRenderPipelineStateWithTileDescriptor:tileDesc
                                                                    options:0
                                                                 reflection:nil
                                                                      error:&error];
    if (!_clearTileStates) {
        NSLog(@"Failed to create tile pipeline state, error %@", error);
    }

    MTLDepthStencilDescriptor *depthStateDesc = [[MTLDepthStencilDescriptor alloc] init];
    depthStateDesc.depthCompareFunction = MTLCompareFunctionLess;
    depthStateDesc.depthWriteEnabled = NO;
    _depthState = [_device newDepthStencilStateWithDescriptor:depthStateDesc];

    // Create the command queue
    _commandQueue = [_device newCommandQueue];
    
    // Set alpha blending as our starting rendering method
    _transparencyMethod = AAPLMethod4LayerOrderIndependent;

    _frameNum = 0;
}

/// Create and load our assets into Metal objects including meshes and textures
- (void)loadAssets {
	NSError *error;

    // Creata a ModelIO vertexDescriptor so that we format/layout our ModelIO mesh vertices to
    //   fit our Metal render pipeline's vertex descriptor layout
    MDLVertexDescriptor *modelIOVertexDescriptor =
        MTKModelIOVertexDescriptorFromMetal(_mtlVertexDescriptor);

    // Indicate how each Metal vertex descriptor attribute maps to each ModelIO  attribute
    modelIOVertexDescriptor.attributes[AAPLVertexAttributePosition].name = MDLVertexAttributePosition;
    modelIOVertexDescriptor.attributes[AAPLVertexAttributeTexcoord].name  = MDLVertexAttributeTextureCoordinate;

    NSURL *modelFileURL = [[NSBundle mainBundle] URLForResource:@"Meshes/Temple.obj"
                                                  withExtension:nil];

    if(!modelFileURL) {
        NSLog(@"Could not find model (%@) file in bundle", modelFileURL.absoluteString);
    }

    _meshes = [AAPLMesh newMeshesFromUrl:modelFileURL
                 modelIOVertexDescriptor:modelIOVertexDescriptor
                             metalDevice:_device
                                   error:&error];

    if(!_meshes || error) {
        NSLog(@"Could not create meshes from model file %@: %@", modelFileURL.absoluteString,
              error.localizedDescription);
    }
}

/// Update the state of our "Game" for the current frame
- (void)updateGameState
{
    // Update any game state (including updating dynamically changing Metal buffer)
    _uniformBufferIndex = (_uniformBufferIndex + 1) % AAPLMaxBuffersInFlight;

    AAPLFrameUniforms *uniforms =
        (AAPLFrameUniforms *)_frameUniformBuffers[_uniformBufferIndex].contents;

    uniforms->projectionMatrix = _projectionMatrix;
    uniforms->viewMatrix = matrix4x4_translation(0.0, 0, 1000.0);
    vector_float3 rotationAxis = {0, 1, 0};
    matrix_float4x4 modelMatrix = matrix4x4_rotation(_rotation, rotationAxis);
    matrix_float4x4 translation = matrix4x4_translation(0.0, -200, 0);
    modelMatrix = matrix_multiply(modelMatrix, translation);

    uniforms->modelViewMatrix = matrix_multiply(uniforms->viewMatrix, modelMatrix);

    uniforms->screenWidth = _windowSize.width;

    _rotation += 0.01;
}

/// Called whenever view changes orientation or layout is changed
- (void) mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    // When reshape is called, update the aspect ratio and projection matrix since the view
    //   orientation or size has changed
    _windowSize = size;
    
	float aspect = size.width / (float)size.height;
    _projectionMatrix = matrix_perspective_left_hand(65.0f * (M_PI / 180.0f), aspect, 1.0f, 5000.0);
}

- (MTLSize)optimalTileSize {
    return MTLSizeMake(32, 16, 1);
}

// Called whenever the view needs to render
- (void)drawInMTKView:(nonnull MTKView *)view {
    
    // Wait to ensure only AAPLMaxBuffersInFlight are getting proccessed by any stage in the Metal
    //   pipeline (App, Metal, Drivers, GPU, etc)
    dispatch_semaphore_wait(_inFlightSemaphore, DISPATCH_TIME_FOREVER);

    // Create a new command buffer for each renderpass to the current drawable
    id <MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";

    // Add completion hander which signal _inFlightSemaphore when Metal and the GPU has fully
    //   finished proccssing the commands we're encoding this frame.  This indicates when the
    //   dynamic buffers, that we're writing to this frame, will no longer be needed by Metal
    //   and the GPU.
    __block dispatch_semaphore_t block_sema = _inFlightSemaphore;
    [commandBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
        dispatch_semaphore_signal(block_sema);
    }];

    [self updateGameState];

    // Obtain a renderPassDescriptor generated from the view's drawable textures
    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;

    // If we've gotten a renderPassDescriptor we can render to the drawable, otherwise we'll skip
    //   any rendering this frame because we have no drawable to draw to
    if(renderPassDescriptor != nil) {
        
        MTLSize tileSize = {};

        tileSize = self.optimalTileSize;
        
        renderPassDescriptor.tileWidth = tileSize.width;
        renderPassDescriptor.tileHeight = tileSize.height;
        
        // Get the imageblock sample length from the compiled pipeline state
        renderPassDescriptor.imageblockSampleLength =
            _resolveStates.imageblockSampleLength;
        // 创建一个渲染命令编码器，这样我们就可以渲染到一些东西
        id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"Rendering";

        // 如果不使用设备内存，我们需要清除线程组数据
        [renderEncoder pushDebugGroup:@"Clear Imageblock Memory"];
        [renderEncoder setRenderPipelineState:_clearTileStates];
        [renderEncoder dispatchThreadsPerTile:tileSize];
        [renderEncoder popDebugGroup];

        // 设置渲染命令编码器状态
        [renderEncoder pushDebugGroup:@"Render Mesh"];
		[renderEncoder setCullMode:MTLCullModeBack];
        [renderEncoder setDepthStencilState:_depthState];
        [renderEncoder setRenderPipelineState:_pipelineStates];

        // 设置每帧缓冲
        [renderEncoder setVertexBuffer:_frameUniformBuffers[_uniformBufferIndex]
                                offset:0
                               atIndex:AAPLBufferIndexFrameUniforms];

        [renderEncoder setFragmentBuffer:_frameUniformBuffers[_uniformBufferIndex]
                                  offset:0
                                 atIndex:AAPLBufferIndexFrameUniforms];

        [renderEncoder setFragmentBuffer:_oitBufferData
                                  offset:0
                                 atIndex:AAPLBufferIndexOITData];


        for (__unsafe_unretained AAPLMesh *mesh in _meshes) {
            __unsafe_unretained MTKMesh *metalKitMesh = mesh.metalKitMesh;

            // 设置网格的顶点缓冲区
            for (NSUInteger bufferIndex = 0; bufferIndex < metalKitMesh.vertexBuffers.count; bufferIndex++) {
                __unsafe_unretained MTKMeshBuffer *vertexBuffer = metalKitMesh.vertexBuffers[bufferIndex];
                if((NSNull*)vertexBuffer != [NSNull null]) {
                    [renderEncoder setVertexBuffer:vertexBuffer.buffer
                                            offset:vertexBuffer.offset
                                           atIndex:bufferIndex];
                }
            }

            // 绘制网格的每个子网格
            for(__unsafe_unretained AAPLSubmesh *submesh in mesh.submeshes) {
                // 从渲染管道中设置读取/采样的纹理
                [renderEncoder setFragmentTexture:submesh.textures[AAPLTextureIndexBaseColor]
                                          atIndex:AAPLTextureIndexBaseColor];

                MTKSubmesh *metalKitSubmesh = submesh.metalKitSubmmesh;

                [renderEncoder drawIndexedPrimitives:metalKitSubmesh.primitiveType
                                          indexCount:metalKitSubmesh.indexCount
                                           indexType:metalKitSubmesh.indexType
                                         indexBuffer:metalKitSubmesh.indexBuffer.buffer
                                   indexBufferOffset:metalKitSubmesh.indexBuffer.offset];
            }
        }

        [renderEncoder popDebugGroup];

        // 解析线程组数据中的OIT数据
        [renderEncoder pushDebugGroup:@"ResolveTranparency"];
        [renderEncoder setRenderPipelineState:_resolveStates];
        [renderEncoder dispatchThreadsPerTile:tileSize];
        [renderEncoder popDebugGroup];
        
        //完成编码命令
        [renderEncoder endEncoding];
    }

    // 一旦 framebuffer 完成，就使用当前绘制对象安排一个 present
    [commandBuffer presentDrawable:view.currentDrawable];

    // 完成渲染并将命令缓冲区推入GPU
    [commandBuffer commit];

    _frameNum++;
}

@end



