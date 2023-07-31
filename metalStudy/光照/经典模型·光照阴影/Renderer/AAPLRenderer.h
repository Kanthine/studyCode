@import MetalKit;

@interface AAPLRenderer : NSObject<MTKViewDelegate>

- (nonnull instancetype)initWithMTKView:(nonnull MTKView *)mtkView;

@end
