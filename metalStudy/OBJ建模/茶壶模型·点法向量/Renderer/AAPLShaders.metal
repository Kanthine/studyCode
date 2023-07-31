#include <metal_stdlib>
#include "AAPLShaderTypes.h"

using namespace metal;

struct VertexDetaile {
    float3 position   [[attribute(AAPLVertexAttributePosition)]];
    float2 texCoord   [[attribute(AAPLVertexAttributeTexcoord)]];
    float3 normal     [[attribute(AAPLVertexAttributeNormal)]];    // 法线
    float3 tangent    [[attribute(AAPLVertexAttributeTangent)]];   // 切线
    float3 bitangent  [[attribute(AAPLVertexAttributeBitangent)]]; // 双切线
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
    
    float3 ambient;  /// 环境光
    float3 diffuse;  /// 漫反射
    float3 specular; /// 镜面光
};

vertex ObjData vertexRender(const VertexDetaile in [[ stage_in ]],
                            const device Uniforms &uniforms[[buffer(AAPLVertexInputIndexUniforms)]]) {
    ObjData out;
    out.position = uniforms.projectionMatrix * uniforms.cameraMatrix * uniforms.worldMatrix * float4(in.position, 1.0);
    out.texCoord = in.texCoord;
    
    out.tangent   =  normalize(uniforms.worldMatrix * float4(in.tangent,   1.0));
    out.bitangent = -normalize(uniforms.worldMatrix * float4(in.bitangent, 1.0));
    out.normal    =  normalize(uniforms.worldMatrix * float4(in.normal,    1.0));
    
    /**  计算环境光   */
    out.ambient = uniforms.ambient;

    /**  计算漫反射   */
    /// 变换后的法向量
    float3 transformNormal = normalize((uniforms.worldMatrix * float4(in.normal, 1.0)).xyz);
    
    /// 物体表面到光源的向量
    float3 vp = normalize(uniforms.lightLocation - (uniforms.worldMatrix * float4(in.position, 1.0)).xyz); /// 定位光
    if (uniforms.isDirectionLight) {
        vp = normalize(uniforms.lightDirection);
    }
    float dotPos = max(0.0, dot(transformNormal, vp));
    out.diffuse = uniforms.diffuse * dotPos;
    
    /**  计算镜面光   */
    
    /// 计算像素点到摄像机的向量
    float3 eye = normalize(uniforms.cameraPos - (uniforms.worldMatrix * float4(in.position, 1.0)).xyz);
    /// 计算像素点到光源位置的向量
    float3 halfVector = normalize(eye + vp);
    float shininess = 50.0; /// 粗糙度、越小越光滑
    float dotPos1 = dot(transformNormal, halfVector);
    float powerFactor = max(0.0, pow(dotPos1, shininess));
    out.specular = uniforms.specular * powerFactor;
    return out;
}

fragment half4 fragmentRender(ObjData in [[ stage_in ]],
                              texture2d<half> baseColorMap [[ texture (AAPLTextureIndexBaseColor) ]] ) {
    float3 finalColor = float3(0.9, 0.9, 0.9);
    float3 color = finalColor * in.ambient + finalColor * in.specular + finalColor * in.diffuse;
    return half4(half3(color), 1.0);
}
