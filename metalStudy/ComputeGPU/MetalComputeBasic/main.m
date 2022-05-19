#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import "MetalAdder.h"

/// 使用 CPU 对两个数组的元素做加法运算
void add_arrays(const float* inA,
                const float* inB,
                float* result,
                int length) {
    for (int index = 0; index < length ; index++) {
        result[index] = inA[index] + inB[index];
    }
}

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        id<MTLDevice> device = MTLCreateSystemDefaultDevice();

        MetalAdder* adder = [[MetalAdder alloc] initWithDevice:device];
        [adder prepareData];
        [adder sendComputeCommand];

        NSLog(@"Execution finished");
    }
    return 0;
}
