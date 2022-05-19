#import <Foundation/Foundation.h>
#import <Metal/Metal.h>

NS_ASSUME_NONNULL_BEGIN

@interface MetalAdder : NSObject
- (instancetype)initWithDevice: (id<MTLDevice>) device;
- (void)prepareData;
- (void)sendComputeCommand;
@end

NS_ASSUME_NONNULL_END

//
//- (void)prepareData {
//    /// 随机生成假数据
//    float arrayA[arrayLength], arrayB[arrayLength];
//    for (unsigned long index = 0; index < arrayLength; index++) {
//        arrayA[index] = (float)rand()/(float)(RAND_MAX);
//        arrayB[index] = (float)rand()/(float)(RAND_MAX);
//    }
//    
//    _mBufferA = [_mDevice newBufferWithBytes:&arrayA length:sizeof(arrayA) options:MTLResourceStorageModeShared];
//    _mBufferB = [_mDevice newBufferWithBytes:&arrayB length:sizeof(arrayB) options:MTLResourceStorageModeShared];
//    _mBufferResult = [_mDevice newBufferWithLength:sizeof(arrayA) options:MTLResourceStorageModeShared];
//}
