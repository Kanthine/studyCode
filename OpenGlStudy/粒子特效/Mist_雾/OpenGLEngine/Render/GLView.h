//
//  GLView.h
//  Mist
//
//  Created by 苏莫离 on 2019/10/17.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface GLView : UIView

@property (nonatomic, assign) float zNear;
@property (nonatomic, assign) float zFar;
@property (nonatomic, assign) float cx;
@property (nonatomic, assign) float cz;
@property (nonatomic, assign) float tx;
@property (nonatomic, assign) float tz;

- (void)drawView:(CADisplayLink *)displayLink; // drawView方法

@end

NS_ASSUME_NONNULL_END
