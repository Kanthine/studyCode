@import MetalKit;

#import "AAPLRenderer.h"
#import "AAPLShaderTypes.h"


@implementation AAPLRenderer {
    // 离屏渲染的目标纹理
    id<MTLTexture> _renderTargetTexture;

    // 离屏渲染的 RenderPass
    MTLRenderPassDescriptor* _offscreenRenderPass;

    // 离屏渲染管线
    id<MTLRenderPipelineState> _renderToTextureRenderPipeline;

    // 实时渲染
    id<MTLRenderPipelineState> _drawableRenderPipeline;

    id<MTLDevice> _device;
    id<MTLCommandQueue> _commandQueue;
    
    float _aspectRatio;
}

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView {
    self = [super init];
    if(self) {
        NSError *error;
        mtkView.clearColor = MTLClearColorMake(1.0, 0.0, 0.0, 1.0);
        _device = mtkView.device;
        _commandQueue = [_device newCommandQueue];
        
        // Set up a texture for rendering to and sampling from
        MTLTextureDescriptor *texDescriptor = [MTLTextureDescriptor new];
        texDescriptor.textureType = MTLTextureType2D;
        texDescriptor.width = 512;
        texDescriptor.height = 512;
        texDescriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
        texDescriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
        _renderTargetTexture = [_device newTextureWithDescriptor:texDescriptor];

        // Set up a render pass descriptor for the render pass to render into _renderTargetTexture.
        _offscreenRenderPass = [MTLRenderPassDescriptor new];
        _offscreenRenderPass.colorAttachments[0].texture = _renderTargetTexture;
        _offscreenRenderPass.colorAttachments[0].loadAction = MTLLoadActionClear;
        _offscreenRenderPass.colorAttachments[0].clearColor = MTLClearColorMake(1, 1, 1, 1);
        _offscreenRenderPass.colorAttachments[0].storeAction = MTLStoreActionStore;
        
        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];

        MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"Drawable Render Pipeline";
        pipelineStateDescriptor.sampleCount = mtkView.sampleCount;
        pipelineStateDescriptor.vertexFunction =  [defaultLibrary newFunctionWithName:@"textureVertexShader"];
        pipelineStateDescriptor.fragmentFunction =  [defaultLibrary newFunctionWithName:@"textureFragmentShader"];
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
        pipelineStateDescriptor.vertexBuffers[AAPLVertexInputIndexVertices].mutability = MTLMutabilityImmutable;
        _drawableRenderPipeline = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];

        NSAssert(_drawableRenderPipeline, @"Failed to create pipeline state to render to screen: %@", error);

        // Set up pipeline for rendering to the offscreen texture.
        // Reuse the descriptor and change properties that differ.
        pipelineStateDescriptor.label = @"Offscreen Render Pipeline";
        pipelineStateDescriptor.sampleCount = 1;
        pipelineStateDescriptor.vertexFunction =  [defaultLibrary newFunctionWithName:@"simpleVertexShader"];
        pipelineStateDescriptor.fragmentFunction =  [defaultLibrary newFunctionWithName:@"simpleFragmentShader"];
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = _renderTargetTexture.pixelFormat;
        _renderToTextureRenderPipeline = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];
        NSAssert(_renderToTextureRenderPipeline, @"Failed to create pipeline state to render to texture: %@", error);
    }
    return self;
}

#pragma mark - MetalKit View Delegate

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    _aspectRatio =  (float)size.height / (float)size.width;
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    /// 使用命令队列创建一个缓冲区
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"Command Buffer";

    /// 阶段一：离屏渲染阶段
    {
        static const AAPLSimpleVertex triVertices[] = {
            // Positions     ,  Colors
            { {  0.5,  -0.5 },  { 1.0, 0.0, 0.0, 1.0 } },
            { { -0.5,  -0.5 },  { 0.0, 1.0, 0.0, 1.0 } },
            { {  0.0,   0.5 },  { 0.0, 0.0, 1.0, 0.0 } },
        };
        
        /// 使用自定义的 RenderPass 创建一个绘图命令编码器
        id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:_offscreenRenderPass];
        renderEncoder.label = @"Offscreen Render Pass";
        [renderEncoder setRenderPipelineState:_renderToTextureRenderPipeline];
        [renderEncoder setVertexBytes:&triVertices length:sizeof(triVertices) atIndex:AAPLVertexInputIndexVertices];
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
        
        /// 当有多个 RenderPass 分别渲染，期望这些 RenderPass 按顺序执行命令
        /// 必须保证在 RenderPass 开始之前、完成上一个 RenderPass 的命令编码
        [renderEncoder endEncoding];
    }
    
    /// 阶段二：实时渲染阶段
    /// 通过 view 获取的 RenderPass 最终渲染到 CAMetalLayer.drawable.texture，因此是实时渲染
    MTLRenderPassDescriptor *drawableRenderPassDescriptor = view.currentRenderPassDescriptor;
    if(drawableRenderPassDescriptor != nil) {
        static const AAPLTextureVertex quadVertices[] = {
            // Positions     , Texture coordinates
            { {  0.5,  -0.5 },  { 1.0, 1.0 } },
            { { -0.5,  -0.5 },  { 0.0, 1.0 } },
            { { -0.5,   0.5 },  { 0.0, 0.0 } },

            { {  0.5,  -0.5 },  { 1.0, 1.0 } },
            { { -0.5,   0.5 },  { 0.0, 0.0 } },
            { {  0.5,   0.5 },  { 1.0, 0.0 } },
        };
        
        /// 创建实时渲染编码器
        id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:drawableRenderPassDescriptor];
        renderEncoder.label = @"Drawable Render Pass";
        [renderEncoder setRenderPipelineState:_drawableRenderPipeline];
        [renderEncoder setVertexBytes:&quadVertices length:sizeof(quadVertices) atIndex:AAPLVertexInputIndexVertices];
        [renderEncoder setVertexBytes:&_aspectRatio length:sizeof(_aspectRatio) atIndex:AAPLVertexInputIndexAspectRatio];

        // 离屏渲染的目标纹理，作为实时渲染的源数据
        [renderEncoder setFragmentTexture:_renderTargetTexture atIndex:AAPLTextureInputIndexColor];
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
        [renderEncoder endEncoding];
        [commandBuffer presentDrawable:view.currentDrawable];
    }
    [commandBuffer commit];
}

@end
