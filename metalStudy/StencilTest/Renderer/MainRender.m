@import MetalKit;

#import "MainRender.h"
#include "MaskShaderType.h"
#import "MaskRender.h"

@interface MainRender ()

{
    id<MTLDevice>              _device;
    id<MTLCommandQueue>        _commandQueue;
    vector_float2             _viewportSize;
    
    id<MTLRenderPipelineState> _pipelineState;
    id<MTLDepthStencilState>   _stencilState;
}

@property (nonatomic, strong) MaskRender *maskRender;

@end

@implementation MainRender

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView {
    self = [super init];
    if(self) {
        _device = mtkView.device;
        mtkView.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
        mtkView.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;

        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
        id<MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
        id<MTLFunction> fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentShader"];
        MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"渲染管道";
        pipelineStateDescriptor.vertexFunction = vertexFunction;
        pipelineStateDescriptor.fragmentFunction = fragmentFunction;
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
        pipelineStateDescriptor.depthAttachmentPixelFormat = mtkView.depthStencilPixelFormat;
        pipelineStateDescriptor.stencilAttachmentPixelFormat = mtkView.depthStencilPixelFormat;
        NSError *error;
        _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];
        NSAssert(_pipelineState, @"Failed to create pipeline state: %@", error);
        
        [self buildStencilState];
        
        _commandQueue = [_device newCommandQueue];
        
        _maskRender = [[MaskRender alloc] initWithMetalKitView:mtkView];
    }
    return self;
}

- (void)buildStencilState {
    
    MTLStencilDescriptor *stencilStateDesc = [MTLStencilDescriptor new];
    /// 比较策略：拿 ReferenceValue 参考值和当前像素缓存上的值进行比较
    stencilStateDesc.stencilCompareFunction = MTLCompareFunctionGreater;  /// 新值大于现有值，则通过测试
    stencilStateDesc.stencilFailureOperation = MTLStencilOperationKeep;   /// 模板测试失败·保持当前的模板值
    stencilStateDesc.depthStencilPassOperation = MTLStencilOperationKeep; /// 保持当前模板值
    
    MTLDepthStencilDescriptor *depthStencilDesc = [MTLDepthStencilDescriptor new];
    depthStencilDesc.label = @"渲染·深度模板测试";
    depthStencilDesc.depthWriteEnabled = NO;
    depthStencilDesc.frontFaceStencil = stencilStateDesc;
    depthStencilDesc.backFaceStencil = stencilStateDesc;
    
    _stencilState = [_device newDepthStencilStateWithDescriptor:depthStencilDesc];
}

#pragma mark - MTKView Delegate Methods

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
}

/// 通过模板测试的片段像素点会被替换到颜色缓冲区中，从而显示出来，
/// 未通过的则不会保存到颜色缓冲区中，从而达到了过滤的功能
- (void)drawInMTKView:(nonnull MTKView *)view {
    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;
    if (renderPassDescriptor == nil) return;

    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"Command Buffer";
    
    id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    renderEncoder.label = @"Render Encoder";
    [_maskRender drawMaskWithRender:renderEncoder viewportSize:_viewportSize];
    [self drawTriangleWithRender:renderEncoder];
    [renderEncoder endEncoding];
    [commandBuffer presentDrawable:view.currentDrawable];
    [commandBuffer commit];
}

- (void)drawTriangleWithRender:(id<MTLRenderCommandEncoder>)renderEncoder {
    vector_float2 vertices[3] = {
        simd_make_float2(_viewportSize.x / 2.0,               0),
        simd_make_float2(                    0, _viewportSize.y),
        simd_make_float2(      _viewportSize.x, _viewportSize.y),
    };
    
    [renderEncoder setRenderPipelineState:_pipelineState];
    [renderEncoder setStencilReferenceValue:64];
    [renderEncoder setDepthStencilState:_stencilState];
    [renderEncoder setVertexBytes:&_viewportSize length:sizeof(_viewportSize) atIndex:VertexInputIndexViewport];
    [renderEncoder setVertexBytes:vertices length:sizeof(vertices) atIndex:VertexInputIndexVertices];
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
}

@end
