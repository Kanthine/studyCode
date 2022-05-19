//
//  ViewController.m
//  PointSprite
//
//  Created by 苏莫离 on 2019/10/17.
//

#import "ViewController.h"
#import "GLView.h"

@interface ViewController ()
@property (nonatomic, strong) GLView *glView;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = UIColor.whiteColor;
    [self.view addSubview:self.glView];
}

- (GLView *)glView{
    if (!_glView) {
        _glView = [[GLView alloc] initWithFrame:self.view.bounds];
    }
    return _glView;
}

@end
