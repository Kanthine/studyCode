@import MetalKit;
#import "AAPLRenderer.h"
#import "AAPLShaderTypes.h"

@implementation AAPLRenderer {
    id<MTLDevice>              _device;
    id<MTLCommandQueue>        _commandQueue;
    id<MTLRenderPipelineState> _pipelineState;
    id<MTLDepthStencilState> _depthState; /// 深度与模板测试
    vector_float2            _viewportSize;
}

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView {
    self = [super init];
    if(self) {
        mtkView.clearColor = MTLClearColorMake(0, 0, 0, 1);
        /// 深度缓冲区中的每个像素都是32位浮点值
        mtkView.depthStencilPixelFormat = MTLPixelFormatDepth32Float;
        /// 每帧开始渲染时，将深度缓冲区中的每个像素的深度值统一刷成 1.0
        mtkView.clearDepth = 1.0;
        
        _device = mtkView.device;
        
        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
        id<MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
        id<MTLFunction> fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentShader"];
        
        MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"Render Pipeline";
        pipelineStateDescriptor.sampleCount = mtkView.sampleCount;
        pipelineStateDescriptor.vertexFunction = vertexFunction;
        pipelineStateDescriptor.fragmentFunction = fragmentFunction;
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
        pipelineStateDescriptor.depthAttachmentPixelFormat = mtkView.depthStencilPixelFormat;
        pipelineStateDescriptor.vertexBuffers[AAPLVertexInputIndexVertices].mutability = MTLMutabilityImmutable;
        
        NSError *error;
        _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];
        NSAssert(_pipelineState, @"Failed to create pipeline state: %@", error);
        
        /// 深度测试对象
        MTLDepthStencilDescriptor *depthDescriptor = [MTLDepthStencilDescriptor new];
        depthDescriptor.depthCompareFunction = MTLCompareFunctionLessEqual;
        depthDescriptor.depthWriteEnabled = YES;
        _depthState = [_device newDepthStencilStateWithDescriptor:depthDescriptor];
        _commandQueue = [_device newCommandQueue];
    }
    return self;
}

#pragma mark - MTKView Delegate Methods

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;
    if(renderPassDescriptor == nil) return;

    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer]; /// 从命令队列获取一块缓冲区
    commandBuffer.label = @"Command Buffer";
    
    /// 使用缓冲区创建一个命令编码器
    id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    renderEncoder.label = @"Render Encoder";
    [renderEncoder setRenderPipelineState:_pipelineState]; /// 设置渲染管线
    [renderEncoder setDepthStencilState:_depthState];      /// 设置深度测试
    [renderEncoder setVertexBytes:&_viewportSize length:sizeof(_viewportSize) atIndex:AAPLVertexInputIndexViewport];
    
    const AAPLVertex quadVertices[] = {
        // Pixel positions (x, y) and clip depth (z),        RGBA colors.
        { {                 100,                 100, 0.5 }, { 0.5, 0.5, 0.5, 1 } },
        { {                 100, _viewportSize.y-100, 0.5 }, { 0.5, 0.5, 0.5, 1 } },
        { { _viewportSize.x-100, _viewportSize.y-100, 0.5 }, { 0.5, 0.5, 0.5, 1 } },
        
        { {                 100,                 100, 0.5 }, { 0.5, 0.5, 0.5, 1 } },
        { { _viewportSize.x-100, _viewportSize.y-100, 0.5 }, { 0.5, 0.5, 0.5, 1 } },
        { { _viewportSize.x-100,                 100, 0.5 }, { 0.5, 0.5, 0.5, 1 } },
    };
    
    [renderEncoder setVertexBytes:quadVertices length:sizeof(quadVertices) atIndex:AAPLVertexInputIndexVertices];
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
    

    const AAPLVertex triangleVertices[] = {
        /// Pixel positions (x, y) and clip depth (z),                           RGBA colors.
        { {                    200, _viewportSize.y - 200, _leftVertexDepth  }, { 1, 1, 1, 1 } },
        { {  _viewportSize.x / 2.0,                   200, _topVertexDepth   }, { 1, 1, 1, 1 } },
        { {  _viewportSize.x - 200, _viewportSize.y - 200, _rightVertexDepth }, { 1, 1, 1, 1 } }
    };
    
    [renderEncoder setVertexBytes:triangleVertices length:sizeof(triangleVertices) atIndex:AAPLVertexInputIndexVertices];
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3]; /// 绘制三角形
    [renderEncoder endEncoding];
    [commandBuffer presentDrawable:view.currentDrawable]; /// 在 renderPass 命令完成后，呈现到 drawable
    [commandBuffer commit]; /// 将命令提交到 GPU
}

@end
