#include <metal_stdlib>
#include "AAPLShaderTypes.h"
using namespace metal;

struct RasterizerData {
    /// 属性限定符 [[position]] 表示顶点着色器输出的剪裁空间坐标
    /// 该坐标必须是 float4 类型
    float4 position [[position]];

    /// 在光栅化阶段，会使用平滑插值算法，为图元中的每个片元差值
    /// 光栅化阶段处理之后，将该值传递到片段着色器
    float4 color;
};

vertex RasterizerData
vertexShader(uint vertexID [[vertex_id]],
             constant AAPLVertex *vertices [[buffer(AAPLVertexInputIndexVertices)]],
             constant vector_float2 &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]]) {
    /// 视图坐标转剪裁空间坐标
    float2 position = vertices[vertexID].position.xy / viewportSize * 2.0;
    
    RasterizerData out;
    out.position = vector_float4(position, 0.0, 1.0);
    out.color = vertices[vertexID].color; /// 传递色值到光栅化阶段
    return out;
}

fragment float4 fragmentShader(RasterizerData in [[stage_in]]) {
    return in.color;
}

