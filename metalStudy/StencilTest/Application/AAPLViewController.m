#import "AAPLViewController.h"
#import "MainRender.h"

@implementation AAPLViewController {
    MTKView *_view;
    MainRender *_renderer;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    _view = (MTKView *)self.view;
    _view.device = MTLCreateSystemDefaultDevice();
    NSAssert(_view.device, @"Metal is not supported on this device");
    _renderer = [[MainRender alloc] initWithMetalKitView:_view];
    NSAssert(_renderer, @"Renderer failed initialization");
    [_renderer mtkView:_view drawableSizeWillChange:_view.drawableSize];
    _view.delegate = _renderer;
}

@end
