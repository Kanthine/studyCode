@import simd;
@import MetalKit;

#import "AAPLRenderer.h"
#import "AAPLShaderTypes.h"
#import "DMSDFGen.h"


const NSString *kIconName = @"test2";
@implementation AAPLRenderer {
    id<MTLDevice> _device;
    id<MTLRenderPipelineState> _renderPipelineState;
    id<MTLCommandQueue> _commandQueue;
    id<MTLTexture> _texture;
    id<MTLTexture> _sdfTexture;
    vector_float2 _viewportSize;
    float _timer;
}

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView {
    self = [super init];
    if(self) {
        NSError *error = NULL;
        mtkView.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
        _device = mtkView.device;

        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];

        /// 渲染着色器
        id<MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
        id<MTLFunction> fragmentFunction = [defaultLibrary newFunctionWithName:@"sdfFragment"];
        fragmentFunction = [defaultLibrary newFunctionWithName:@"flashFragment"];
        fragmentFunction = [defaultLibrary newFunctionWithName:@"fireFragment"];
        MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"Simple Render Pipeline";
        pipelineStateDescriptor.vertexFunction = vertexFunction;
        pipelineStateDescriptor.fragmentFunction = fragmentFunction;
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
        _renderPipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor
                                                                 error:&error];
        NSAssert(_renderPipelineState, @"Failed to create render pipeline state: %@", error);
        
        _commandQueue = [_device newCommandQueue];
        _timer = 0.0;
        [self testTexture];
    }

    return self;
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    _timer += 0.016;

    /// 为每一帧渲染创建一个命令缓冲区
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";

    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;
    if(renderPassDescriptor == nil) return;
    
    float width = _viewportSize.x * 0.8;
    float height = _texture.height * 1.0 / _texture.width * width;
    float x = (_viewportSize.x - width) / 2.0;
    float y = (_viewportSize.y - height) / 2.0;
    
    vector_float4 quadVertices[6] = {
        simd_make_float4(        x,          y,   0,  0),
        simd_make_float4(        x, y + height,   0, 1.0),
        simd_make_float4(x + width, y + height, 1.0, 1.0),
        
        simd_make_float4(        x,          y,   0,  0),
        simd_make_float4(x + width,          y, 1.0,  0),
        simd_make_float4(x + width, y + height, 1.0, 1.0),
    };
    
    id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    renderEncoder.label = @"MyRenderEncoder";
    [renderEncoder setViewport:(MTLViewport){0.0, 0.0, _viewportSize.x, _viewportSize.y, -1.0, 1.0 }];
    [renderEncoder setRenderPipelineState:_renderPipelineState];
    [renderEncoder setVertexBytes:quadVertices length:sizeof(quadVertices) atIndex:VertexInputIndexVertices];
    [renderEncoder setVertexBytes:&_viewportSize length:sizeof(_viewportSize) atIndex:VertexInputIndexViewportSize];
    [renderEncoder setFragmentTexture:_texture atIndex:VertexInputIndexTexture];
    [renderEncoder setFragmentTexture:_sdfTexture atIndex:VertexInputIndexTextureSDF];
    [renderEncoder setFragmentBytes:&_timer length:sizeof(float) atIndex:VertexInputIndexTimer];
    [renderEncoder setFragmentBytes:&_viewportSize length:sizeof(_viewportSize) atIndex:VertexInputIndexViewportSize];
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
    [renderEncoder endEncoding];

    // Schedule a present once the framebuffer is complete using the current drawable.
    [commandBuffer presentDrawable:view.currentDrawable];
    [commandBuffer commit];
}

@end




@implementation AAPLRenderer (Texture)

- (void)testTexture {
//    [self wordTexture];
    
    [self loadAssets];
    [self loadAssetsSDF];
}


- (void)loadAssetsSDF {
#if defined(TARGET_IOS) || defined(TARGET_TVOS)
    UIImage *image = [UIImage imageNamed:kIconName];
    CGImageRef imageRef = image.CGImage;
#else
    NSImage *image = [NSImage imageNamed:kIconName];
    NSData *data = [image TIFFRepresentation];
    CGImageSourceRef source = CGImageSourceCreateWithData(CFBridgingRetain(data), NULL);
    CGImageRef imageRef = CGImageSourceCreateImageAtIndex(source, 0, NULL);
#endif

    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceGray();
    if(!colorSpace) {
        return ;
    }

    size_t width = CGImageGetWidth(imageRef);
    size_t height = CGImageGetHeight(imageRef);
    size_t bytesPerRow = width;
    size_t bufferLength = bytesPerRow * height;
    unsigned char *pixelInfo = malloc(bufferLength * sizeof(unsigned char));
    if(!pixelInfo) {
        CGColorSpaceRelease(colorSpace);
        return ;
    }

    CGContextRef context = CGBitmapContextCreate(pixelInfo,
                                                 width,
                                                 height,
                                                 8,
                                                 width,
                                                 colorSpace,
                                                 kCGBitmapByteOrderDefault);
    if(!context) {
        CGColorSpaceRelease(colorSpace);
        free(pixelInfo);
        return ;
    }
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
    CGContextRelease(context);
    CGColorSpaceRelease(colorSpace);
    
    unsigned char *outInfo = malloc(bufferLength * sizeof(unsigned char));
    [DMSDFGen sdfBuildDistanceFieldWith:outInfo radius:30.0 srcImg:pixelInfo width:(int)width height:(int)height];
    
    NSError *error = NULL;
    MTLTextureDescriptor *textureDescriptor = [[MTLTextureDescriptor alloc] init];
    textureDescriptor.textureType = MTLTextureType2D;
    textureDescriptor.pixelFormat = MTLPixelFormatA8Unorm;
    textureDescriptor.width = width;
    textureDescriptor.height = height;
    textureDescriptor.usage = MTLTextureUsageShaderRead;
    _sdfTexture = [_device newTextureWithDescriptor:textureDescriptor];
    NSAssert(_sdfTexture, @"Could not load texture: %@", error);
    _sdfTexture.label = @"sdf Texture";
    MTLRegion mtlRegion = MTLRegionMake3D(0, 0, 0, width, height, 1);
    [_sdfTexture replaceRegion:mtlRegion mipmapLevel:0 withBytes:outInfo bytesPerRow:width * 1];
}

- (void)loadAssets {
#if defined(TARGET_IOS) || defined(TARGET_TVOS)
    UIImage *image = [UIImage imageNamed:kIconName];
    CGImageRef imageRef = image.CGImage;
#else
    NSImage *image = [NSImage imageNamed:kIconName];
    NSData *data = [image TIFFRepresentation];
    CGImageSourceRef source = CGImageSourceCreateWithData(CFBridgingRetain(data), NULL);
    CGImageRef imageRef = CGImageSourceCreateImageAtIndex(source, 0, NULL);
#endif

    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    if(!colorSpace) {
        return ;
    }

    size_t width = CGImageGetWidth(imageRef);
    size_t height = CGImageGetHeight(imageRef);
    size_t bytesPerRow = width * 4;
    size_t bufferLength = bytesPerRow * height;
    unsigned char *pixelInfo = malloc(bufferLength * sizeof(unsigned char));
    if(!pixelInfo) {
        CGColorSpaceRelease(colorSpace);
        return ;
    }

    CGContextRef context = CGBitmapContextCreate(pixelInfo,
                                                 width,
                                                 height,
                                                 8,
                                                 bytesPerRow,
                                                 colorSpace,
                                                 kCGImageAlphaPremultipliedLast);    // RGBA
    if(!context) {
        CGColorSpaceRelease(colorSpace);
        free(pixelInfo);
        return ;
    }
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
    CGContextRelease(context);
    CGColorSpaceRelease(colorSpace);
    
    NSError *error = NULL;
    MTLTextureDescriptor *textureDescriptor = [[MTLTextureDescriptor alloc] init];
    textureDescriptor.textureType = MTLTextureType2D;
    textureDescriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
    textureDescriptor.width = width;
    textureDescriptor.height = height;
    textureDescriptor.usage = MTLTextureUsageShaderRead;
    _texture = [_device newTextureWithDescriptor:textureDescriptor];
    NSAssert(_texture, @"Could not load texture: %@", error);
    _texture.label = @"image Texture";
    MTLRegion mtlRegion = MTLRegionMake3D(0, 0, 0, width, height, 1);
    [_texture replaceRegion:mtlRegion mipmapLevel:0 withBytes:pixelInfo bytesPerRow:bytesPerRow];
}

@end
