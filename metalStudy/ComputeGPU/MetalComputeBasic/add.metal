#include <metal_stdlib>
using namespace metal;

/// 使用 GPU 对两个数组的元素做加法运算
kernel void add_arrays(device const float* inA,
                       device const float* inB,
                       device float* result,
                       uint index [[thread_position_in_grid]]) {    
    result[index] = inA[index] + inB[index];
}
