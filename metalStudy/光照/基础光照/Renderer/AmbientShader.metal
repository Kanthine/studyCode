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
    float4 ambient; /// 环境光
};

float2 textureCoordinateFromNormal_Ambient(float3 normal) {
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

vertex ShaderInOut vertexRender_Ambient(VertexDesc in [[ stage_in ]],
                                constant Uniforms &uniformData [[buffer(kAttributeUniforms)]]) {
    ShaderInOut out;
    out.position = uniformData.projectionMatrix * uniformData.cameraMatrix * uniformData.worldMatrix * in.position;
    out.normal = in.normal;
    out.textureCoordinate = textureCoordinateFromNormal_Ambient(in.normal);
    out.ambient = float4(uniformData.ambient, 1.0);
    return out;
}

fragment half4 fragmentShader_Ambient(ShaderInOut        in             [[ stage_in ]],
                                      texture2d<float> colorTexture [[ texture(kAttributeTexture) ]]) {
    constexpr sampler linearSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
    float4 color = colorTexture.sample(linearSampler, in.textureCoordinate);
    return half4(color * in.ambient);
}

/* 环境光(Ambient)：指的是从四面八方照射到物体上，全方位 360°都均匀的光。
 * 其代表的是现实世界中从光源射出，经过多次反射后，各方向基本均匀的光。
 * 环境光最大的特点是不依赖于光源的位置，而且没有方向性！
 *
 * 环境光不但入射是均匀的，反射也是各向均匀的。用于计算环境光的 数学模型非常简单，具体公式如下：
 *        环境光照射结果 = 材质的反射系数 * 环境光强度
 */
