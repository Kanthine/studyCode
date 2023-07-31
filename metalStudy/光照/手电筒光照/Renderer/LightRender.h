//
//  LightRender.h
//  HelloTriangle
//
//  Created by wyl on 2018/8/23.
//  Copyright © 2018 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>
@import MetalKit;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, kLightType) {
    kLightTypeNone = 0,
    kLightTypeAmbient,
    kLightTypeDiffuse,
    kLightTypeSpecular,
    kLightTypeCompound,
};

@interface LightRender : NSObject<MTKViewDelegate>

@property (nonatomic, assign) kLightType lightType;

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView
                                   lightType:(kLightType)lightType;

@end

NS_ASSUME_NONNULL_END
