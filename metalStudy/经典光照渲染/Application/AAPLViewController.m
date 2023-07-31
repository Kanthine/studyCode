/*
See LICENSE folder for this sample’s licensing information.

Abstract:
Implementation of the cross-platform view controller
*/

#import "AAPLViewController.h"

#import "AAPLRenderer.h"

@implementation AAPLViewController {
    MTKView *_view;
    AAPLRenderer *_renderer;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Set the view to use the default device
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    NSAssert(device, @"Metal is not supported on this device");
    _view = (MTKView *)self.view;
    _view.device = device;
    _renderer = [[AAPLRenderer alloc] initWithMetalKitView:_view];
    NSAssert(_renderer, @"Renderer failed initialization");
    [_renderer mtkView:_view drawableSizeWillChange:_view.drawableSize];
    _view.delegate = self;
}

- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size
{
    [_renderer mtkView:view drawableSizeWillChange:size];
}

- (void)drawInMTKView:(MTKView *)view
{
    [_renderer drawSceneToView:view];
}

@end
