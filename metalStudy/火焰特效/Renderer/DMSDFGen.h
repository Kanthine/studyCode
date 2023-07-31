//
//  DMSDFGen.h
//  DMBulletSDK
//
//  Created by liguoqing on 2022/8/11.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface DMSDFGen : NSObject
// The output distance field is encoded as bytes, where 0 = radius (outside) and 255 = -radius (inside).
// Input and output can be the same buffer.
//   out - Output of the distance transform, one byte per pixel.
//   outstride - Bytes per row on output image.
//   radius - The radius of the distance field narrow band in pixels.
//   img - Input image, one byte per pixel.
//   width - Width if the image.
//   height - Height if the image.
//   stride - Bytes per row on input image.
+ (void)sdfBuildDistanceFieldWith:(unsigned char*)outData
//                        outstride:(int)outstride
                           radius:(float)radius
                           srcImg:(const unsigned char *)img
                            width:(int)width
                           height:(int)height;
//                           stride:(int)stride;

// This function converts the antialiased image where each pixel represents coverage (box-filter
// sampling of the ideal, crisp edge) to a distance field with narrow band radius of sqrt(2).
// This is the fastest way to turn antialised image to contour texture. This function is good
// if you don't need the distance field for effects (i.e. fat outline or dropshadow).
// Input and output buffers must be different.
//   out - Output of the distance transform, one byte per pixel.
//   outstride - Bytes per row on output image.
//   img - Input image, one byte per pixel.
//   width - Width if the image.
//   height - Height if the image.
//   stride - Bytes per row on input image.
+ (void)sdfCoverageToDistanceFieldWith:(unsigned char*)outData
//                             outstride:(int)outstride
                                srcImg:(const unsigned char *)img
                                 width:(int)width
                                height:(int)height;
//                                stride:(int)stride;

@end

NS_ASSUME_NONNULL_END
