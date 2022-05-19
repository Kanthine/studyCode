#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>
#import "AAPLConfig.h"

#if TARGET_IOS || TARGET_TVOS
@import UIKit;
#else
@import AppKit;
#endif

@protocol AAPLViewDelegate <NSObject>

/// 视图大小、方向改变时回调
- (void)drawableResize:(CGSize)size;

/// 需要绘制时，回调
- (void)renderToMetalLayer:(nonnull CAMetalLayer *)metalLayer;

@end

#if TARGET_IOS || TARGET_TVOS
@interface AAPLView : UIView <CALayerDelegate>
#else
@interface AAPLView : NSView <CALayerDelegate>
#endif

@property (nonatomic, nonnull, readonly) CAMetalLayer *metalLayer;

@property (nonatomic, getter=isPaused) BOOL paused;

@property (nonatomic, nullable) id<AAPLViewDelegate> delegate;

- (void)initCommon;

#if AUTOMATICALLY_RESIZE
- (void)resizeDrawable:(CGFloat)scaleFactor;
#endif

#if ANIMATION_RENDERING
- (void)stopRenderLoop;
#endif

- (void)render;

@end
