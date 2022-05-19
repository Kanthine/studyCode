#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

@interface AAPLRenderer : NSObject

- (nonnull instancetype)initWithMetalDevice:(nonnull id<MTLDevice>)device
                        drawablePixelFormat:(MTLPixelFormat)drawabklePixelFormat;

- (void)renderToMetalLayer:(nonnull CAMetalLayer *)metalLayer;

- (void)drawableResize:(CGSize)drawableSize;

@end
