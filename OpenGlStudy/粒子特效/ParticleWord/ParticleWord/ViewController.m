//
//  ViewController.m
//  ParticleWord
//
//  Created by i7y on 2022/2/10.
//

#import "ViewController.h"
#import "TextRender.h"

@interface ViewController ()
@property (nonatomic ,strong) TextRender *render;
@property (nonatomic ,strong) MTKView *mtkView;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    _render = [[TextRender alloc] initWithMTKView:self.mtkView];
    [_render mtkView:self.mtkView drawableSizeWillChange:self.mtkView.drawableSize];
}

- (UIView *)view {
    return self.mtkView;
}

- (MTKView *)mtkView {
    if (_mtkView == nil) {
        _mtkView = [[MTKView alloc] init];
    }
    return _mtkView;
}

@end
