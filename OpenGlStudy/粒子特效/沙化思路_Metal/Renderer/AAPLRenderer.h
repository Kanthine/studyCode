@import MetalKit;

@interface AAPLRenderer : NSObject
<MTKViewDelegate>

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView;

@property (nonatomic, readonly, nonnull) id <MTLDevice> device;

@property (nonatomic, readonly, nullable, weak) MTKView *view;

@end
