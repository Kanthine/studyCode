@import simd;
@import MetalKit;

#import "AAPLRenderer.h"
#import "AAPLShaderTypes.h"

@implementation AAPLRenderer {
    id<MTLDevice> _device;
    id<MTLRenderPipelineState> _pipelineState;
    id<MTLCommandQueue> _commandQueue;
    vector_float2 _viewportSize;

    id<MTLTexture> _point_1_Map;
    id<MTLTexture> _point_2_Map;
    id<MTLTexture> _point_3_Map;
}

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView {
    self = [super init];
    if(self) {
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
        
        [self loadTexture];
    }
    return self;
}

- (void)loadTexture {
    MTKTextureLoader *textureLoader = [[MTKTextureLoader alloc] initWithDevice:_device];
    NSDictionary *textureLoaderOptions = @{
        MTKTextureLoaderOptionTextureUsage       : @(MTLTextureUsageShaderRead),
        MTKTextureLoaderOptionTextureStorageMode : @(MTLStorageModePrivate),
    };
    NSError *error = nil;
    
    _point_1_Map = [textureLoader newTextureWithName:@"FairyMap" scaleFactor:1.0 bundle:nil options:textureLoaderOptions
                                               error:&error];
    _point_1_Map.label = @"FairyMap";

    _point_2_Map = [textureLoader newTextureWithName:@"PointSprite" scaleFactor:1.0 bundle:nil options:textureLoaderOptions
                                               error:&error];
    _point_2_Map.label = @"PointSprite";
    
    _point_3_Map = [textureLoader newTextureWithName:@"rectangle" scaleFactor:1.0 bundle:nil options:textureLoaderOptions
                                               error:&error];
    _point_3_Map.label = @"rectangle";
    NSAssert(_point_1_Map && _point_2_Map && _point_3_Map, @"Could not load fairy texture: %@", error);
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    
    float sacle_1 = _point_1_Map.width / (float)(_point_1_Map.height);
    float sacle_2 = _point_2_Map.width / (float)(_point_2_Map.height);
    float sacle_3 = _point_3_Map.width / (float)(_point_3_Map.height);
    vector_float4 points[] = {
        { 0.1, 0.1, sacle_1, 1 },
        { 0.5, 0.5, sacle_3, 3 },
        { 0.9, 0.9, sacle_2, 2 },
    };
    
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";
    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;

    if(renderPassDescriptor != nil) {
        id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"MyRenderEncoder";
        [renderEncoder setRenderPipelineState:_pipelineState];
        [renderEncoder setViewport:(MTLViewport){0.0, 0.0, _viewportSize.x, _viewportSize.y, 0.0, 1.0 }];
        [renderEncoder setVertexBytes:points length:sizeof(points) atIndex:VertexInputPoint];
        [renderEncoder setFragmentTexture:_point_1_Map atIndex:FragmentInputTexture_1];
        [renderEncoder setFragmentTexture:_point_2_Map atIndex:FragmentInputTexture_2];
        [renderEncoder setFragmentTexture:_point_3_Map atIndex:FragmentInputTexture_3];
        [renderEncoder drawPrimitives:MTLPrimitiveTypePoint vertexStart:0 vertexCount:3];
        [renderEncoder endEncoding];
        [commandBuffer presentDrawable:view.currentDrawable];
    }
    [commandBuffer commit];
}

@end
