@import simd;
@import MetalKit;
#import "AAPLRenderer.h"
#import "AAPLShaderTypes.h"

@implementation AAPLRenderer
{
    id<MTLDevice> _device;
    id<MTLRenderPipelineState> _pipelineState;
    id<MTLCommandQueue> _commandQueue;
    vector_float2 _viewportSize;
    CFTimeInterval _currentTime;
}

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView {
    self = [super init];
    if(self) {
        _currentTime = 0;
        NSError *error;

        _device = mtkView.device;

        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
        id<MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
        id<MTLFunction> fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentShader"];

        MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"Simple Pipeline";
        pipelineStateDescriptor.vertexFunction = vertexFunction;
        pipelineStateDescriptor.fragmentFunction = fragmentFunction;
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
        _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor
                                                                 error:&error];
        NSAssert(_pipelineState, @"Failed to create pipeline state: %@", error);
        _commandQueue = [_device newCommandQueue];
    }

    return self;
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    _viewportSize.x = size.width * 1.0;
    _viewportSize.y = size.height * 1.0;
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;
    if(renderPassDescriptor == nil) return;
    static const vector_float4 quadVertices[6] = {
        {0.0, 0.0, 0.0, 0.0 },
        {0.0, 1.0, 0.0, 1.0 },
        {1.0, 0.0, 1.0, 0.0 },
        {1.0, 1.0, 1.0, 1.0 },
        {0.0, 1.0, 0.0, 1.0 },
        {1.0, 0.0, 1.0, 0.0 },
    };
    _currentTime += 0.1;

    Uniforms data;
    data.timeStep = _currentTime;
    data.viewportSize = _viewportSize;
    
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";

    id<MTLRenderCommandEncoder> renderEncoder =
    [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    renderEncoder.label = @"MyRenderEncoder";
    [renderEncoder setRenderPipelineState:_pipelineState];
    [renderEncoder setVertexBytes:quadVertices length:sizeof(quadVertices) atIndex:AAPLInputIndexVertices];
    [renderEncoder setFragmentBytes:&data length:sizeof(Uniforms) atIndex:AAPLInputIndexUniforms];
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
    [renderEncoder endEncoding];
    [commandBuffer presentDrawable:view.currentDrawable];
    [commandBuffer commit];
}

@end
