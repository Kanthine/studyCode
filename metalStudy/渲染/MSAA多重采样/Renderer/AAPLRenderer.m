#import "AAPLRenderer.h"
#import "AAPLShaderTypes.h"
#import "AAPLConfig.h"
#import "DMMaskManager.h"

#pragma mark - Renderer Implementation

/// A class responsible for updating and rendering the view.
@implementation AAPLRenderer
{
    id<MTLDevice> _device;
    id<MTLCommandQueue> _commandQueue;
    
    MTLPixelFormat _renderTargetPixelFormat;
    MTLPixelFormat _drawablePixelFormat;
    MTLTextureDescriptor* _multisampleTextureDescriptor;
    id<MTLTexture> _multisampleTexture;
    MTLRenderPipelineDescriptor* _renderPipelineDescriptor;
    id<MTLRenderPipelineState> _renderPipelineState;
    id<MTLFunction> _fragmentFunctionNonHDR;
    id<MTLFunction> _fragmentFunctionHDR;
    BOOL _usesHDR;
    
    MTLTextureDescriptor* _resolveTextureDescriptor;
    id<MTLTexture> _resolveResultTexture;
    id<MTLRenderPipelineState> _compositionPipelineState;
    
    // A custom MSAA resolve in immediate-mode rendering (IMR) mode with a compute pass.
    id<MTLFunction> _averageResolveIMRKernelFunction;
    id<MTLFunction> _hdrResolveIMRKernelFunction;
    id<MTLComputePipelineState> _resolveComputePipelineState;
    MTLSize _intrinsicThreadgroupSize;
    MTLSize _threadgroupsInGrid;
    
    vector_uint2 _viewportSize;
    NSUInteger _frameNum;
    float _backgroundBrightness;
    
    id<MTLTexture> _maskTexture;
}

/// Checks whether the device supports tile shaders that were introduced with Apple GPU family 4.
- (BOOL)supportsTileShaders {
    return [_device supportsFamily:MTLGPUFamilyApple4];
}

#pragma mark - Initialization

- (nonnull instancetype)initWithMetalDevice:(nonnull id<MTLDevice>)device
                        drawablePixelFormat:(MTLPixelFormat)drawablePixelFormat {
    if (self = [super init]) {
        _animated = YES;
        
        _frameNum = 0;
        _backgroundBrightness = 0.0;
        _renderingQuality = 1.0;
        _device = device;
        _commandQueue = [_device newCommandQueue];

        // 使用 RGBA16Float 保存 subpixel samples 中的HDR值，直到它们被解析
        // RGBA16Float 使用的内存是8位格式的两倍
        _renderTargetPixelFormat = MTLPixelFormatRGBA16Float;

        // The drawable pixel format is either an 8-bit or 16-bit pixel format.
        _drawablePixelFormat = drawablePixelFormat;
        
        _resolvingOnTileShaders = NO;
        _antialiasingEnabled = YES;
        _antialiasingOptionsChanged = NO;
        
        /// 所有设备支持的最大采样数
        _antialiasingSampleCount = 4;
        
        _resolveOption = AAPLResolveOptionBuiltin;
        
        id<MTLLibrary> shaderLib = [_device newDefaultLibrary];
        
        NSAssert(shaderLib, @"Couldn't create the default shader library.");
        
        [self createRenderPipelineState:shaderLib];
        [self createResolveKernelPrograms:shaderLib];
        [self createResolvePipelineState:shaderLib];
        [self createMultisampleTextureDescriptor];
        [self createMaskTexture];

    }
    return self;
}

- (void)createRenderPipelineState:(id<MTLLibrary>)metalLibrary {
    id<MTLFunction> vertexFunction = [metalLibrary newFunctionWithName:@"vertexShader"];
    NSAssert(vertexFunction, @"Couldn't load vertex function from default library.");
    
    _fragmentFunctionNonHDR = [metalLibrary newFunctionWithName:@"fragmentShader"];
    NSAssert(_fragmentFunctionNonHDR, @"Couldn't load fragment function from default library.");
    
    _fragmentFunctionHDR = [metalLibrary newFunctionWithName:@"fragmentShaderHDR"];
    NSAssert(_fragmentFunctionHDR, @"Couldn't load fragment function from default library.");

    _renderPipelineDescriptor = [MTLRenderPipelineDescriptor new];
    
    _renderPipelineDescriptor.label                           = @"RenderPipeline";
    _renderPipelineDescriptor.vertexFunction                  = vertexFunction;
    _renderPipelineDescriptor.fragmentFunction                = _fragmentFunctionNonHDR;
    _renderPipelineDescriptor.colorAttachments[0].pixelFormat = _renderTargetPixelFormat;
    if (@available(macOS 13.0, iOS 16.0, *)) {
        _renderPipelineDescriptor.rasterSampleCount = _antialiasingSampleCount;
    } else {
        _renderPipelineDescriptor.sampleCount = _antialiasingSampleCount;
    }
    
    NSError *error;
    
    _renderPipelineState = [_device newRenderPipelineStateWithDescriptor:_renderPipelineDescriptor
                                                                   error:&error];
    NSAssert(_renderPipelineState, @"Failed to create the pipeline state: %@", error);
}

- (void)createResolveKernelPrograms:(id<MTLLibrary>)metalLibrary {
    _averageResolveIMRKernelFunction = [metalLibrary newFunctionWithName:@"averageResolveKernel"];
    NSAssert(_averageResolveIMRKernelFunction, @"Couldn't load average resolve function from default library.");
    
    _hdrResolveIMRKernelFunction = [metalLibrary newFunctionWithName:@"hdrResolveKernel"];
    NSAssert(_hdrResolveIMRKernelFunction, @"Couldn't load HDR resolve function from default library");
}

- (void)createResolvePipelineState:(id<MTLLibrary>)metalLibrary
{
    NSError *error;
    
    {
        // Create compute pipeline for traditional resolve.
        {
            _resolveComputePipelineState = [_device newComputePipelineStateWithFunction:_averageResolveIMRKernelFunction
                                                                                  error:nil];
            
            NSUInteger threadgroupHeight = _resolveComputePipelineState.maxTotalThreadsPerThreadgroup / _resolveComputePipelineState.threadExecutionWidth;
            
            _intrinsicThreadgroupSize = MTLSizeMake(_resolveComputePipelineState.threadExecutionWidth, threadgroupHeight, 1);
        }
    }
    
    // Composite (copy) the rendered scene to the render target.
    {
        id<MTLFunction> compositionVertexProgram = [metalLibrary newFunctionWithName:@"compositeVertexShader"];
        NSAssert(compositionVertexProgram, @"Couldn't load copy vertex function from default library");
        
        id<MTLFunction> compositionFragmentProgram = [metalLibrary newFunctionWithName:@"compositeFragmentShader"];
        NSAssert(compositionFragmentProgram, @"Couldn't load copy fragment function from default library");
        
        MTLRenderPipelineDescriptor * compositionPipelineDescriptor = [MTLRenderPipelineDescriptor new];
        
        compositionPipelineDescriptor.label                            = @"CompositionResolveResultPipeline";
        compositionPipelineDescriptor.vertexFunction                   = compositionVertexProgram;
        compositionPipelineDescriptor.fragmentFunction                 = compositionFragmentProgram;
        compositionPipelineDescriptor.colorAttachments[0].pixelFormat  = _drawablePixelFormat;
        
        _compositionPipelineState = [_device newRenderPipelineStateWithDescriptor:compositionPipelineDescriptor
                                                                            error:&error];
        NSAssert(_compositionPipelineState, @"Failed acquiring pipeline state: %@", error);
    }
}

- (void)createMultisampleTextureDescriptor
{
    _multisampleTextureDescriptor = [MTLTextureDescriptor new];
    
    _multisampleTextureDescriptor.pixelFormat = _renderTargetPixelFormat;
    
    _multisampleTextureDescriptor.textureType = MTLTextureType2DMultisample;
    _multisampleTextureDescriptor.sampleCount = _antialiasingSampleCount;
    
    {
        _multisampleTextureDescriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
        _multisampleTextureDescriptor.storageMode = MTLStorageModePrivate;
    }
    
    _resolveTextureDescriptor = [MTLTextureDescriptor new];
    _resolveTextureDescriptor.pixelFormat = _renderTargetPixelFormat;
    _resolveTextureDescriptor.storageMode = MTLStorageModePrivate;
    _resolveTextureDescriptor.usage = MTLResourceUsageRead | MTLTextureUsageRenderTarget;
    _resolveTextureDescriptor.usage |= (_resolveOption != AAPLResolveOptionBuiltin) ? MTLResourceUsageWrite : 0;
    _resolveTextureDescriptor.textureType = MTLTextureType2D;
}

- (void)createMultisampleTexture
{
    _multisampleTextureDescriptor.width = _viewportSize.x;
    _multisampleTextureDescriptor.height = _viewportSize.y;
    
    _multisampleTexture = [_device newTextureWithDescriptor:_multisampleTextureDescriptor];
    
    _multisampleTexture.label = @"Multisampled Texture";
    
    _resolveTextureDescriptor.width = _viewportSize.x;
    _resolveTextureDescriptor.height = _viewportSize.y;
    _resolveResultTexture = [_device newTextureWithDescriptor:_resolveTextureDescriptor];
    
    _resolveResultTexture.label = @"Resolved Texture";
}

- (void)createMaskTexture {
    NSError *error;

    /// Texture
    size_t width, height;
    uint32_t *bitmapData = [DMMaskManager getBitdataWithData:&width height:&height];
    MTLTextureDescriptor *textureDescriptor = [[MTLTextureDescriptor alloc] init];
    textureDescriptor.textureType = MTLTextureType2D;
    textureDescriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
    textureDescriptor.width = width;
    textureDescriptor.height = height;
    textureDescriptor.usage = MTLTextureUsageShaderRead;
    // 384*160
    _maskTexture = [_device newTextureWithDescriptor:textureDescriptor];
    NSAssert(_maskTexture, @"Could not load mask texture: %@", error);
    _maskTexture.label = @"mask Texture";
    MTLRegion mtlRegion = MTLRegionMake3D(0, 0, 0, width, height, 1);
    [_maskTexture replaceRegion:mtlRegion mipmapLevel:0 withBytes:bitmapData bytesPerRow:width * 4];
}

#pragma mark - Render Loop

- (void)drawInMTKView:(nonnull MTKView*)view {
    if (_antialiasingOptionsChanged) {
        [self updateAntialiasingInPipeline];
        _antialiasingOptionsChanged = NO;
    }
    
    // Create a new command buffer for each render pass to the current drawable.
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    
    id<CAMetalDrawable> currentDrawable = [view currentDrawable];
    
    // Skip rendering the frame if the current drawable is nil.
    if (!currentDrawable) {
        return;
    }
    
    MTLRenderPassDescriptor *renderPassDescriptor = [MTLRenderPassDescriptor new];
    
    renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(_backgroundBrightness, _backgroundBrightness, _backgroundBrightness, 1);
    
    BOOL shouldResolve = _antialiasingEnabled && (_resolveOption == AAPLResolveOptionBuiltin);
    if (_antialiasingEnabled) {
        MTLStoreAction storeAction = shouldResolve ? MTLStoreActionMultisampleResolve : MTLStoreActionStore;
        renderPassDescriptor.colorAttachments[0].storeAction = storeAction;
        renderPassDescriptor.colorAttachments[0].texture = _multisampleTexture;
        renderPassDescriptor.colorAttachments[0].resolveTexture = shouldResolve ? _resolveResultTexture : nil;
    } else {
        renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
        renderPassDescriptor.colorAttachments[0].texture = _resolveResultTexture;
        renderPassDescriptor.colorAttachments[0].resolveTexture = nil;
    }
    
    id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];

    { /* 将素材渲染到多重纹理上 */
        CGSize screenSize = view.drawableSize;
        CGFloat screenScale = screenSize.width / screenSize.height;
        CGSize imageSize = CGSizeMake(_maskTexture.width, _maskTexture.height);
        CGFloat imageScale = imageSize.width / imageSize.height;
        if (screenScale > imageScale) {
            imageSize = CGSizeMake(screenSize.height * imageScale, screenSize.height);
        } else {
            imageSize = CGSizeMake(screenSize.width, screenSize.width / imageScale);
        }
        CGFloat width = imageSize.width / screenSize.width, height = imageSize.height / screenSize.height;
        CGFloat x1 = (1.0 - width) / 2.0, x2 = x1 + width;
        CGFloat y1 = (1.0 - height) / 2.0, y2 = y1 + height;
        vector_float4 quadVertices[6] = {
            {x1, y1, 0.0, 0.0 },
            {x1, y2, 0.0, 1.0 },
            {x2, y1, 1.0, 0.0 },
            {x2, y2, 1.0, 1.0 },
            {x1, y2, 0.0, 1.0 },
            {x2, y1, 1.0, 0.0 },
        };
        
        renderEncoder.label = [NSString stringWithFormat:@"%@%@", @"Render", shouldResolve ? @" + Resolve" : @""];
        [renderEncoder setRenderPipelineState:_renderPipelineState];
        [renderEncoder setFragmentTexture:_maskTexture atIndex:AAPLVertexInputIndexTexture];
        [renderEncoder setVertexBytes:quadVertices length:sizeof(quadVertices) atIndex:AAPLVertexInputIndexVertices];
        // Render both the inner and outer layers of shards.
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
        [renderEncoder endEncoding];
    }
    
        
    if (_antialiasingEnabled && _resolveOption != AAPLResolveOptionBuiltin) {
        /// 多次采样，采样结果存储到 resolveResultTexture
        id<MTLComputeCommandEncoder> computeEncoder = [commandBuffer computeCommandEncoder];
        computeEncoder.label = @"Resolve on Compute";
        [computeEncoder setComputePipelineState:_resolveComputePipelineState];
        [computeEncoder setTexture:_multisampleTexture atIndex:0];
        [computeEncoder setTexture:_resolveResultTexture atIndex:1];
        [computeEncoder dispatchThreadgroups:_threadgroupsInGrid threadsPerThreadgroup:_intrinsicThreadgroupSize];
        [computeEncoder endEncoding];
    }
    
    { /// 多重采样的结果渲染到屏幕
        renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
        renderPassDescriptor.colorAttachments[0].texture = currentDrawable.texture;
        renderPassDescriptor.colorAttachments[0].resolveTexture = nil;
        renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"Composite Pass";
        [renderEncoder setRenderPipelineState:_compositionPipelineState];
        [renderEncoder setFragmentTexture:_resolveResultTexture atIndex:0];
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
        [renderEncoder endEncoding];
    }
    
    [commandBuffer presentDrawable:currentDrawable];
    [commandBuffer commit];
}

- (void)drawableSizeWillChange:(CGSize)drawableSize {
    _viewportSize.x = drawableSize.width * _renderingQuality;
    _viewportSize.y = drawableSize.height * _renderingQuality;
    
    [self createMultisampleTexture];
    
    _threadgroupsInGrid.width = (_viewportSize.x + _intrinsicThreadgroupSize.width - 1) / _intrinsicThreadgroupSize.width;
    _threadgroupsInGrid.height = (_viewportSize.y + _intrinsicThreadgroupSize.height - 1) / _intrinsicThreadgroupSize.height;
    _threadgroupsInGrid.depth = 1;
}

#pragma mark - 抗锯齿控制

- (void)updateAntialiasingInPipeline {
    if (_antialiasingEnabled) {
        [self createMultisampleTextureDescriptor];
        [self createMultisampleTexture];
    }
    
    if (_antialiasingEnabled) {
        _renderPipelineDescriptor.sampleCount = _antialiasingSampleCount;
        _renderPipelineDescriptor.fragmentFunction = _fragmentFunctionNonHDR;
    } else {
        _renderPipelineDescriptor.sampleCount = 1;
        _renderPipelineDescriptor.fragmentFunction = _usesHDR ? _fragmentFunctionHDR : _fragmentFunctionNonHDR;
    }
    _renderPipelineState = [_device newRenderPipelineStateWithDescriptor:_renderPipelineDescriptor error:nil];
    
    if (_antialiasingEnabled) {
        [self updateResolveOptionInPipeline];
    }
}

/// 更新解析管道
- (void)updateResolveOptionInPipeline {
    _usesHDR = _resolveOption == AAPLResolveOptionHDR;
    {
        switch (_resolveOption)
        {
            case AAPLResolveOptionBuiltin:
                /// 如果使用内置解析，则不需要自定义解析管道
                break;
            case AAPLResolveOptionAverage:
                _resolveComputePipelineState = [_device newComputePipelineStateWithFunction:_averageResolveIMRKernelFunction
                                                                                      error:nil];
                break;
            case AAPLResolveOptionHDR:
                _resolveComputePipelineState = [_device newComputePipelineStateWithFunction:_hdrResolveIMRKernelFunction
                                                                                      error:nil];
                break;
            default:
                break;
        }
    }
}

@end
