#include <metal_stdlib>
#import "AAPLShaderTypes.h"
using namespace metal;

struct RasterizerData {
    float4 position [[position]]; /// 几何坐标
    float2 textureCoordinate;     /// 纹理坐标
};

/// 顶点着色器
vertex RasterizerData
vertexShader(uint                   vertexID       [[ vertex_id ]],
             constant vector_float4 *vertexArray   [[ buffer(VertexInputIndexVertices) ]],
             constant vector_float2 &viewportSize  [[ buffer(VertexInputIndexViewportSize) ]]) {

    float2 position = vertexArray[vertexID].xy / viewportSize * 2.0;
    position.x = position.x - 1.0;
    position.y = 1.0 - position.y;
    
    RasterizerData out;
    out.position = float4(position, 0.0, 1.0);
    out.textureCoordinate = vertexArray[vertexID].zw;
    return out;
}

/// 片段着色器
fragment float4 samplingShader(RasterizerData  in           [[stage_in]],
                               texture2d<half> colorTexture [[ texture(TextureIndexOutput) ]]) {
    constexpr sampler textureSampler(mag_filter::linear, min_filter::linear); /// 采样器
    const half4 colorSample = colorTexture.sample (textureSampler, in.textureCoordinate); // 对纹理采样
    return float4(colorSample);
}

// Rec. 709 luma values for grayscale image conversion
constant half3 kRec709Luma = half3(0.2126, 0.7152, 0.0722);

// 计算着色器
kernel void
grayscaleKernel(texture2d<half, access::read>  inTexture  [[texture(TextureIndexInput)]],
                texture2d<half, access::write> outTexture [[texture(TextureIndexOutput)]],
                uint2                          gid        [[thread_position_in_grid]]) {
    /// 检查像素是否在输出纹理的边界内
    if((gid.x >= outTexture.get_width()) || (gid.y >= outTexture.get_height())) return;
    
    half4 inColor  = inTexture.read(gid);
    half  gray     = dot(inColor.rgb, kRec709Luma);
    outTexture.write(half4(gray, gray, gray, 1.0), gid);
}

