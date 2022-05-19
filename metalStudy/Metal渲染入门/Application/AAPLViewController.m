#import "AAPLViewController.h"
#import "AAPLRenderer.h"

@implementation AAPLViewController {
    MTKView *_view;
    AAPLRenderer *_renderer;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    _view = (MTKView *)self.view;
    _view.enableSetNeedsDisplay = YES;
    _view.device = MTLCreateSystemDefaultDevice();
    _view.clearColor = MTLClearColorMake(1.0, 0.5, 0.5, 1.0);
    _renderer = [[AAPLRenderer alloc] initWithMetalKitView:_view];
    if(!_renderer) {
        NSLog(@"Renderer initialization failed");
        return;
    }
    [_renderer mtkView:_view drawableSizeWillChange:_view.drawableSize];
    _view.delegate = _renderer;
}

@end
