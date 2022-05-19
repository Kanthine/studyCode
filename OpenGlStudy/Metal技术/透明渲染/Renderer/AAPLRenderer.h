@import MetalKit;

enum AAPLTransparencyMethod {
    AAPLMethod4LayerOrderIndependent,
};

static const char* __nonnull  s_transparencyMethodNames[] = {
    "4 Layer Order Independant Transparency",
    "2 Layer Order Independant Transparency",
    "Unordered Alpha Blending",
};


@interface AAPLRenderer : NSObject <MTKViewDelegate>

@property (nonatomic) enum AAPLTransparencyMethod transparencyMethod;

-(nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)view;

@end



// https://zhuanlan.zhihu.com/p/92841297
