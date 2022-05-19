@import MetalKit;

NS_ASSUME_NONNULL_BEGIN

@interface MaskRender : NSObject

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView;

- (void)drawMaskWithRender:(id<MTLRenderCommandEncoder>)renderEncoder
              viewportSize:(vector_float2)viewportSize;

@end

NS_ASSUME_NONNULL_END
