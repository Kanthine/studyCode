#import "AAPLViewController.h"
#import "AAPLRenderer.h"

@implementation AAPLViewController
{
    MTKView *_view;

    AAPLRenderer *_renderer;
    UITapGestureRecognizer *_tapRecognizer;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    _view = (MTKView *)self.view;
    _view.device = MTLCreateSystemDefaultDevice();
    if(!_view.device) {
        NSLog(@"Metal is not supported on this device");
        self.view = [[UIView alloc] initWithFrame:self.view.frame];
    }

    _renderer = [[AAPLRenderer alloc] initWithMetalKitView:_view];
    if(!_renderer) {
        NSLog(@"Renderer failed initialization");
        return;
    }

    [_renderer mtkView:_view drawableSizeWillChange:_view.drawableSize];

    _view.delegate = _renderer;

    self.mainLabel.text = [NSString stringWithFormat:@"%s", s_transparencyMethodNames[_renderer.transparencyMethod]];
}

@end

