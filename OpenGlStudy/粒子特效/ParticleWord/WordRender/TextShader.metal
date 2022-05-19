//
//  TextShader.metal
//  Graphics
//
//  Created by 王玉龙 on 2021/11/28.
//

#include <metal_stdlib>
using namespace metal;

typedef struct {
    float4 position [[position]];
} YLTextData;

vertex YLTextData
vertexTextShader(uint vertexID [[vertex_id]],
             constant vector_float2 *vertices [[buffer(0)]],
             constant vector_float2 *viewportSizePointer [[buffer(1)]]) {
    YLTextData out;
    
    float2 pixelSpacePosition = vertices[vertexID].xy;
    vector_float2 viewportSize = vector_float2(*viewportSizePointer);
    
    out.position = vector_float4(0.0, 0.0, 0.0, 1.0);
    out.position.x = 2 * pixelSpacePosition.x / viewportSize.x - 0.5;
    out.position.y = 2 * pixelSpacePosition.y / viewportSize.y;
    return out;
}

fragment float4 fragmentTextShader(YLTextData in [[stage_in]]) {
    return (float4){1, 1, 1, 1}; // 返回插入的颜色
}
