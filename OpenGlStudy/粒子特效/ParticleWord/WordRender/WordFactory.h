//
//  WordFactory.h
//  ParticleWord
//
//  Created by i7y on 2022/2/10.
//

#import <Foundation/Foundation.h>
#import <CoreText/CoreText.h>
#import <QuartzCore/QuartzCore.h>
#include <simd/simd.h>

NS_ASSUME_NONNULL_BEGIN

vector_float2 *getPointsByCGPath(CGPathRef cgPath, int *length);

CGMutablePathRef getPathWithAttributedString(CFAttributedStringRef string);

NS_ASSUME_NONNULL_END
