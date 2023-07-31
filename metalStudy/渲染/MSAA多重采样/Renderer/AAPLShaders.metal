/*
See LICENSE folder for this sample’s licensing information.

Abstract:
The Metal shaders the renderer uses to rasterize the thin shards.
*/

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

#import "AAPLShaderTypes.h"
#import "AAPLShaderCommon.h"

struct RasterizerData {
    float4 position [[position]];
    float2 textureCoordinate;
};

vertex RasterizerData
vertexShader(uint vertexID [[vertex_id]],
             constant vector_float4 *vertexs [[buffer(AAPLVertexInputIndexVertices)]]) {
    RasterizerData out;
    out.position = float4(vertexs[vertexID].x * 2.0 - 1, 1.0 - vertexs[vertexID].y * 2.0, 0.0, 1.0);
    out.textureCoordinate = vertexs[vertexID].zw;
    return out;
}

fragment FragData
fragmentShader(RasterizerData in [[stage_in]],
               texture2d<half> texture [[texture(AAPLVertexInputIndexTexture)]])
{
    constexpr sampler textureSampler(mag_filter::linear, min_filter::linear);
    half4 color = texture.sample(textureSampler, in.textureCoordinate.xy);
    return FragData{color};
}

fragment FragData
fragmentShaderHDR(RasterizerData in [[stage_in]],
                  texture2d<half> texture [[texture(AAPLVertexInputIndexTexture)]])
{
    constexpr sampler textureSampler(mag_filter::linear, min_filter::linear);
    half4 color = texture.sample(textureSampler, in.textureCoordinate.xy);
    const half3 tonemappedColor = tonemapByLuminance(color.xyz);
    return FragData{half4(tonemappedColor, 1.0)};
}
