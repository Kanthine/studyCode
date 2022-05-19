//
//  MetalView.m
//  HelloTriangle-iOS
//
//  Created by i7y on 2022/4/11.
//  Copyright © 2022 Apple. All rights reserved.
//

#import "MetalView.h"
#import "AAPLShaderTypes.h"

@interface MetalView()

{
    id<MTLDevice> _device;

    // The render pipeline generated from the vertex and fragment shaders in the .metal shader file.
    id<MTLRenderPipelineState> _pipelineState;

    // The command queue used to pass commands to the device.
    id<MTLCommandQueue> _commandQueue;

    // The current size of the view, used as an input to the vertex shader.
    vector_uint2 _viewportSize;
    CADisplayLink *_timer;
    
    MTLRenderPassDescriptor *_passDescriptor;
}

@end

@implementation MetalView

+ (id) layerClass {
    return [CAMetalLayer class];
}


-(instancetype)initWithCoder:(NSCoder *)aDecoder {
    if( self = [super initWithCoder:aDecoder] ){
        _device = MTLCreateSystemDefaultDevice();
        _commandQueue = [_device newCommandQueue];
        self.metalLayer.device = _device;
        self.metalLayer.framebufferOnly = YES;
        self.metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        
        
        NSError *error;
        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
        id<MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
        id<MTLFunction> fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentShader"];

        // Configure a pipeline descriptor that is used to create a pipeline state.
        MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"Simple Pipeline";
        pipelineStateDescriptor.vertexFunction = vertexFunction;
        pipelineStateDescriptor.fragmentFunction = fragmentFunction;
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = self.metalLayer.pixelFormat;

        _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor
                                                                 error:&error];
        NSAssert(_pipelineState, @"Failed to create pipeline state: %@", error);

        _commandQueue = [_device newCommandQueue];
        
        _passDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
        _passDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
        _passDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
        _passDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 0, 0, 0);
        
        _timer = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawClick)];
        [_timer addToRunLoop:NSRunLoop.mainRunLoop forMode:NSDefaultRunLoopMode];
    }
    return self;
}

-(CAMetalLayer*) metalLayer {
    return (CAMetalLayer*) self.layer;
}

-(void)drawClick {
    id<CAMetalDrawable> drawable = [self.metalLayer nextDrawable];
    id<MTLTexture> texture = drawable.texture;
    _passDescriptor.colorAttachments[0].texture = texture;
    _viewportSize.x = self.metalLayer.drawableSize.width;
    _viewportSize.y = self.metalLayer.drawableSize.height;
    
    
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";

    id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:_passDescriptor];
    renderEncoder.label = @"MyRenderEncoder";

    
    
    static const AAPLVertex triangleVertices[] =
    {
        // 2D positions,    RGBA colors
        { {  250,  -250 }, { 1, 0, 0, 1 } },
        { { -250,  -250 }, { 0, 1, 0, 1 } },
        { {    0,   250 }, { 0, 0, 1, 1 } },
    };


    // Set the region of the drawable to draw into.
    [renderEncoder setViewport:(MTLViewport){0.0, 0.0, _viewportSize.x, _viewportSize.y, 0.0, 1.0 }];
    
    [renderEncoder setRenderPipelineState:_pipelineState];

    // Pass in the parameter data.
    [renderEncoder setVertexBytes:triangleVertices
                           length:sizeof(triangleVertices)
                          atIndex:AAPLVertexInputIndexVertices];
    
    [renderEncoder setVertexBytes:&_viewportSize
                           length:sizeof(_viewportSize)
                          atIndex:AAPLVertexInputIndexViewportSize];

    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];

    [renderEncoder endEncoding];

    
    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];
}



@end
