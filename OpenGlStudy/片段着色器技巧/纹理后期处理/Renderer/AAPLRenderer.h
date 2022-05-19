@import MetalKit;

@interface AAPLRenderer : NSObject<MTKViewDelegate>

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView;

@end


/** 绘制流程
 * 1、绘制图形
 * 2、处理图形
 * 3、提交到 draw
 */
