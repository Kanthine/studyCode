#include <metal_stdlib>
#include "AAPLShaderTypes.h"
using namespace metal;

struct RasterizerData {
    float4 position [[position]];
    float4 color;
};

vertex RasterizerData
vertexShader(uint                      vertexID     [[ vertex_id ]],
             const device AAPLVertex * vertices     [[ buffer(AAPLVertexInputIndexVertices) ]],
             constant vector_float2   &viewportSize [[ buffer(AAPLVertexInputIndexViewport) ]]) {
    
    /// 坐标系转换
    float2 position = vertices[vertexID].position.xy / viewportSize * 2.0;
    position.x = position.x - 1.0;
    position.y = 1.0 - position.y;
    
    RasterizerData out;
    out.position = vector_float4(position, vertices[vertexID].position.z, 1.0);
    out.color = vertices[vertexID].color;
    return out;
}

fragment float4 fragmentShader(RasterizerData in [[stage_in]]) {
    return in.color;
}

