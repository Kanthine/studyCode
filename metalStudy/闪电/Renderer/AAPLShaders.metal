#include <metal_stdlib>
using namespace metal;
#include "AAPLShaderTypes.h"




constant int kLightingBranch = 4; /// 闪电分支
constant float kLightingDuration = 0.8; /// 闪电持续时长

/// 摄像机矩阵变换：物体坐标系 => 摄像机坐标系
float2 lookLighting(float3 rd, float3 eye, float3 up, float3 target) {
    float t = -dot(eye, up) / dot(rd, up);
    float3 its = -eye + rd * t;
    return float2(dot(its, target), dot(its, cross(target, up)));
}

float2 rotate(float2 p, float a) {
    return float2(p.x * cos(a) - p.y * sin(a), p.x * sin(a) + p.y * cos(a));
}

float hash1(float p) {
    return fract(sin(p * 172.435) * 29572.683) - 0.5;
}

float ns(float p) {
    float fr = fract(p);
    float fl = floor(p);
    return mix(hash1(fl), hash1(fl + 1.0), fr);
}

float fbm(float p) {
    return (ns(p) * 0.4 + ns(p * 2.0 - 10.0) * 0.125 + ns(p * 8.0 + 10.0) * 0.025);
}

float fbmd(float p) {
    float h = 0.01;
    return atan2(fbm(p + h) - fbm(p - h), h);
}

float arcsmp(float x, float seed) {
    return fbm(x * 3.0 + seed * 1111.111) * (1.0 - exp(-x * 5.0));
}

float arc(float2 p, float seed, float len) {
    p *= len;
    float v = abs(p.y - arcsmp(p.x, seed));
    v += exp((2.0 - p.x) * -4.0);
    v = exp(v * -60.0) + exp(v * -10.0) * 0.6;
    v *= smoothstep(0.0, 0.05, p.x);
    return v;
}

float arcc(float2 p, float sd) {
    float v = 0.0;
    float rnd = fract(sd);
    float sp = 0.0;
    v += arc(p, sd, 1.0);
    for(int i = 0; i < kLightingBranch; i ++) { /// 闪电分支
        sp = rnd + 0.01;
        float2 mrk = float2(sp, arcsmp(sp, sd));
        v += arc(rotate(p - mrk, fbmd(sp)), mrk.x, mrk.x * 0.4 + 1.5);
        rnd = fract(sin(rnd * 195.2837) * 1720.938);
    }
    return v;
}




struct RasterizerData {
    float4 position [[position]];
};

vertex RasterizerData
vertexShader(uint vertexID [[vertex_id]],
             constant vector_float4 *vertices [[buffer(AAPLInputIndexVertices)]]) {
    RasterizerData out;
    out.position = float4(vertices[vertexID].x * 2.0 - 1, 1.0 - vertices[vertexID].y * 2.0, 0.0, 1.0);
    return out;
}

fragment float4 fragmentShader(RasterizerData in [[stage_in]],
                               constant Uniforms &uniform [[buffer(AAPLInputIndexUniforms)]]) {
        
    float iTime = uniform.timeStep / 2.0;
    float2 position = in.position.xy / uniform.viewportSize * 2.0;
    position.x = position.x - 1.0;
    position.y = (1.0 - position.y);
    float3 positionOffet = normalize(float3(position, 1.0)); /// 位置偏移
    
    float4 rnd = float4(0.1, 0.2, 0.3, 0.4);
    float arcv = 0.0;
    for(int i = 0; i < 2; i++) { /// 360 度均分为 2n 个扇形
        rnd = fract(sin(rnd * 1.111111) * 298729.258972);
        float ts = rnd.z * 4.0 * 1.61803398875 + 1.0;
        
        /// 摄像机坐标系变换
        float arcfl = floor(iTime / ts + rnd.y) * ts;
        float arca = rnd.x + arcfl * 2.39996;
//        positionOffet.xz += 0.2 * cos(arca + 3.14 * 0.4);
//        positionOffet.yz += 0.2 * sin(arca + 3.14 * 0.4);
        
        float3 eye = float3(0.0, 0.0, (1.0 + rnd.x * 12.0) / 2.0 );
        float3 up  = float3(0.0, 0.0, 1.0);
        float3 target = float3(cos(arca), sin(arca), 0.0);
        float2 lightingUV = lookLighting(positionOffet, eye, up, target);
        float arcseed = floor(iTime * 17.0 + rnd.y);
        float arcfr = fract(iTime / ts + rnd.y) * ts;
        float arcdur = rnd.x * 0.2 + kLightingDuration; // 时长
        float arcint = smoothstep(0.1 + arcdur, arcdur, arcfr);
        float v = arcc(float2((lightingUV.x), lightingUV.y * sign(lightingUV.x)) * 1.4, arcseed * 0.033333);
        v *= arcint;
        arcv += v;
    }
    
    float3 col = mix(float3(0.0), float3(1.0), clamp(arcv, 0.0, 1.0));
    return float4(float3(col), 1.0);
}




