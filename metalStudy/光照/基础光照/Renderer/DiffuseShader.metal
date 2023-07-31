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
    float3 diffuse; /// 漫反射
};

float2 textureCoordinateFromNormal_Diffuse(float3 normal) {
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

vertex ShaderInOut vertexRender_Diffuse(VertexDesc in [[ stage_in ]],
                                constant Uniforms &uniformData [[buffer(kAttributeUniforms)]]) {
    ShaderInOut out;
    out.position = uniformData.projectionMatrix * uniformData.cameraMatrix * uniformData.worldMatrix * in.position;
    out.normal = in.normal;
    out.textureCoordinate = textureCoordinateFromNormal_Diffuse(in.normal);
    
    /// 计算漫反射
    float3 transformNormal = normalize((uniformData.worldMatrix * float4(in.normal, 1.0)).xyz);
    float3 vp = normalize(uniformData.lightLocation - (uniformData.worldMatrix * in.position).xyz);
    if (uniformData.isDirectionLight) {
        vp = normalize(uniformData.lightDirection);
    }
    float dotPos = max(0.0, dot(transformNormal, vp));
    out.diffuse = uniformData.diffuse * dotPos;
    return out;
}

fragment half4 fragmentShader_Diffuse(ShaderInOut        in             [[ stage_in ]],
                                      texture2d<float> colorTexture [[ texture(kAttributeTexture) ]]) {
    constexpr sampler linearSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
    float4 color = colorTexture.sample(linearSampler, in.textureCoordinate);
    return half4(color * float4(in.diffuse, 1.0)); /// 最终光照结果
    /// 正对光源(入射角小)的位置看起来较亮，而随着入射角的增大越来越暗，直到入射角大于 90°后完全不能照亮。
}

/* 散射光(Diffuse): 从物体表面向全方位 360° 均匀反射的光；
 * 散射光具体代表的是现实世界中粗糙的物体表面被光照射时，反射光在各个方向基本均匀(也称为“漫反射”)的情况；
 *
 * 虽然反射后的散射光在各个方向是均匀的，但散射光反射的强度与入射光的强度以及入射的 角度密切相关。
 * 因此，当光源的位置发生变化时，散射光的效果会发生明显变化。主要体现为当光垂直地照射到物体表面时比斜照时要亮，其具体计算公式如下：
 *          散射光照射结果 = 材质的反射系数 * 散射光强度 * max(cos(入射角),0)
 *      实际开发中往往分两步进行计算，此时公式被拆解为如下情况。
 *          散射光最终强度 = 散射光强度 * max(cos(入射角),0)
 *          散射光照射结果 = 材质的反射系数 * 散射光最终强度
 *
 * 与环境光计算公式唯一的区别是引入了最后一项 max(cos(入射 角),0) 。
 * 其含义是入射角越大，反射强度越弱，当入射角的余弦值为负时(即入射角大于 90°)，反射强度为 0。
 */
