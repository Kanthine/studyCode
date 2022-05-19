//
//  RectangleShader.metal
//  DeferredLighting
//
//  Created by i7y on 2022/2/18.
//  Copyright © 2022 Apple. All rights reserved.
//

#include <metal_stdlib>
#include "AAPLShaderTypes.h"
using namespace metal;

struct RasterizerData {
    float4 position [[position]];
};

vertex RasterizerData
vertexShader(uint vertexID [[vertex_id]],
             constant vector_float2 *vertices [[buffer(VertexInputVertex)]],
             constant vector_float2 *viewportSizePointer [[buffer(VertexInputViewportSize)]])
{
    RasterizerData out;
    float2 pixelSpacePosition = vertices[vertexID].xy;
    vector_float2 viewportSize = vector_float2(*viewportSizePointer);
    out.position = vector_float4(0.0, 0.0, 0.7, 1.0);
    out.position.xy = pixelSpacePosition / (viewportSize / 2.0);
    return out;
}

fragment float4 fragmentShader(RasterizerData in [[stage_in]]) {
    return float4(1.0, 1.0, 1.0, 0.1);
}

