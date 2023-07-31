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
    float3 specular; /// 镜面光
};

float2 textureCoordinateFromNormal_Specular(float3 normal) {
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

vertex ShaderInOut vertexRender_Specular(VertexDesc in [[ stage_in ]],
                                constant Uniforms &uniformData [[buffer(kAttributeUniforms)]]) {
    ShaderInOut out;
    out.position = uniformData.projectionMatrix * uniformData.cameraMatrix * uniformData.worldMatrix * in.position;
    out.normal = in.normal;
    out.textureCoordinate = textureCoordinateFromNormal_Specular(in.normal);
    
    /// 计算镜面光
    float3 transformNormal = normalize((uniformData.worldMatrix * float4(in.normal, 1.0)).xyz);
    
    /// 计算像素点到摄像机的向量
    float3 eye = normalize(uniformData.cameraPos - (uniformData.worldMatrix * in.position).xyz);
    /// 计算像素点到光源位置的向量
    float3 vp = normalize(uniformData.lightLocation - (uniformData.worldMatrix * in.position).xyz);
    if (uniformData.isDirectionLight) {
        vp = normalize(uniformData.lightDirection);
    }
    float3 halfVector = normalize(eye + vp);
    float shininess = 20.0; /// 粗糙度、越小越光滑
    float dotPos = dot(transformNormal, halfVector);
    float powerFactor = max(0.0, pow(dotPos, shininess));
    out.specular = uniformData.specular * powerFactor;
    
    return out;
}

/* 点光源
 * 需要知道眼睛位置、光源位置
 * 光源位置与眼睛位置有一个中间方向
 * 当中间方向与材质的法向量重叠时、入射角等于反射角、眼睛正好接受反射光，此时反射光最亮
 * 法向量与中间夹角最小的时候，眼睛感受到的光最亮
 */
/* 镜面光(Specular)：当光滑表面被照射时会有方向很集中的反射光；
 *
 * 与散射光最终强度仅依赖于入射光与被照射点法向量的夹角不同，镜面光的最终强度还依赖于观察者的位置。
 * 也就是说，如果从摄像机到被照射点的向量不在反射光方向集中的范围内，观察者将不会看到镜面光
 *
 * 镜面光的计算模型比前面的两种光都要复杂一些，具体公式如下：
 *          镜面光照射结果 = 材质的反射系数 * 镜面光强度 * max(0, cos(半向量与法向量的夹角))
 *      实际开发中往往分两步进行计算，此时公式被拆解为如下情况：
 *          镜面光最终强度 = 镜面光强度 * max(0, cos(半向量与法向量的夹角))
 *          镜面光照射结果 = 材质的反射系数 * 镜面光最终强度
 *      半向量：指的是从被照射点到光源的向量与从被照射点到观察点向量的平均向量；
 *
 */
fragment half4 fragmentShader_Specular(ShaderInOut        in             [[ stage_in ]],
                                      texture2d<float> colorTexture [[ texture(kAttributeTexture) ]]) {
    constexpr sampler linearSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
    float4 color = colorTexture.sample(linearSampler, in.textureCoordinate); /// 材质的反射系数
//    return half4(color * float4(in.specular, 1.0));
    return half4(color + float4(in.specular, 1.0));
}
