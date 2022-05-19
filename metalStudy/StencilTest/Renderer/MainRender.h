@import MetalKit;

NS_ASSUME_NONNULL_BEGIN

@interface MainRender : NSObject <MTKViewDelegate>
- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView;
@end

NS_ASSUME_NONNULL_END
