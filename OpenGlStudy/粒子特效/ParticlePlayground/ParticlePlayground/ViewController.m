//
//  ViewController.m
//  ParticlePlayground
//
//  Created by 苏莫离 on 2019/10/17.
//

#import "ViewController.h"
#import "GLView.h"


static const double kPlatformSliderHeight = 30.0;
static const double kPlatformSliderWeight = 300.0;

@interface PlatformSlider ()
@property (nonatomic, strong) UILabel *tipLabel;
@property (nonatomic, strong) UISlider *slider;
@property (nonatomic, strong) UILabel *valueLabel;
@end


@implementation PlatformSlider

+ (instancetype)item:(NSString *)item
            minValue:(float)minValue
            maxValue:(float)maxValue
        currentValue:(float)currentValue {
    return [[PlatformSlider alloc] initWithItem:item
                                       minValue:minValue
                                       maxValue:maxValue
                                   currentValue:currentValue];
}


- (instancetype)initWithItem:(NSString *)item
                    minValue:(float)minValue
                    maxValue:(float)maxValue
                currentValue:(float)currentValue {
    self = [super init];
    
    if (self) {
        _tipLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 50, kPlatformSliderHeight)];
        _tipLabel.text = item;
        [self addSubview:_tipLabel];

        _slider = [[UISlider alloc] initWithFrame:CGRectMake(50, 0, 200, kPlatformSliderHeight)];
        _slider.value = currentValue;
        _slider.minimumValue = minValue;
        _slider.maximumValue = maxValue;
        [_slider addTarget:self action:@selector(sliderValueChange) forControlEvents:UIControlEventValueChanged];
        [self addSubview:_slider];
    
        _valueLabel = [[UILabel alloc] initWithFrame:CGRectMake(250, 0, 50, kPlatformSliderHeight)];
        _valueLabel.text = [NSString stringWithFormat:@"%.2f",currentValue];
        [self addSubview:_valueLabel];
    }
    return self;
}

- (void)sliderValueChange {
    if (self.sliderChangeHandle) {
        self.valueLabel.text = [NSString stringWithFormat:@"%.2f",self.slider.value];
        self.sliderChangeHandle(self.slider.value);
    }
}

@end

@implementation MatrixSetView

+ (instancetype)viewWithItems:(NSArray<PlatformSlider *> *)items {
    MatrixSetView *view = [[MatrixSetView alloc] initWithFrame:CGRectMake(10, 50, kPlatformSliderWeight, kPlatformSliderHeight * items.count)];
    for (int i = 0; i < items.count; i++) {
        items[i].frame = CGRectMake(0, kPlatformSliderHeight * i, 300, kPlatformSliderHeight);
        [view addSubview:items[i]];
    }
    return view;
}

@end











@interface ViewController ()
@property (nonatomic, strong) GLView *glView;
@property (nonatomic, strong) MatrixSetView *setView;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = UIColor.whiteColor;
    [self glView];
//    [self setSlideView];
}

- (void)setSlideView {
    PlatformSlider *zNearSlider = [PlatformSlider item:@"zNear" minValue:0 maxValue:20 currentValue:self.glView.zNear];
    PlatformSlider *zFarSlider = [PlatformSlider item:@"zFar" minValue:10 maxValue:1000 currentValue:self.glView.zFar];
    PlatformSlider *fovSlider = [PlatformSlider item:@"fov" minValue:0 maxValue:M_PI currentValue:self.glView.fov];
    PlatformSlider *zPosSlider = [PlatformSlider item:@"zCamera" minValue:-10 maxValue:10 currentValue:self.glView.zPos];
    _setView = [MatrixSetView viewWithItems:@[zNearSlider,zFarSlider,fovSlider,zPosSlider]];
    [self.view addSubview:_setView];
    
    __weak typeof(self) weakSelf = self;
    zNearSlider.sliderChangeHandle = ^(double value) {
        __strong typeof(weakSelf) strongSelf = weakSelf;
        strongSelf.glView.zNear = value;
    };
    zFarSlider.sliderChangeHandle = ^(double value) {
        __strong typeof(weakSelf) strongSelf = weakSelf;
        strongSelf.glView.zFar = value;
    };
    fovSlider.sliderChangeHandle = ^(double value) {
        __strong typeof(weakSelf) strongSelf = weakSelf;
        strongSelf.glView.fov = value;
    };
    zPosSlider.sliderChangeHandle = ^(double value) {
        __strong typeof(weakSelf) strongSelf = weakSelf;
        strongSelf.glView.zPos= value;
    };
}


- (GLView *)glView{
    if (!_glView) {
        _glView = [[GLView alloc] initWithFrame:self.view.bounds];
        
        _glView.zNear = 0.3;
        _glView.zFar = 300.0;
        _glView.zPos = -1.5;
        _glView.fov = 1.0;
        [self.view addSubview:_glView];
    }
    return _glView;
}

@end
