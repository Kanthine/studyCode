@import simd;
@import MetalKit;

#import "AAPLRenderer.h"
#import "AAPLShaderTypes.h"

@implementation AAPLRenderer {
    id<MTLDevice> _device;

    id<MTLComputePipelineState> _computePipelineState; /// 计算管线
    id<MTLRenderPipelineState> _renderPipelineState;   /// 渲染管线

    id<MTLCommandQueue> _commandQueue; ///命令编码器

    id<MTLTexture> _outputTexture;
    vector_float2 _viewportSize;

    // 计算着色器的调度
    MTLSize _threadgroupSize;
    MTLSize _threadgroupCount;
    
    float _timer;
}

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView {
    self = [super init];
    if(self) {
        NSError *error = NULL;
        mtkView.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
        _device = mtkView.device;

        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
        
        /// 计算着色器
        id<MTLFunction> kernelFunction = [defaultLibrary newFunctionWithName:@"grayscaleKernel"];
        _computePipelineState = [_device newComputePipelineStateWithFunction:kernelFunction error:&error];
        NSAssert(_computePipelineState, @"Failed to create compute pipeline state: %@", error);

        /// 渲染着色器
        id<MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
        id<MTLFunction> fragmentFunction = [defaultLibrary newFunctionWithName:@"samplingShader"];
        MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"Simple Render Pipeline";
        pipelineStateDescriptor.vertexFunction = vertexFunction;
        pipelineStateDescriptor.fragmentFunction = fragmentFunction;
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
        _renderPipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor
                                                                 error:&error];
        NSAssert(_renderPipelineState, @"Failed to create render pipeline state: %@", error);
        
        MTLTextureDescriptor *textureDescriptor = [[MTLTextureDescriptor alloc] init];
        textureDescriptor.textureType = MTLTextureType2D;
        textureDescriptor.pixelFormat = MTLPixelFormatBGRA8Unorm; /// 像素格式：BGRA8
        textureDescriptor.width  = mtkView.drawableSize.width;
        textureDescriptor.height = mtkView.drawableSize.height;
        textureDescriptor.usage = MTLTextureUsageShaderWrite | MTLTextureUsageShaderRead;
        _outputTexture = [_device newTextureWithDescriptor:textureDescriptor];


        _threadgroupSize = MTLSizeMake(16, 16, 1); /// 计算着色器的线程组单元是 16 x 16

        /// 线程组单元的个数（可以更多，确保覆盖整个图像）
        _threadgroupCount.width  = (_outputTexture.width  + _threadgroupSize.width -  1) / _threadgroupSize.width;
        _threadgroupCount.height = (_outputTexture.height + _threadgroupSize.height - 1) / _threadgroupSize.height;
        _threadgroupCount.depth = 1;

        _commandQueue = [_device newCommandQueue];
        
        _timer = 0.0;
    }

    return self;
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    _timer += 0.01;

    /// 为每一帧渲染创建一个命令缓冲区
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";

    /****************** 使用计算着色器处理纹理 ******************/
    id<MTLComputeCommandEncoder> computeEncoder = [commandBuffer computeCommandEncoder];
    [computeEncoder setComputePipelineState:_computePipelineState];
    [computeEncoder setTexture:_outputTexture atIndex:TextureIndexOutput];
    [computeEncoder setBytes:&_timer length:sizeof(_timer) atIndex:TextureIndexTimer];
    [computeEncoder dispatchThreadgroups:_threadgroupCount threadsPerThreadgroup:_threadgroupSize];
    [computeEncoder endEncoding];

    /****************** 使用渲染着色器绘制内容到目标纹理 ******************/
    /// 经过计算着色器处理后，在同一个命令缓冲区中继续编码一个渲染管线，将来自 computePass 命令的输出纹理作为 renderPass 的输入纹理
    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;
    if(renderPassDescriptor == nil) return;
    
    vector_float4 quadVertices[6] = {
        ///                                    几何坐标                     纹理坐标
        simd_make_float4(_viewportSize.x * 0.25, _viewportSize.y * 0.25,   0,  0),
        simd_make_float4(_viewportSize.x * 0.25, _viewportSize.y * 0.75,   0, 1.0),
        simd_make_float4(_viewportSize.x * 0.75, _viewportSize.y * 0.75, 1.0, 1.0),
        
        simd_make_float4(_viewportSize.x * 0.25, _viewportSize.y * 0.25,   0,  0),
        simd_make_float4(_viewportSize.x * 0.75, _viewportSize.y * 0.25, 1.0,  0),
        simd_make_float4(_viewportSize.x * 0.75, _viewportSize.y * 0.75, 1.0, 1.0),
    };
    id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    renderEncoder.label = @"MyRenderEncoder";
    [renderEncoder setViewport:(MTLViewport){0.0, 0.0, _viewportSize.x, _viewportSize.y, -1.0, 1.0 }];
    [renderEncoder setRenderPipelineState:_renderPipelineState];
    [renderEncoder setVertexBytes:quadVertices length:sizeof(quadVertices) atIndex:VertexInputIndexVertices];
    [renderEncoder setVertexBytes:&_viewportSize length:sizeof(_viewportSize) atIndex:VertexInputIndexViewportSize];
    [renderEncoder setFragmentTexture:_outputTexture atIndex:TextureIndexOutput];
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
    [renderEncoder endEncoding];

    // Schedule a present once the framebuffer is complete using the current drawable.
    [commandBuffer presentDrawable:view.currentDrawable];
    [commandBuffer commit];
}

@end
