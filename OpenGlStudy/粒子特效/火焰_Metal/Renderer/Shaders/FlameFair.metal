//
//  FlameFair.metal
//  DeferredLighting
//
//  Created by i7y on 2022/2/15.
//  Copyright © 2022 Apple. All rights reserved.
//

#include <metal_stdlib>
#include "AAPLShaderTypes.h"
using namespace metal;

/// 粒子噪声
float4 permute(float4 x) {
    x = ((x * 34.0) + 1.0) * x;
    float4 b = float4(289.0, 289.0, 289.0, 289.0);
    return modf(x, b);
}

float4 taylorInvSqrt(float4 r) {
    return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(float3 v){
    const float2 C = float2(1.0/6.0, 1.0/3.0);
    const float4 D = float4(0.0, 0.5, 1.0, 2.0);
    
    // First corner
    float3 i = floor(v + dot(v, C.yyy));
    float3 x0 = v - i + dot(i, C.xxx);
    
    // Other corners
    float3 g = step(x0.yzx, x0.xyz);
    float3 l = 1.0 - g;
    float3 i1 = min(g.xyz, l.zxy);
    float3 i2 = max(g.xyz, l.zxy);
    
    //  x0 = x0 - 0. + 0.0 * C
    float3 x1 = x0 - i1  + 1.0 * C.xxx;
    float3 x2 = x0 - i2  + 2.0 * C.xxx;
    float3 x3 = x0 - 1.0 + 3.0 * C.xxx;
    
    // Permutations
    float3 ibc = float3(289.0, 289.0, 289.0);
    i = modf(i, ibc);
    float4 p = permute(permute(permute(
                i.z + float4(0.0, i1.z, i2.z, 1.0))
                + i.y + float4(0.0, i1.y, i2.y, 1.0))
                + i.x + float4(0.0, i1.x, i2.x, 1.0));
                
    // Gradients
    // ( N*N points uniformly over a square, mapped onto an octahedron.)
    float n_= 1.0 / 7.0;// N=7
    float3 ns = n_* D.wyz-D.xzx;

    float4 j = p-49.*floor(p*ns.z*ns.z);//  mod(p,N*N)

    float4 x_=floor(j*ns.z);
    float4 y_=floor(j-7.*x_);// mod(j,N)

    float4 x=x_*ns.x+ns.yyyy;
    float4 y=y_*ns.x+ns.yyyy;
    float4 h=1.-abs(x)-abs(y);

    float4 b0=float4(x.xy,y.xy);
    float4 b1=float4(x.zw,y.zw);

    float4 s0=floor(b0)*2.+1.;
    float4 s1=floor(b1)*2.+1.;
    float4 sh=-step(h,float4(0.));
                
    float4 a0=b0.xzyw+s0.xzyw*sh.xxyy;
    float4 a1=b1.xzyw+s1.xzyw*sh.zzww;
    
    float3 p0=float3(a0.xy,h.x);
    float3 p1=float3(a0.zw,h.y);
    float3 p2=float3(a1.xy,h.z);
    float3 p3=float3(a1.zw,h.w);
    
    //Normalise gradients
    float4 norm=taylorInvSqrt(float4(dot(p0,p0),dot(p1,p1),dot(p2,p2),dot(p3,p3)));
    p0*=norm.x;
    p1*=norm.y;
    p2*=norm.z;
    p3*=norm.w;
    
    // Mix final noise value
    float4 m=max(.6-float4(dot(x0,x0),dot(x1,x1),dot(x2,x2),dot(x3,x3)),0.);
    m=m*m;
    return 42.*dot(m*m,float4(dot(p0,x0),dot(p1,x1),
    dot(p2,x2),dot(p3,x3)));
}

float3 snoiseVec3( float3 x ){
    float s  = snoise(float3( x ));
    float s1 = snoise(float3( x.y - 19.1 , x.z + 33.4 , x.x + 47.2 ));
    float s2 = snoise(float3( x.z + 74.2 , x.x - 124.5 , x.y + 99.4 ));
    float3 c = float3( s , s1 , s2 );
    return c;
}

float3 curlNoise( float3 p ){
    const float e = .1;
    float3 dx = float3( e   , 0.0 , 0.0 );
    float3 dy = float3( 0.0 , e   , 0.0 );
    float3 dz = float3( 0.0 , 0.0 , e   );
    float3 p_x0 = snoiseVec3( p - dx );
    float3 p_x1 = snoiseVec3( p + dx );
    float3 p_y0 = snoiseVec3( p - dy );
    float3 p_y1 = snoiseVec3( p + dy );
    float3 p_z0 = snoiseVec3( p - dz );
    float3 p_z1 = snoiseVec3( p + dz );
    float x = p_y1.z - p_y0.z - p_z1.y + p_z0.y;
    float y = p_z1.x - p_z0.x - p_x1.z + p_x0.z;
    float z = p_x1.y - p_x0.y - p_y1.x + p_y0.x;
    const float divisor = 1.0 / ( 2.0 * e );
    return normalize( float3( x , y , z ) * divisor );
}



static constant float kFairyRadius = 1;

struct FPData {
    float4 position [[position]];
    float2 textureCoord;
    float sjFactor; /// 用于传递给片元着色器的总衰减因子
    float life;
};

vertex FPData
vertexShaderFP(uint                     vid           [[ vertex_id ]],
               uint                     iid           [[ instance_id ]],
               constant vector_float2   &viewportSize [[ buffer(VertexInputViewportSize) ]],
               constant PEUniform &uniforms [[buffer(5)]],
               constant AAPLFairyVertex * vertices    [[ buffer(VertexInputVertex) ]],
               const device PointInfo   * points      [[ buffer(VertexInputPoint) ]]) {
    
    FPData out;
    out.life = points[iid].life;
    out.textureCoord = 0.5 * (float2(vertices[vid].position.xy) + 1);
    out.sjFactor = (5.0 - out.life) / 5.0;
    
    float2 vertex_position = vertices[vid].position.xy;
    float2 fairy_eye_pos = points[iid].position.xy;
    float2 vertex_eye_position = fairy_eye_pos + kFairyRadius * vertex_position;
    vertex_eye_position = vertex_eye_position / viewportSize * 2.0;
    
    float3 position = float3(vertex_eye_position, 0);
    float3 noise = curlNoise(float3(position.x*.02,position.y*.008,uniforms.uTime * 0.05));
    float3 distortion = float3(position.x*2.,position.y,1.) * noise * uniforms.uProgress;
    float3 newPos = position + distortion;
    out.position = float4(newPos, 1);

//    out.position = float4(vertex_eye_position, 0, 1);
    return out;
}

fragment float4 fragmentShaderFP(FPData in [[stage_in]],
                                 texture2d<half> colorTexture [[ texture(FragmentInputTexture) ]]) {
//    if (in.life == 10) { /// 该片元的生命期为 10.0 时，处于未激活状态，不绘制
//        return float4(0, 0, 0, 0);
//    }
//    
    constexpr sampler textureSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
    
    const half4 colorSample = colorTexture.sample(textureSampler, in.textureCoord);
    float4 fragColor = float4(0, 0, 0, 0);
    float disT = distance(in.position.xyz, float3(0.0, 0.0, 0.0));
    float tampFactor = abs((1.0 - disT / kFairyRadius)) * in.sjFactor; // 计算片元颜色插值因子
    float4 factor4 = float4(tampFactor, tampFactor, tampFactor, tampFactor);
    fragColor = clamp(factor4, {0, 0, 0, 0}, {0.7569f,0.2471f,0.1176f,1.0f});
    fragColor = fragColor * colorSample.a;
    return fragColor;
}
