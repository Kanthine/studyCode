#import "MetalAdder.h"

// The number of floats in each array, and the size of the arrays in bytes.
const unsigned int arrayLength = 1 << 24;
const unsigned int bufferSize = arrayLength * sizeof(float);

@implementation MetalAdder {
    id<MTLDevice> _mDevice;

    id<MTLComputePipelineState> _computePipelineState; // 计算管线
    id<MTLCommandQueue> _commandQueue; ///命令队列

    /// 数据缓冲区
    id<MTLBuffer> _mBufferA;
    id<MTLBuffer> _mBufferB;
    id<MTLBuffer> _mBufferResult;

}

- (instancetype) initWithDevice: (id<MTLDevice>) device {
    self = [super init];
    if (self) {
        _mDevice = device;
        NSError* error = nil;

        id<MTLLibrary> defaultLibrary = [_mDevice newDefaultLibrary];
        NSAssert(defaultLibrary, @"Failed to find the default library.");

        id<MTLFunction> addFunction = [defaultLibrary newFunctionWithName:@"add_arrays"];

        _computePipelineState = [_mDevice newComputePipelineStateWithFunction:addFunction error:&error];
        NSAssert(_computePipelineState, @"Failed to created pipeline state object, error %@.", error);
        
        _commandQueue = [_mDevice newCommandQueue];
        NSAssert(_commandQueue, @"Failed to find the command queue.");
    }

    return self;
}

- (void)prepareData {
    _mBufferA = [_mDevice newBufferWithLength:bufferSize options:MTLResourceStorageModeShared];
    _mBufferB = [_mDevice newBufferWithLength:bufferSize options:MTLResourceStorageModeShared];
    _mBufferResult = [_mDevice newBufferWithLength:bufferSize options:MTLResourceStorageModeShared];
    
    /// 随机生成假数据
    float *arrayA = _mBufferA.contents, *arrayB = _mBufferB.contents;
    for (unsigned long index = 0; index < arrayLength; index++) {
        arrayA[index] = (float)rand()/(float)(RAND_MAX);
        arrayB[index] = (float)rand()/(float)(RAND_MAX);
    }
}

/// 使用 GPU 计算
- (void)sendComputeCommand {
    /// 命令缓冲区
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    id<MTLComputeCommandEncoder> computeEncoder = [commandBuffer computeCommandEncoder];

    [computeEncoder setComputePipelineState:_computePipelineState];
    [computeEncoder setBuffer:_mBufferA offset:0 atIndex:0];
    [computeEncoder setBuffer:_mBufferB offset:0 atIndex:1];
    [computeEncoder setBuffer:_mBufferResult offset:0 atIndex:2];

    MTLSize gridSize = MTLSizeMake(arrayLength, 1, 1);

    /// 线程组中的线程数
    NSUInteger threadGroupSize = MIN( _computePipelineState.maxTotalThreadsPerThreadgroup, arrayLength);
    /// 由于是计算一维数组，因此 height、depth 均设置为 1
    MTLSize threadgroupSize = MTLSizeMake(threadGroupSize, 1, 1);

    [computeEncoder dispatchThreads:gridSize threadsPerThreadgroup:threadgroupSize];

    // End the compute pass.
    [computeEncoder endEncoding];

    // Execute the command.
    [commandBuffer commit];

    /// 阻塞当前线程，等待任务完成
    [commandBuffer waitUntilCompleted];

    [self verifyResults];
}

- (void)verifyResults {
    float* a = _mBufferA.contents;
    float* b = _mBufferB.contents;
    float* result = _mBufferResult.contents;

    for (unsigned long index = 0; index < arrayLength; index++) {
        if (result[index] != (a[index] + b[index])) {
            printf("Compute ERROR: index=%lu result=%g vs %g=a+b\n",
                   index, result[index], a[index] + b[index]);
            assert(result[index] == (a[index] + b[index]));
        }
    }
    printf("Compute results as expected\n");
}

@end
