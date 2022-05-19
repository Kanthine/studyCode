#include <metal_stdlib>
#include <simd/simd.h>
#include "AAPLShaderTypes.h"
using namespace metal;

struct RasterizerData {
    float4 position [[position]];
    float3 color;
};

vertex RasterizerData
vertexShader(uint vertexID [[ vertex_id ]],
             constant AAPLVertex *vertexArray [[ buffer(AAPLVertexInputIndexVertices) ]],
             constant AAPLUniforms &uniforms  [[ buffer(AAPLVertexInputIndexUniforms) ]]) {
    
    /// 坐标系转换
    float2 position = vertexArray[vertexID].position.xy / uniforms.viewportSize * 2.0;
    position *= uniforms.scale; /// 缩放动画
    
    RasterizerData out;
    out.position = float4(position, 0, 1);
    out.color = vertexArray[vertexID].color;
    return out;
}

fragment float4
fragmentShader(RasterizerData in [[stage_in]]) {
    return float4(in.color, 1.0);
}

