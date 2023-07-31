#include <metal_stdlib>
#include "GlobalType.h"
using namespace metal;

struct VertexDesc {
    float4 position [[attribute(kVertexAttributePosition)]];
    float3 normal   [[attribute(kVertexAttributeNormal)]];
};

struct ShaderInOut {
    float4 position [[position]];
    float2 textureCoordinate; /// 纹理坐标
    float3 normal; /// 法向量
};

float2 textureCoordinateFromNormal(float3 normal) {
    float phi = acos(normal.y);
    float theta = sin(phi) == 0 ? 0 : acos(normal.x / sin(phi));
    float v = phi / 3.1415926;
    float u = theta / (3.1415926 * 2.0);
    if(normal.z < 0) {
        theta = theta + 3.1415926;
        u = theta / (3.1415926 * 2.0);
        u = 1 - u + 0.5;
    }
    
    return float2(u ,v);
}

vertex ShaderInOut vertexRender(VertexDesc in [[ stage_in ]],
                                constant Uniforms &uniformData [[buffer(kAttributeUniforms)]]) {
    ShaderInOut out;
    out.position = uniformData.projectionMatrix * uniformData.cameraMatrix * uniformData.worldMatrix * in.position;
    out.normal = in.normal;
    out.textureCoordinate = textureCoordinateFromNormal(in.normal);
    return out;
}

fragment half4 fragmentShader(ShaderInOut        in             [[ stage_in ]],
                               texture2d<float> colorTexture [[ texture(kAttributeTexture) ]]) {
    constexpr sampler linearSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
    float4 color = colorTexture.sample(linearSampler, in.textureCoordinate);
    return half4(color);
}
