//
//  NoiseFunc.c
//  Noise
//
//  Created by wyl on 2022/1/25.
//  Copyright © 2022 Apple. All rights reserved.
//

#include "NoiseFunc.h"

/// 粒子噪声

vector_float4 permute(vector_float4 x) {
    x = ((x * 34.0) + 1.0) * x;
    float b = 289.0;
    return (vector_float4){modff(x.x, &b), modff(x.y, &b), modff(x.z, &b), modff(x.w, &b)};
}

vector_float4 taylorInvSqrt(vector_float4 r) {
    return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vector_float3 v) {
    const vector_float2 C = (vector_float2){1.0/6.0, 1.0/3.0};
    const vector_float4 D = (vector_float4){0.0, 0.5, 1.0, 2.0};
    
    // First corner
    vector_float3 i = v + simd_dot(v, C.yyy);
    i = (vector_float3){floor(i.x), floor(i.y), floor(i.z)};
    vector_float3 x0 = v - i + simd_dot(i, C.xxx);
    
    // Other corners
    vector_float3 g = simd_step(x0.yzx, x0.xyz);
    vector_float3 l = 1.0 - g;
    vector_float3 i1 = simd_min(g.xyz, l.zxy);
    vector_float3 i2 = simd_max(g.xyz, l.zxy);
    
    //  x0 = x0 - 0. + 0.0 * C
    vector_float3 x1 = x0 - i1  + 1.0 * C.xxx;
    vector_float3 x2 = x0 - i2  + 2.0 * C.xxx;
    vector_float3 x3 = x0 - 1.0 + 3.0 * C.xxx;
    
    // Permutations
    float ibc = 289.0;
    i = (vector_float3){modff(i.x, &ibc), modff(i.y, &ibc), modff(i.z, &ibc)};
    vector_float4 p = permute(permute(permute(
                i.z + simd_make_float4(0.0, i1.z, i2.z, 1.0))
                + i.y + simd_make_float4(0.0, i1.y, i2.y, 1.0))
                + i.x + simd_make_float4(0.0, i1.x, i2.x, 1.0));
                
    // Gradients
    // ( N*N points uniformly over a square, mapped onto an octahedron.)
    float n_= 1.0 / 7.0;// N=7
    vector_float3 ns = n_* D.wyz-D.xzx;

    vector_float4 j = p-49.*floor(p*ns.z*ns.z);//  mod(p,N*N)

    vector_float4 x_ = floor(j * ns.z);
    vector_float4 y_ = floor(j - 7. * x_);// mod(j,N)

    vector_float4 x = x_*ns.x+ns.yyyy;
    vector_float4 y = y_*ns.x+ns.yyyy;
    vector_float4 h = 1.-simd_abs(x)-simd_abs(y);
    vector_float4 b0 = simd_make_float4(x.xy,y.xy);
    vector_float4 b1 = simd_make_float4(x.zw,y.zw);
    vector_float4 s0=floor(b0)*2.+1.;
    vector_float4 s1=floor(b1)*2.+1.;
    vector_float4 sh=-simd_step(h,simd_make_float4(0.));
                
    vector_float4 a0=b0.xzyw+s0.xzyw*sh.xxyy;
    vector_float4 a1=b1.xzyw+s1.xzyw*sh.zzww;
    
    vector_float3 p0 = simd_make_float3(a0.xy,h.x);
    vector_float3 p1 = simd_make_float3(a0.zw,h.y);
    vector_float3 p2 = simd_make_float3(a1.xy,h.z);
    vector_float3 p3 = simd_make_float3(a1.zw,h.w);
    
    //Normalise gradients
    vector_float4 norm=taylorInvSqrt(simd_make_float4(simd_dot(p0,p0),simd_dot(p1,p1),simd_dot(p2,p2),simd_dot(p3,p3)));
    p0*=norm.x;
    p1*=norm.y;
    p2*=norm.z;
    p3*=norm.w;
    
    // Mix final noise value
    vector_float4 m=simd_max(.6-simd_make_float4(simd_dot(x0,x0),simd_dot(x1,x1),simd_dot(x2,x2),simd_dot(x3,x3)),0.);
    m=m*m;
    
    return 42.0 * simd_dot(m*m,simd_make_float4(simd_dot(p0,x0),simd_dot(p1,x1), simd_dot(p2,x2),simd_dot(p3,x3)));
}

//float3 snoiseVec3( float3 x ){
//    float s  = snoise(float3( x ));
//    float s1 = snoise(float3( x.y - 19.1 , x.z + 33.4 , x.x + 47.2 ));
//    float s2 = snoise(float3( x.z + 74.2 , x.x - 124.5 , x.y + 99.4 ));
//    float3 c = float3( s , s1 , s2 );
//    return c;
//}

//float3 curlNoise( float3 p ){
//    const float e = .1;
//    float3 dx = float3( e   , 0.0 , 0.0 );
//    float3 dy = float3( 0.0 , e   , 0.0 );
//    float3 dz = float3( 0.0 , 0.0 , e   );
//    float3 p_x0 = snoiseVec3( p - dx );
//    float3 p_x1 = snoiseVec3( p + dx );
//    float3 p_y0 = snoiseVec3( p - dy );
//    float3 p_y1 = snoiseVec3( p + dy );
//    float3 p_z0 = snoiseVec3( p - dz );
//    float3 p_z1 = snoiseVec3( p + dz );
//    float x = p_y1.z - p_y0.z - p_z1.y + p_z0.y;
//    float y = p_z1.x - p_z0.x - p_x1.z + p_x0.z;
//    float z = p_x1.y - p_x0.y - p_y1.x + p_y0.x;
//    const float divisor = 1.0 / ( 2.0 * e );
//    return normalize( float3( x , y , z ) * divisor );
//}

