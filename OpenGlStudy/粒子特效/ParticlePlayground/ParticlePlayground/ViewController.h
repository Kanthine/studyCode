//
//  ViewController.h
//  ParticlePlayground
//
//  Created by 苏莫离 on 2019/10/17.
//

#import <UIKit/UIKit.h>

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



@interface ViewController : UIViewController


@end

