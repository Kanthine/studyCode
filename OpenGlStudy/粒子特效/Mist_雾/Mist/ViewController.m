//
//  ViewController.m
//  Mist
//
//  Created by 苏莫离 on 2019/10/21.
//

#import "ViewController.h"
#import "GLView.h"


@interface PlatformSlider : UIView
@property (nonatomic, copy) NSString *item;
@property (nonatomic, assign) float minValue;
@property (nonatomic, assign) float maxValue;
@property (nonatomic, assign) float currentValue;
@property (nonatomic, copy) void(^sliderChangeHandle)(double value);

+ (instancetype)item:(NSString *)item
            minValue:(float)minValue
            maxValue:(float)maxValue
        currentValue:(float)currentValue;
@end

@interface MatrixSetView : UIView
+ (instancetype)viewWithItems:(NSArray<PlatformSlider *> *)items;
@end




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
        _slider.minimumValue = minValue;
        _slider.maximumValue = maxValue;
        _slider.value = currentValue;
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
    [self setSlideView];
}

- (void)setSlideView {
    PlatformSlider *zNearSlider = [PlatformSlider item:@"zNear" minValue:0 maxValue:20 currentValue:self.glView.zNear];
    PlatformSlider *zFarSlider = [PlatformSlider item:@"zFar" minValue:10 maxValue:1000 currentValue:self.glView.zFar];
    PlatformSlider *cxSlider = [PlatformSlider item:@"cx" minValue:-100 maxValue:100 currentValue:self.glView.cx];
    PlatformSlider *czSlider = [PlatformSlider item:@"cz" minValue:-100 maxValue:100 currentValue:self.glView.cz];
    PlatformSlider *txSlider = [PlatformSlider item:@"tx" minValue:-100 maxValue:100 currentValue:self.glView.tx];
    PlatformSlider *tzSlider = [PlatformSlider item:@"tz" minValue:-100 maxValue:100 currentValue:self.glView.tz];
    _setView = [MatrixSetView viewWithItems:@[zNearSlider,zFarSlider,cxSlider,czSlider, txSlider, tzSlider]];
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
    cxSlider.sliderChangeHandle = ^(double value) {
        __strong typeof(weakSelf) strongSelf = weakSelf;
        strongSelf.glView.cx = value;
    };
    czSlider.sliderChangeHandle = ^(double value) {
        __strong typeof(weakSelf) strongSelf = weakSelf;
        strongSelf.glView.cz= value;
    };
    txSlider.sliderChangeHandle = ^(double value) {
        __strong typeof(weakSelf) strongSelf = weakSelf;
        strongSelf.glView.tx = value;
    };
    tzSlider.sliderChangeHandle = ^(double value) {
        __strong typeof(weakSelf) strongSelf = weakSelf;
        strongSelf.glView.tz= value;
    };
}


- (GLView *)glView{
    if (!_glView) {
        _glView = [[GLView alloc] initWithFrame:self.view.bounds];
        _glView.zNear = 0.3;
        _glView.zFar = 300.0;
        _glView.cx = 60;
        _glView.cz = 90;
        _glView.tx = 0;
        _glView.tz = 0;
        [self.view addSubview:_glView];
    }
    return _glView;
}

@end
