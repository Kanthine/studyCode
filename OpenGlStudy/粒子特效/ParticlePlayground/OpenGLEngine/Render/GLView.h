//
//  GLView.h
//  Ball
//
//  Created by 苏莫离 on 2019/10/17.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface GLView : UIView

@property (nonatomic, assign) float zNear;
@property (nonatomic, assign) float zFar;
@property (nonatomic, assign) float fov;
@property (nonatomic, assign) float zPos;

- (void)drawView:(CADisplayLink *)displayLink; // drawView方法

@end

NS_ASSUME_NONNULL_END
