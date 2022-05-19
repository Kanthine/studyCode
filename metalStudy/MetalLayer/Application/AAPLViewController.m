#import "AAPLViewController.h"
#if TARGET_IOS || TARGET_TVOS
#import "AAPLUIView.h"
#else
#import "AAPLNSView.h"
#endif
#import "AAPLRenderer.h"

#import <QuartzCore/CAMetalLayer.h>

@implementation AAPLViewController {
    AAPLRenderer *_renderer;
}

- (void)viewDidLoad {
    [super viewDidLoad];

    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    AAPLView *view = (AAPLView *)self.view;

    //设置 layer.device，以便 layer 创建 drawable
    view.metalLayer.device = device;

    /// 当 view 大小、方向改变时、当需要视图绘制时，调用 delegate 方法
    view.delegate = self;
    view.metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
    _renderer = [[AAPLRenderer alloc] initWithMetalDevice:device drawablePixelFormat:view.metalLayer.pixelFormat];
}

- (void)drawableResize:(CGSize)size {
    [_renderer drawableResize:size];
}

- (void)renderToMetalLayer:(nonnull CAMetalLayer *)layer {
    [_renderer renderToMetalLayer:layer];
}

@end
