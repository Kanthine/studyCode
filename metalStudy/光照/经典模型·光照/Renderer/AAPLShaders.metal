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
    float4  tangent;
    float4  bitangent;
    float4  normal;
    
    float4 ambient;  /// 环境光
    float4 diffuse;  /// 漫反射
    float4 specular; /// 镜面光
};

vertex ObjData vertexRender(const VertexDetaile in [[ stage_in ]],
                            const device Uniforms &uniforms[[buffer(AAPLVertexInputIndexUniforms)]]) {
    ObjData out;
    out.position = uniforms.projectionMatrix * uniforms.cameraMatrix * uniforms.worldMatrix * float4(in.position, 1.0);
    out.texCoord = in.texCoord;
    
    out.tangent = normalize(uniforms.worldMatrix * float4(in.tangent, 1.0));
    out.bitangent = -normalize(uniforms.worldMatrix * float4(in.bitangent, 1.0));
    out.normal = normalize(uniforms.worldMatrix * float4(in.normal, 1.0));
    
    
    /**  计算环境光   */
    out.ambient = uniforms.ambient;
    
    /**  计算漫反射   */
    /// 物体表面点 O 到光源 L 的向量 OP
    float3 OP = normalize(uniforms.lightLocation - (uniforms.worldMatrix * float4(in.position, 1.0)).xyz); /// 定位光（灯泡）
    if (uniforms.isDirectionLight) {
        OP = normalize(uniforms.lightDirection); /// 定向光（太阳光）：光线之间是平行的
    }
    float dotPos = max(0.0, dot(out.normal.xyz, OP)); /// 余弦值
    out.diffuse = uniforms.diffuse * dotPos;
    
    /**  计算镜面光   */
    
    /// 物体表面点 O 到摄像机 C 的向量 OC
    float3 eye = normalize(uniforms.cameraPos - (uniforms.worldMatrix * float4(in.position, 1.0)).xyz);
    // 半法向量：从被照射点到光源的向量 OP 与从被照射点到观察点向量 OC 的平均向量
    float3 halfVector = normalize(eye + OP);
    float shininess = 50.1; /// 粗糙度越小、镜面光面积越大
    float dotPos1 = dot(out.normal.xyz, halfVector);
    float powerFactor = max(0.0, pow(dotPos1, shininess));
    out.specular = uniforms.specular * powerFactor;
    
    return out;
}

fragment half4 fragmentRender(ObjData in [[ stage_in ]],
                              texture2d<half> baseColorMap [[ texture (AAPLTextureIndexBaseColor) ]] ) {
    constexpr sampler linearSampler(mip_filter::linear,
                                    mag_filter::linear,
                                    min_filter::linear,
                                    s_address::repeat,
                                    t_address::repeat);
    half4 color = baseColorMap.sample(linearSampler,in.texCoord.xy);
    color = color * half4(in.ambient + in.diffuse + in.specular);    
    return color;
}
