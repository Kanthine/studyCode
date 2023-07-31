#include <metal_stdlib>
#include "AAPLShaderTypes.h"

using namespace metal;

struct VertexDetaile {
    float3 position  [[attribute(AAPLVertexAttributePosition)]];
    float2 texCoord  [[attribute(AAPLVertexAttributeTexcoord)]];
    float3 normal     [[attribute(AAPLVertexAttributeNormal)]];
    float3 tangent    [[attribute(AAPLVertexAttributeTangent)]];
    float3 bitangent  [[attribute(AAPLVertexAttributeBitangent)]];
};

struct ObjData {
    float4 position [[position]];
    float2 texCoord;
    float2 shadow_uv;
    half   shadow_depth;
    float3 eye_position;
    float4  tangent;
    float4  bitangent;
    float4  normal;
};

vertex ObjData vertexRender(const VertexDetaile in [[ stage_in ]],
                            const device Uniforms &uniforms[[buffer(AAPLVertexInputIndexUniforms)]]) {
    ObjData out;
    out.position = uniforms.projectionMatrix * uniforms.cameraMatrix * uniforms.worldMatrix * float4(in.position, 1.0);
    out.texCoord = in.texCoord;
    
    out.tangent = normalize(uniforms.worldMatrix * float4(in.tangent, 1.0));
    out.bitangent = -normalize(uniforms.worldMatrix * float4(in.bitangent, 1.0));
    out.normal = normalize(uniforms.worldMatrix * float4(in.normal, 1.0));
    return out;
}

fragment half4 fragmentRender(ObjData in [[ stage_in ]],
                              texture2d<half> baseColorMap [[ texture (AAPLTextureIndexBaseColor) ]] ) {
    constexpr sampler linearSampler(mip_filter::linear,
                                    mag_filter::linear,
                                    min_filter::linear,
                                    s_address::repeat,
                                    t_address::repeat);
    half4 color_sample = baseColorMap.sample(linearSampler,in.texCoord.xy);
    return color_sample;
}
