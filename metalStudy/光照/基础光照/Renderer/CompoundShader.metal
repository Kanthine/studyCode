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
    
    float3 ambient;  /// 环境光
    float3 diffuse;  /// 漫反射
    float3 specular; /// 镜面光
};

float2 textureCoordinateFromNormal_Compound(float3 normal) {
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

vertex ShaderInOut vertexRender_Compound(VertexDesc in [[ stage_in ]],
                                constant Uniforms &uniformData [[buffer(kAttributeUniforms)]]) {
    ShaderInOut out;
    out.position = uniformData.projectionMatrix * uniformData.cameraMatrix * uniformData.worldMatrix * in.position;
    out.normal = in.normal;
    out.textureCoordinate = textureCoordinateFromNormal_Compound(in.normal);
    
    /**  计算环境光   */
    out.ambient = uniformData.ambient;

    /**  计算漫反射   */
    /// 变换后的法向量
    float3 transformNormal = normalize((uniformData.worldMatrix * float4(in.normal, 1.0)).xyz);
    
    /// 物体表面到光源的向量
    float3 vp = normalize(uniformData.lightLocation - (uniformData.worldMatrix * in.position).xyz); /// 定位光
    if (uniformData.isDirectionLight) {
        vp = normalize(uniformData.lightDirection);
    }
    float dotPos = max(0.0, dot(transformNormal, vp));
    out.diffuse = uniformData.diffuse * dotPos;
    
    /**  计算镜面光   */
    
    /// 计算像素点到摄像机的向量
    float3 eye = normalize(uniformData.cameraPos - (uniformData.worldMatrix * in.position).xyz);
    /// 计算像素点到光源位置的向量
    float3 halfVector = normalize(eye + vp);
    float shininess = 50.0; /// 粗糙度、越小越光滑
    float dotPos1 = dot(transformNormal, halfVector);
    float powerFactor = max(0.0, pow(dotPos1, shininess));
    out.specular = uniformData.specular * powerFactor;
    return out;
}

/* 现实世界中：环境光、散射光、镜面光 3 种通道是同时作用的
 */
fragment half4 fragmentShader_Compound(ShaderInOut        in             [[ stage_in ]],
                                      texture2d<float> colorTexture [[ texture(kAttributeTexture) ]]) {
    constexpr sampler linearSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
    float4 color = colorTexture.sample(linearSampler, in.textureCoordinate); /// 材质的反射系数
    return half4(color * float4((in.ambient + in.diffuse + in.specular), 1.0));
}
