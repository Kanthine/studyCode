#import "AAPLRenderer.h"
#import "AAPLShaderTypes.h"
#import "AAPLConfig.h"

#if CREATE_DEPTH_BUFFER
static const MTLPixelFormat AAPLDepthPixelFormat = MTLPixelFormatDepth32Float;
#endif

@implementation AAPLRenderer {
    id <MTLDevice>              _device;
    id <MTLCommandQueue>        _commandQueue;
    id <MTLRenderPipelineState> _pipelineState;
    id <MTLBuffer>              _vertices;
    id <MTLTexture>             _depthTarget;

    /// renderPass，创建一个渲染命令编码器来绘制目标纹理 drawable
    MTLRenderPassDescriptor *_drawableRenderDescriptor;

    vector_float2 _viewportSize;
    
    NSUInteger _frameNum;
}

- (nonnull instancetype)initWithMetalDevice:(nonnull id<MTLDevice>)device
                        drawablePixelFormat:(MTLPixelFormat)drawabklePixelFormat {
    self = [super init];
    if (self) {
        _frameNum = 0;

        _device = device;

        _commandQueue = [_device newCommandQueue];

        _drawableRenderDescriptor = [MTLRenderPassDescriptor new];
        _drawableRenderDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
        _drawableRenderDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 1, 1, 1);
        _drawableRenderDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
        
#if CREATE_DEPTH_BUFFER /// 开启深度测试
        _drawableRenderDescriptor.depthAttachment.loadAction = MTLLoadActionClear;
        _drawableRenderDescriptor.depthAttachment.clearDepth = 1.0;
        _drawableRenderDescriptor.depthAttachment.storeAction = MTLStoreActionDontCare;
#endif

        {
            id<MTLLibrary> shaderLib = [_device newDefaultLibrary];
            if(!shaderLib) {
                NSLog(@" ERROR: Couldnt create a default shader library");
                return nil;
            }
            id <MTLFunction> vertexProgram = [shaderLib newFunctionWithName:@"vertexShader"];
            if(!vertexProgram) {
                NSLog(@">> ERROR: Couldn't load vertex function from default library");
                return nil;
            }

            id <MTLFunction> fragmentProgram = [shaderLib newFunctionWithName:@"fragmentShader"];
            if(!fragmentProgram) {
                NSLog(@" ERROR: Couldn't load fragment function from default library");
                return nil;
            }

            /// 创建一个 buffer 存储顶点与颜色
            static const AAPLVertex quadVertices[] = {
                // Pixel positions, Color coordinates
                { {  250,  -250 },  { 1.f, 0.f, 0.f } },
                { { -250,  -250 },  { 0.f, 1.f, 0.f } },
                { { -250,   250 },  { 0.f, 0.f, 1.f } },

                { {  250,  -250 },  { 1.f, 0.f, 0.f } },
                { { -250,   250 },  { 0.f, 0.f, 1.f } },
                { {  250,   250 },  { 1.f, 0.f, 1.f } },
            };
            _vertices = [_device newBufferWithBytes:quadVertices length:sizeof(quadVertices) options:MTLResourceStorageModeShared];
            _vertices.label = @"Quad";

            /// 创建渲染管线
            MTLRenderPipelineDescriptor *pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
            pipelineDescriptor.label                           = @"MyPipeline";
            pipelineDescriptor.vertexFunction                  = vertexProgram;
            pipelineDescriptor.fragmentFunction                = fragmentProgram;
            pipelineDescriptor.colorAttachments[0].pixelFormat = drawabklePixelFormat;

#if CREATE_DEPTH_BUFFER
            /// 深度测试的像素格式： 32位浮点型
            pipelineDescriptor.depthAttachmentPixelFormat      = AAPLDepthPixelFormat;
#endif

            NSError *error;
            _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineDescriptor
                                                                     error:&error];
            if(!_pipelineState) {
                NSLog(@"ERROR: Failed aquiring pipeline state: %@", error);
                return nil;
            }
        }
    }
    return self;
}

- (void)renderToMetalLayer:(nonnull CAMetalLayer*)metalLayer {
    _frameNum++;
    
    id<CAMetalDrawable> currentDrawable = [metalLayer nextDrawable];
    if(!currentDrawable) return;  /// 如果获取不到 drawable，则跳过此帧
    
    _drawableRenderDescriptor.colorAttachments[0].texture = currentDrawable.texture;
    
    id <MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:_drawableRenderDescriptor];
    [renderEncoder setRenderPipelineState:_pipelineState];
    [renderEncoder setVertexBuffer:_vertices offset:0 atIndex:AAPLVertexInputIndexVertices ];

    {
        AAPLUniforms uniforms;
#if ANIMATION_RENDERING
        uniforms.scale = 0.5 + (1.0 + 0.5 * sin(_frameNum * 0.1));
#else
        uniforms.scale = 1.0;
#endif
        uniforms.viewportSize = _viewportSize;
        [renderEncoder setVertexBytes:&uniforms length:sizeof(uniforms) atIndex:AAPLVertexInputIndexUniforms];
    }
    
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
    [renderEncoder endEncoding];
    [commandBuffer presentDrawable:currentDrawable];
    [commandBuffer commit];
}

/// 方向、大小改变时调用
- (void)drawableResize:(CGSize)drawableSize {
    _viewportSize.x = drawableSize.width;
    _viewportSize.y = drawableSize.height;
    
#if CREATE_DEPTH_BUFFER
    MTLTextureDescriptor *depthTargetDescriptor = [MTLTextureDescriptor new];
    depthTargetDescriptor.width       = drawableSize.width;
    depthTargetDescriptor.height      = drawableSize.height;
    depthTargetDescriptor.pixelFormat = AAPLDepthPixelFormat;
    depthTargetDescriptor.storageMode = MTLStorageModePrivate;
    depthTargetDescriptor.usage       = MTLTextureUsageRenderTarget;
    
    _depthTarget = [_device newTextureWithDescriptor:depthTargetDescriptor];
    
    _drawableRenderDescriptor.depthAttachment.texture = _depthTarget;
#endif
}

@end
