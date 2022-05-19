#import "MaskRender.h"
#include "MaskShaderType.h"

@interface MaskRender ()
{
    id<MTLRenderPipelineState> _maskPipelineState;
    id<MTLDepthStencilState>   _maskStencilState;
    MTLRenderPassDescriptor   *_maskRenderPassDescriptor;
}
@end


@implementation MaskRender

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView {
    self = [super init];
    if (self) {
        
        id<MTLLibrary> defaultLibrary = [mtkView.device newDefaultLibrary];
        MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"蒙版·管道管线";
        pipelineStateDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
        pipelineStateDescriptor.depthAttachmentPixelFormat = mtkView.depthStencilPixelFormat;
        pipelineStateDescriptor.stencilAttachmentPixelFormat = mtkView.depthStencilPixelFormat;
        NSError *error;
        _maskPipelineState = [mtkView.device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];
        NSAssert(_maskPipelineState, @"Failed to create pipeline state: %@", error);
        
        MTLStencilDescriptor *stencilStateDesc = [MTLStencilDescriptor new];
        stencilStateDesc.stencilCompareFunction = MTLCompareFunctionAlways;      /// 新值总是通过测试
        stencilStateDesc.stencilFailureOperation = MTLStencilOperationKeep;      /// 保持深度缓冲区的原有值
        /// stencilStateDesc.depthFailureOperation = MTLStencilOperationIncrementClamp;
        stencilStateDesc.depthStencilPassOperation = MTLStencilOperationReplace; /// 拿参考值更新深度缓冲区的原有值

        
        MTLDepthStencilDescriptor *depthStencilDesc = [MTLDepthStencilDescriptor new];
        depthStencilDesc.label = @"蒙版·测试";
        depthStencilDesc.depthWriteEnabled = NO;
        depthStencilDesc.frontFaceStencil = stencilStateDesc;
        depthStencilDesc.backFaceStencil = stencilStateDesc;
        _maskStencilState = [mtkView.device newDepthStencilStateWithDescriptor:depthStencilDesc];
    }
    return self;
}

- (void)drawMaskWithRender:(id<MTLRenderCommandEncoder>)renderEncoder viewportSize:(vector_float2)viewportSize {
    vector_float2 vertices[3] = {
        simd_make_float2(      viewportSize.x / 4.0, viewportSize.y / 2.0),
        simd_make_float2(3.0 * viewportSize.x / 4.0, viewportSize.y / 2.0),
        simd_make_float2(      viewportSize.x / 2.0, viewportSize.y),
    };
    
    [renderEncoder setRenderPipelineState:_maskPipelineState];
    [renderEncoder setStencilReferenceValue:128];
    [renderEncoder setDepthStencilState:_maskStencilState];
    [renderEncoder setVertexBytes:&viewportSize length:sizeof(viewportSize) atIndex:VertexInputIndexViewport];
    [renderEncoder setVertexBytes:vertices length:sizeof(vertices) atIndex:VertexInputIndexVertices];
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
}

@end
