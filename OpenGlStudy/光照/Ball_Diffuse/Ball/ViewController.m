//
//  ViewController.m
//  Ball
//
//  Created by 苏莫离 on 2019/10/17.
//

#import "ViewController.h"
#import "GLView.h"

@interface ViewController ()
@property (nonatomic, strong) UISlider *slider;
@property (nonatomic, strong) GLView *glView;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = UIColor.whiteColor;
}

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    [self glView];
}

- (void)sliderValueChangeClick:(UISlider *)slider {
    self.glView.lightOffset = slider.value;
}

- (GLView *)glView{
    if (!_glView) {
        _glView = [[GLView alloc] initWithFrame:self.view.bounds];
        [self.view addSubview:_glView];
        [self.view addSubview:self.slider];
    }
    return _glView;
}

- (UISlider *)slider {
    if (_slider == nil) {
        UISlider *slider = [[UISlider alloc] initWithFrame:CGRectMake(20, 60, CGRectGetWidth(UIScreen.mainScreen.bounds) - 40, 20)];
        slider.maximumValue = 1;
        slider.minimumValue = 0;
        [slider addTarget:self action:@selector(sliderValueChangeClick:) forControlEvents:UIControlEventValueChanged];
        _slider = slider;
    }
    return _slider;
}

@end
