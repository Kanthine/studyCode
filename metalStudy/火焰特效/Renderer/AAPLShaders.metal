#include <metal_stdlib>
#import "AAPLShaderTypes.h"
using namespace metal;


// 噪声设置：火焰效果
constant float Power = 5.059;
constant float Dumping = 10.0;

float3 hash3(float3 p) {
    p = float3(dot(p, float3(127.1, 311.7, 74.7)),
               dot(p, float3(269.5, 183.3, 246.1)),
               dot(p, float3(113.5, 271.9, 124.6)));
    return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
}

float noise(float3 p) {
    float3 i = floor(p);
    float3 f = fract(p);
    float3 u = f * f * (3.0 - 2.0 * f);
    float n0 = dot(hash3(i + float3(0.0, 0.0, 0.0)), f - float3(0.0, 0.0, 0.0));
    float n1 = dot(hash3(i + float3(1.0, 0.0, 0.0)), f - float3(1.0, 0.0, 0.0));
    float n2 = dot(hash3(i + float3(0.0, 1.0, 0.0)), f - float3(0.0, 1.0, 0.0));
    float n3 = dot(hash3(i + float3(1.0, 1.0, 0.0)), f - float3(1.0, 1.0, 0.0));
    float n4 = dot(hash3(i + float3(0.0, 0.0, 1.0)), f - float3(0.0, 0.0, 1.0));
    float n5 = dot(hash3(i + float3(1.0, 0.0, 1.0)), f - float3(1.0, 0.0, 1.0));
    float n6 = dot(hash3(i + float3(0.0, 1.0, 1.0)), f - float3(0.0, 1.0, 1.0));
    float n7 = dot(hash3(i + float3(1.0, 1.0, 1.0)), f - float3(1.0, 1.0, 1.0));
    float ix0 = mix(n0, n1, u.x);
    float ix1 = mix(n2, n3, u.x);
    float ix2 = mix(n4, n5, u.x);
    float ix3 = mix(n6, n7, u.x);
    float ret = mix(mix(ix0, ix1, u.y), mix(ix2, ix3, u.y), u.z) * 0.5 + 0.5;
    return ret * 2.0 - 1.0;
}

float3 renderFire(float2 uv, float iTime, float dist) {
    float3 coord = float3(uv, iTime * 0.25);
    float n = abs(noise(coord));
    n += 0.5 * abs(noise(coord * 2.0));
    n += 0.25 * abs(noise(coord * 4.0));
    n += 0.125 * abs(noise(coord * 8.0));
    n *= (100.001 - Power);
    float k = clamp(dist, 0.0, 1.0);
    n *= dist / pow(1.01 - k, 10);
    float3 col = float3(1.0, 0.25, 0.08) / n;
    col = pow(col, float3(2.0));
    return float3(clamp(col, 0.0, 1.0));
}














struct RasterizerData {
    float4 position [[position]]; /// 几何坐标
    float2 textureCoordinate;     /// 纹理坐标
};

/// 顶点着色器
vertex RasterizerData
vertexShader(uint                   vertexID       [[ vertex_id ]],
             constant vector_float4 *vertexArray   [[ buffer(VertexInputIndexVertices) ]],
             constant vector_float2 &viewportSize  [[ buffer(VertexInputIndexViewportSize) ]]) {

    float2 position = vertexArray[vertexID].xy / viewportSize * 2.0;
    position.x = position.x - 1.0;
    position.y = 1.0 - position.y;
    
    RasterizerData out;
    out.position = float4(position, 0.0, 1.0);
    out.textureCoordinate = vertexArray[vertexID].zw;
    return out;
}

fragment float4 sdfFragment(RasterizerData  in           [[stage_in]],
                            texture2d<half> imageTexture [[ texture(VertexInputIndexTexture) ]],
                            texture2d<half> sdfTexture [[ texture(VertexInputIndexTextureSDF)]],
                            constant vector_float2 &viewportSize [[buffer(VertexInputIndexViewportSize)]],
                            constant float &timer [[buffer(VertexInputIndexTimer)]]) {
    constexpr sampler textureSampler(mag_filter::linear, min_filter::linear, address::clamp_to_zero);
    const half4 imageColor = imageTexture.sample(textureSampler, in.textureCoordinate);
    const float dist = sdfTexture.sample(textureSampler, in.textureCoordinate).a;
    float fillAlpha = smoothstep(0.0, 1.0, dist);
    half4 color = half4(fillAlpha * imageColor.rgb, fillAlpha * imageColor.a);
    return float4(color);
}

fragment float4 flashFragment(RasterizerData  in           [[stage_in]],
                              texture2d<half> colorTexture [[ texture(VertexInputIndexTexture) ]],
                              texture2d<half> sdfTexture [[ texture(VertexInputIndexTextureSDF)]],
                              constant vector_float2 &viewportSize [[buffer(VertexInputIndexViewportSize)]],
                              constant float &timer [[buffer(VertexInputIndexTimer)]]) {
    constexpr sampler textureSampler(mag_filter::linear, min_filter::linear, address::clamp_to_zero);
    half4 fillColor = colorTexture.sample(textureSampler, in.textureCoordinate);
    const float dist = sdfTexture.sample (textureSampler, in.textureCoordinate).a;
    fillColor = half4(1.0);
    
    float fillAlpha = smoothstep(0.45, 0.55, dist);
    half4 color = half4(fillAlpha * fillColor.rgb, fillAlpha * fillColor.a);
    
    float longDuration = 1.5;
    float time = fmod(timer / 50.0, longDuration);
    float innerDuration = 1.2;
    if (time < innerDuration) {
        half3 light = half3(fillColor.rgb) + half3(0.3); /// 光线
        float p = (sin(time * (2 * 3.1415926 / innerDuration) + 3.1415926 * 1.5) + 1.0) * 0.5;
        color += half4(dist * p * light, dist * p); //发光特效
    }
    return float4(color);
}


fragment float4 fireFragment(RasterizerData  in           [[stage_in]],
                             texture2d<half> colorTexture [[ texture(VertexInputIndexTexture) ]],
                             texture2d<half> sdfTexture [[ texture(VertexInputIndexTextureSDF)]],
                             constant vector_float2 &viewportSize [[buffer(VertexInputIndexViewportSize)]],
                             constant float &timer [[buffer(VertexInputIndexTimer)]]) {
    constexpr sampler textureSampler(mag_filter::linear, min_filter::linear);
    const float dist = sdfTexture.sample (textureSampler, in.textureCoordinate).a;
    
    float iTime = timer * 2.0;
    float2 uv = in.textureCoordinate * 10.0;
    
    float fillDist = 1 - pow(dist, 0.15);
    
    float3 col = renderFire(uv, iTime, fillDist);
    col = pow(col, float3(0.4545));
    return float4(col, 1.0);
}

/*
 * 黑边：    0.001 ~ 0.2
 * 火苗范围： 0.2 ～ 0.5
 */
