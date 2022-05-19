#include <metal_stdlib>
#include <simd/simd.h>
#include "AAPLShaderTypes.h"
using namespace metal;

struct RasterizerData {
    float4 position [[position]]; /// 剪裁空间的坐标
    float2 textureCoordinate;     /// 纹理坐标，在光栅话阶段被平滑的插值
};

vertex RasterizerData
vertexShader(uint vertexID [[ vertex_id ]],
             constant AAPLVertex     *vertexArray  [[ buffer(VertexInputIndexVertices) ]],
             constant vector_float2 &viewportSize  [[ buffer(VertexInputIndexViewportSize) ]]) {
    float2 position = vertexArray[vertexID].position.xy / viewportSize * 2.0;
    
    RasterizerData out;
    out.position = vector_float4(position, 0.0, 1.0);
    out.textureCoordinate = vertexArray[vertexID].textureCoordinate;
    return out;
}

fragment float4
samplingShader(RasterizerData in [[stage_in]],
               texture2d<half> colorTexture [[ texture(TextureIndexBaseColor) ]]) {
    constexpr sampler textureSampler (mag_filter::linear, min_filter::linear);
    const half4 colorSample = colorTexture.sample(textureSampler, in.textureCoordinate);
    return float4(colorSample);
}

