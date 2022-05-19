//
//  TextRender.m
//  Graphics
//
//  Created by 苏沫离 on 2020/9/25.
//

#import "TextRender.h"
#import "TextShaderTypes.h"
#import "WordFactory.h"

@import simd;

@interface TextRender ()

{
    id<MTLDevice> _device;
    id<MTLRenderPipelineState> _pipelineState;
    id<MTLCommandQueue> _commandQueue;
    vector_float2 _viewportSize;
    id<MTLBuffer> _vertexsBuffer;
}

@property (nonatomic, assign) int pointCount;

@end


@implementation TextRender

- (instancetype)initWithMTKView:(nonnull MTKView *)mtkView {
    self = [super init];
    if (self) {
        mtkView.device = MTLCreateSystemDefaultDevice();
        NSAssert(mtkView.device, @"Metal is not supported on this device");
        _device = mtkView.device;
        mtkView.delegate = self;
        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
        id<MTLFunction> vertexFunc = [defaultLibrary newFunctionWithName:@"vertexTextShader"];
        id<MTLFunction> framentFunc = [defaultLibrary newFunctionWithName:@"fragmentTextShader"];
        
        MTLRenderPipelineDescriptor *descriptor = [[MTLRenderPipelineDescriptor alloc] init];
        descriptor.label = @"文本·渲染管道";
        descriptor.vertexFunction = vertexFunc;
        descriptor.fragmentFunction = framentFunc;
        descriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
        
        NSError *error;
        _pipelineState = [_device newRenderPipelineStateWithDescriptor:descriptor error:&error];
        NSAssert(_pipelineState, @"Failed to create pipeline state: %@", error);
        
        _commandQueue = [_device newCommandQueue];
        
        self.word = @"文字粒子沙化";
    }
    return self;
}

#pragma mark - MTKViewDelegate

- (void)drawInMTKView:(nonnull MTKView *)view {
    
    if (_pointCount < 1) return;
    
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"文本·命令缓冲池";
    
    MTLRenderPassDescriptor *descriptor = view.currentRenderPassDescriptor;
    if (descriptor != nil) {
        id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:descriptor];
        renderEncoder.label = @"文本·命令编码器";
        
        [renderEncoder setViewport:(MTLViewport){0,0,_viewportSize.x,_viewportSize.y,0,1.0}];
        [renderEncoder setRenderPipelineState:_pipelineState];
        
        [renderEncoder setVertexBuffer:_vertexsBuffer offset:0 atIndex:ShaderParamTypeVertices];
        [renderEncoder setVertexBytes:&_viewportSize length:sizeof(_viewportSize) atIndex:ShaderParamTypeViewport];
        
        [renderEncoder drawPrimitives:MTLPrimitiveTypeLineStrip vertexStart:0 vertexCount:_pointCount];
        [renderEncoder endEncoding];
        [commandBuffer presentDrawable:view.currentDrawable];
    }
    [commandBuffer commit];
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
}

- (void)setWord:(NSString *)word {
    NSMutableAttributedString *string = [[NSMutableAttributedString alloc] initWithString:word attributes:@{NSFontAttributeName:[UIFont systemFontOfSize:100 * UIScreen.mainScreen.scale]}];
    CGMutablePathRef path = getPathWithAttributedString((__bridge CFAttributedStringRef _Nullable)string);
    int count = 0;
    vector_float2 *points = getPointsByCGPath(path, &count);
    self.pointCount = count;
    
    _vertexsBuffer = [_device newBufferWithBytes:points length:sizeof(vector_float2) * _pointCount  options:MTLResourceStorageModeShared];
}

@end
