#include <metal_stdlib>
#include "MaskShaderType.h"
using namespace metal;

struct MaskData {
    float4 position [[position]];
};

vertex MaskData
vertexShader(uint                         vertexID     [[ vertex_id ]],
             const device vector_float2  *vertices     [[ buffer(VertexInputIndexVertices) ]],
             constant  vector_float2     &viewportSize [[ buffer(VertexInputIndexViewport) ]]) {
    
    /// 坐标系转换
    float2 position = vertices[vertexID].xy / viewportSize * 2.0;
    position.x = position.x - 1.0;
    position.y = 1.0 - position.y;
    
    MaskData out;
    out.position = vector_float4(position, 0.0, 1.0);
    return out;
}

fragment float4 fragmentShader(MaskData in [[stage_in]]) {
    return float4(1.0);
}
