#include <metal_stdlib>
#include "GlobalType.h"
using namespace metal;

struct SkyboxVertex {
    float4 position [[attribute(SkyVertexAttributePosition)]];
    float3 normal   [[attribute(SkyVertexAttributeNormal)]];
};

struct SkyboxInOut {
    float4 position [[position]];
    float3 texcoord;
};

vertex SkyboxInOut skybox_vertex(SkyboxVertex in [[ stage_in ]],
                                 constant Uniforms &uniformData [[buffer(SkyBufferIndexUniformData)]]) {
    SkyboxInOut out;
    out.position = uniformData.projectionMatrix * uniformData.cameraMatrix * uniformData.worldMatrix * in.position;
    out.texcoord = in.normal;
    return out;
}

fragment half4 skybox_fragment(SkyboxInOut        in             [[ stage_in ]],
                               texturecube<float> skybox_texture [[ texture(SkyTextureIndexBaseColor) ]]) {
    constexpr sampler linearSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
    float4 color = skybox_texture.sample(linearSampler, in.texcoord);
    return half4(color);
}
