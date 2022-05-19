@import simd;
@import MetalKit;

#import "AAPLRenderer.h"
#import "AAPLShaderTypes.h"

@implementation AAPLRenderer
{
    id<MTLDevice> _device;
    id<MTLComputePipelineState> _computePipelineState;
    id<MTLRenderPipelineState> _renderPipelineState;
    id<MTLCommandQueue> _commandQueue;
    id<MTLTexture> _outputTexture;

    vector_float2 _viewportSize;
    CGRect _rectangleFrame;

    float _timer;
    id<MTLBuffer> _timerBuffer;
}

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView {
    self = [super init];
    if(self) {
        NSError *error = NULL;
        _rectangleFrame = CGRectMake(-450, -450, 900, 900);

        mtkView.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
        _device = mtkView.device;

        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
        id<MTLFunction> kernelFunction = [defaultLibrary newFunctionWithName:@"compute"];
        _computePipelineState = [_device newComputePipelineStateWithFunction:kernelFunction error:&error];
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
        
        _timerBuffer = [_device newBufferWithLength:sizeof(float) options:MTLResourceStorageModeShared];
        _commandQueue = [_device newCommandQueue];
        
        MTLTextureDescriptor *textureDescriptor = [[MTLTextureDescriptor alloc] init];
        textureDescriptor.textureType = MTLTextureType2D;
        textureDescriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
        textureDescriptor.width = CGRectGetWidth(_rectangleFrame);
        textureDescriptor.height = CGRectGetHeight(_rectangleFrame);
        textureDescriptor.usage = MTLTextureUsageShaderWrite | MTLTextureUsageShaderRead ;
        _outputTexture = [_device newTextureWithDescriptor:textureDescriptor];
    }
    return self;
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    _timer += 0.01;
    memcpy(_timerBuffer.contents, &_timer, sizeof(float));
    
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";

    id<MTLComputeCommandEncoder> computeEncoder = [commandBuffer computeCommandEncoder];
    [computeEncoder setComputePipelineState:_computePipelineState];
    [computeEncoder setBuffer:_timerBuffer offset:0 atIndex:0];
    [computeEncoder setTexture:_outputTexture atIndex:AAPLTextureIndexOutput];
    MTLSize threadGroupCount = MTLSizeMake(8, 8, 1);
    MTLSize threadGroups = MTLSizeMake(view.currentDrawable.texture.width / threadGroupCount.width, view.currentDrawable.texture.height / threadGroupCount.height, 1);
    [computeEncoder dispatchThreadgroups:threadGroups threadsPerThreadgroup:threadGroupCount];
    [computeEncoder endEncoding];

    
    float x = _rectangleFrame.origin.x, y = _rectangleFrame.origin.y;
    float width = _rectangleFrame.size.width, height = _rectangleFrame.size.height;
    AAPLVertex quadVertices[6] =
    {
        { { x, y},  { 0.f, 0.f } },
        { { x + width,  y },  { 0.f, 1.f } },
        { { x + width,  y + height},  { 1.f, 1.f } },

        { { x, y},  { 0.f, 0.f } },
        { { x,  y + height},  { 1.f, 0.f } },
        { { x + width,  y + height},  { 1.f, 1.f } },
    };
    
    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;
    if(renderPassDescriptor != nil) {
        id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"MyRenderEncoder";
        [renderEncoder setViewport:(MTLViewport){0.0, 0.0, _viewportSize.x, _viewportSize.y, -1.0, 1.0 }];
        [renderEncoder setRenderPipelineState:_renderPipelineState];
        [renderEncoder setVertexBytes:quadVertices length:sizeof(quadVertices)
                              atIndex:AAPLVertexInputIndexVertices];

        [renderEncoder setVertexBytes:&_viewportSize
                               length:sizeof(_viewportSize)
                              atIndex:AAPLVertexInputIndexViewportSize];

        [renderEncoder setFragmentTexture:_outputTexture
                                  atIndex:AAPLTextureIndexOutput];
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle
                          vertexStart:0
                          vertexCount:6];
        [renderEncoder endEncoding];
        [commandBuffer presentDrawable:view.currentDrawable];
    }

    [commandBuffer commit];
}

@end
