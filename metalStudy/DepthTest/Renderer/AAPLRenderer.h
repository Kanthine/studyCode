@import MetalKit;

@interface AAPLRenderer : NSObject<MTKViewDelegate>

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView;

@property float topVertexDepth;
@property float leftVertexDepth;
@property float rightVertexDepth;

@end
