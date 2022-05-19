@import simd;
@import MetalKit;

#import "AAPLRenderer.h"
#import "AAPLImage.h"
#import "AAPLShaderTypes.h"

@implementation AAPLRenderer
{
    MTKView *_mtkView;

    id<MTLDevice> _device;
    id<MTLCommandQueue> _commandQueue;

    id<MTLRenderPipelineState> _renderPipelineState;
    MTLRenderPassDescriptor *_renderPassDescriptor;

    id<MTLComputePipelineState> _computePipelineState;


    id<MTLTexture> _imageTexture;
    id<MTLTexture> _drawableTexture;
    id<MTLTexture> _finalTexture;

    vector_float2 _viewportSize;
    float _timer;
    
    MTLSize _threadgroupSize;
    MTLSize _threadgroupCount;
}

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView {
    self = [super init];
    if(self) {
        _timer = 0;
        NSError *error = NULL;
        _device = mtkView.device;
        mtkView.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
        _mtkView = mtkView;
        
        MTLTextureDescriptor *drawableDescriptor = [[MTLTextureDescriptor alloc] init];
        drawableDescriptor.textureType = MTLTextureType2D;
        drawableDescriptor.pixelFormat = mtkView.colorPixelFormat;
        drawableDescriptor.width = mtkView.drawableSize.width;
        drawableDescriptor.height = mtkView.drawableSize.height;
        drawableDescriptor.usage = MTLTextureUsageShaderWrite | MTLTextureUsageShaderRead | MTLTextureUsageRenderTarget;
        _drawableTexture = [_device newTextureWithDescriptor:drawableDescriptor];
        _finalTexture = [_device newTextureWithDescriptor:drawableDescriptor];

        _renderPassDescriptor = [MTLRenderPassDescriptor new];
        _renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 0.0);
        _renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
        _renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
        _renderPassDescriptor.colorAttachments[0].texture = _drawableTexture;
        
        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
        id<MTLFunction> kernelFunction = [defaultLibrary newFunctionWithName:@"grayscaleKernel"];
        _computePipelineState = [_device newComputePipelineStateWithFunction:kernelFunction
                                                                       error:&error];
        NSAssert(_computePipelineState, @"Failed to create compute pipeline state: %@", error);
        
        id<MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
        id<MTLFunction> fragmentFunction = [defaultLibrary newFunctionWithName:@"samplingShader"];
        MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"Simple Render Pipeline";
        pipelineStateDescriptor.vertexFunction = vertexFunction;
        pipelineStateDescriptor.fragmentFunction = fragmentFunction;
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
        _renderPipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];
        NSAssert(_renderPipelineState, @"Failed to create render pipeline state: %@", error);

        NSURL *imageFileLocation = [[NSBundle mainBundle] URLForResource:@"Image" withExtension:@"tga"];
        AAPLImage * image = [[AAPLImage alloc] initWithTGAFileAtLocation:imageFileLocation];
        if(!image) return nil;
        
        MTLTextureDescriptor *textureDescriptor = [[MTLTextureDescriptor alloc] init];
        textureDescriptor.textureType = MTLTextureType2D;
        textureDescriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
        textureDescriptor.width = image.width;
        textureDescriptor.height = image.height;
        textureDescriptor.usage = MTLTextureUsageShaderRead;
        _imageTexture = [_device newTextureWithDescriptor:textureDescriptor];
        
        MTLRegion region = {{ 0, 0, 0 }, {textureDescriptor.width, textureDescriptor.height, 1}};
        NSUInteger bytesPerRow = 4 * textureDescriptor.width;
        [_imageTexture replaceRegion:region mipmapLevel:0 withBytes:image.data.bytes bytesPerRow:bytesPerRow];
        NSAssert(_imageTexture && !error, @"Failed to create inpute texture: %@", error);
        _threadgroupSize = MTLSizeMake(16, 16, 1);
        _threadgroupCount.width  = (_drawableTexture.width  + _threadgroupSize.width -  1) / _threadgroupSize.width;
        _threadgroupCount.height = (_drawableTexture.height + _threadgroupSize.height - 1) / _threadgroupSize.height;
        _threadgroupCount.depth = 1;
        _commandQueue = [_device newCommandQueue];
    }
    return self;
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    _timer += 1 / 16.0;
    
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";
    
    [self drawOriginWithBuffer:commandBuffer];
    [self handlerOriginWithBuffer:commandBuffer];
    [self drawFinalWithBuffer:commandBuffer];
    [commandBuffer presentDrawable:view.currentDrawable];
    [commandBuffer commit];
}

#pragma mark - Draw

- (void)drawOriginWithBuffer:(id<MTLCommandBuffer>)commandBuffer  {
    static const AAPLVertex quadVertices[] = {
        { {  250,  -250 },  { 1.f, 1.f } },
        { { -250,  -250 },  { 0.f, 1.f } },
        { { -250,   250 },  { 0.f, 0.f } },

        { {  250,  -250 },  { 1.f, 1.f } },
        { { -250,   250 },  { 0.f, 0.f } },
        { {  250,   250 },  { 1.f, 0.f } },
    };
    
    id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:_renderPassDescriptor];
    renderEncoder.label = @"MyRenderEncoder";
    [renderEncoder pushDebugGroup:@"Draw Origin Data"];

    [renderEncoder setViewport:(MTLViewport){0.0, 0.0, _viewportSize.x, _viewportSize.y, -1.0, 1.0 }];
    [renderEncoder setRenderPipelineState:_renderPipelineState];
    [renderEncoder setVertexBytes:quadVertices length:sizeof(quadVertices) atIndex:AAPLVertexInputIndexVertices];
    [renderEncoder setVertexBytes:&_viewportSize length:sizeof(_viewportSize) atIndex:AAPLVertexInputIndexViewportSize];
    [renderEncoder setFragmentTexture:_imageTexture atIndex:AAPLTextureIndexOutput];
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
    [renderEncoder endEncoding];
    [renderEncoder popDebugGroup];
}

- (void)handlerOriginWithBuffer:(id<MTLCommandBuffer>)commandBuffer {
    id<MTLComputeCommandEncoder> computeEncoder = [commandBuffer computeCommandEncoder];
    [computeEncoder pushDebugGroup:@"Handler Origin Data"];
    [computeEncoder setComputePipelineState:_computePipelineState];
    [computeEncoder setTexture:_drawableTexture atIndex:AAPLTextureIndexInput];
    [computeEncoder setTexture:_finalTexture atIndex:AAPLTextureIndexOutput];
    [computeEncoder setBytes:&_viewportSize length:sizeof(_viewportSize) atIndex:AAPLVertexInputIndexViewportSize];
    [computeEncoder setBytes:&_timer length:sizeof(float) atIndex:AAPLVertexInputIndexTimer];
    [computeEncoder dispatchThreadgroups:_threadgroupCount threadsPerThreadgroup:_threadgroupSize];
    [computeEncoder endEncoding];
    [computeEncoder popDebugGroup];
}

- (void)drawFinalWithBuffer:(id<MTLCommandBuffer>)commandBuffer {
    MTLRenderPassDescriptor *renderPassDescriptor = _mtkView.currentRenderPassDescriptor;
    if(renderPassDescriptor == nil) return;
    
    float width = _viewportSize.x / 2.0, height = _viewportSize.y / 2.0;
    AAPLVertex quadVertices[] = {
        { { width,  -height},  { 1.f, 1.f } },
        { {-width,  -height},  { 0.f, 1.f } },
        { {-width,   height},  { 0.f, 0.f } },

        { { width,  -height},  { 1.f, 1.f } },
        { {-width,   height},  { 0.f, 0.f } },
        { { width,   height},  { 1.f, 0.f } },
    };
    
    id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    [renderEncoder pushDebugGroup:@"Draw Final Data"];
    renderEncoder.label = @"MyRenderEncoder";
    [renderEncoder setViewport:(MTLViewport){0.0, 0.0, _viewportSize.x, _viewportSize.y, -1.0, 1.0 }];
    [renderEncoder setRenderPipelineState:_renderPipelineState];
    [renderEncoder setVertexBytes:quadVertices length:sizeof(quadVertices) atIndex:AAPLVertexInputIndexVertices];
    [renderEncoder setVertexBytes:&_viewportSize length:sizeof(_viewportSize) atIndex:AAPLVertexInputIndexViewportSize];
    [renderEncoder setFragmentTexture:_finalTexture atIndex:AAPLTextureIndexOutput];
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
    [renderEncoder endEncoding];
    [renderEncoder popDebugGroup];
}

@end
