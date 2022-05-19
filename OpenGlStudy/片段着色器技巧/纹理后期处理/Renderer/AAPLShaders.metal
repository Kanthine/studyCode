#include <metal_stdlib>
#import "AAPLShaderTypes.h"
using namespace metal;

struct RasterizerData {
    float4 clipSpacePosition [[position]];
    float2 textureCoordinate;
};

vertex RasterizerData
vertexShader(uint                   vertexID       [[ vertex_id ]],
             constant AAPLVertex   *vertexArray    [[ buffer(AAPLVertexInputIndexVertices) ]],
             constant vector_float2 &viewportSize  [[ buffer(AAPLVertexInputIndexViewportSize) ]]) {

    RasterizerData out;
    float2 pixelSpacePosition = vertexArray[vertexID].position.xy;
    out.clipSpacePosition.xy = pixelSpacePosition / (viewportSize / 2.0);
    out.clipSpacePosition.z = 0.0;
    out.clipSpacePosition.w = 1.0;
    out.textureCoordinate = vertexArray[vertexID].textureCoordinate;
    return out;
}

/// fract 函数: 返回一个数的小数部分
fragment float4 samplingShader(RasterizerData  in           [[stage_in]],
                               texture2d<half> colorTexture [[ texture(AAPLTextureIndexOutput) ]]) {
    constexpr sampler textureSampler (mag_filter::linear, min_filter::linear);
    const half4 colorSample = colorTexture.sample (textureSampler, in.textureCoordinate);
    return float4(colorSample);
}


constant float PI = 3.141592657;

float random_Sin(float2 postion) {
    return fract(sin(dot(postion, float2(12.9898,78.233))) * 43758.5453123);
}

float noise(float2 postion) {
    float2 i = floor(postion);
    float2 f = fract(postion);

    // Four corners in 2D of a tile
    float a = random_Sin(i);
    float b = random_Sin(i + float2(1.0, 0.0));
    float c = random_Sin(i + float2(0.0, 1.0));
    float d = random_Sin(i + float2(1.0, 1.0));
    float2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

constant int kFBMLoopCount = 6;
float fbm (float2 postion) {
    // Initial values
    float value = 0.0;
    float amplitude = 0.5;
    
    // Loop of octaves
    for (int i = 0; i < kFBMLoopCount; i++) {
        value += amplitude * noise(postion);
        postion *= 2.0;
        amplitude *= 0.5;
    }
    return value;
}

float fbm_2(float2 postion) {
    float v = 0.0;
    float a = 0.5;
    float2 shift = float2(100.0);
    // Rotate to reduce axial bias
    matrix_float2x2 rot = (matrix_float2x2){{
        { cos(0.5), sin(0.5)},
        {-sin(0.5), cos(0.5)},
    }};
    
    for (int i = 0; i < 5; ++i) {
        v += a * noise(postion);
        postion = rot * postion * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}


/// 水流算法
//kernel void
//grayscaleKernel(texture2d<half, access::read>  inTexture  [[texture(AAPLTextureIndexInput)]],
//                texture2d<half, access::write> outTexture [[texture(AAPLTextureIndexOutput)]],
//                constant vector_float2      &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
//                constant float                 &timer     [[buffer(AAPLVertexInputIndexTimer)]],
//                uint2                          sourceGid  [[thread_position_in_grid]]) {
//
//    if((sourceGid.x >= outTexture.get_width()) || (sourceGid.y >= outTexture.get_height())) {
//        return;
//    }
//
//    float u_time = timer / 5.0;
//
//    float2 postion = float2(sourceGid) / viewportSize;
//    float3 color = float3(0.0);
//    float2 q = float2(0.0);
//    q.x = fbm_2(postion + 0.00 * u_time);
//    q.y = fbm_2(postion + float2(1.0));
//
//    float2 r = float2(0.);
//    r.x = fbm(postion + 1.0 * q + float2(1.7, 9.2) + 0.15 * u_time);
//    r.y = fbm(postion + 1.0 * q + float2(8.3, 2.8) + 0.126 * u_time);
//
//    float f = fbm_2(postion + r);
//
//    color = mix(float3(0.101961,0.619608,0.666667),
//                float3(0.666667,0.666667,0.498039),
//                clamp((f*f)*4.0,0.0,1.0));
//
//    color = mix(color,
//                float3(0,0,0.164706),
//                clamp(length(q),0.0,1.0));
//
//    color = mix(color,
//                float3(0.666667,1,1),
//                clamp(length(r),0.0,1.0));
//    color = (f*f*f+.6*f*f+.5*f)*color;
//
//
//
//    uint2 desGid = sourceGid + uint2(color.xy * viewportSize / 5.0);
//
//
//
//    half4 inColor = inTexture.read(sourceGid);
//    half2 sizeLimit = half2(outTexture.get_width(), outTexture.get_height());
//    desGid -= uint2(sizeLimit * step(sizeLimit, half2(desGid)));
//    outTexture.write(inColor, desGid);
//}
//

/// 极坐标系
//kernel void
//grayscaleKernel(texture2d<half, access::read>  inTexture  [[texture(AAPLTextureIndexInput)]],
//                texture2d<half, access::write> outTexture [[texture(AAPLTextureIndexOutput)]],
//                constant vector_float2      &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
//                constant float                 &timer     [[buffer(AAPLVertexInputIndexTimer)]],
//                uint2                          sourceGid  [[thread_position_in_grid]]) {
//
//    if((sourceGid.x >= outTexture.get_width()) || (sourceGid.y >= outTexture.get_height())) {
//        return;
//    }
//
//    float u_time = timer / 5.0;
//
//    float2 postion = float2(sourceGid) / viewportSize;
//    float2 pos = float2(0.5) - postion;
//    float r = length(pos) * 2.0;
//    float a = atan2(pos.y,pos.x);
//    float f = cos(a * 3.0 + u_time);
//    f = abs(cos(a * 3.0 + u_time));
//    f = abs(cos(a * 2.5 + u_time)) * 0.5 + 0.3;
//    f = abs(cos(a * 12.0 + u_time) * sin(a * 3.0 + u_time)) * 0.8 + 0.1;
//    float3 offet = float3( 1.0 - smoothstep(f, f + 0.02, r));
//
//
//    uint2 desGid = sourceGid + uint2(offet.xy * viewportSize / 5.0);
//    half4 inColor = inTexture.read(sourceGid);
//    half2 sizeLimit = half2(outTexture.get_width(), outTexture.get_height());
//    desGid -= uint2(sizeLimit * step(sizeLimit, half2(desGid)));
//    outTexture.write(inColor, desGid);
//}
//



//kernel void
//grayscaleKernel(texture2d<half, access::read>  inTexture  [[texture(AAPLTextureIndexInput)]],
//                texture2d<half, access::write> outTexture [[texture(AAPLTextureIndexOutput)]],
//                constant vector_float2      &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
//                constant float                 &timer     [[buffer(AAPLVertexInputIndexTimer)]],
//                uint2                          sourceGid  [[thread_position_in_grid]]) {
//
//    if((sourceGid.x >= outTexture.get_width()) || (sourceGid.y >= outTexture.get_height())) {
//        return;
//    }
//
//    float u_time = timer / 5.0;
//
//    float2 postion = float2(sourceGid) / viewportSize;
//    postion.x *= viewportSize.x / viewportSize.y;
//    postion = postion * 2.0 - 1.0; // 将空间重新映射为 [-1, 1]
//    /// 计算点到四象限的距离：abs() 在起作用
//    float d = length(abs(postion) + sin(u_time));
//    float p0 = 0.2 * abs(sin(u_time)) + 0.2;
//    d = length(min(abs(postion) - p0, 0.0));
//    d = length(max(abs(postion) - p0, 0.0));
//    float3 offet = float3(smoothstep(0.3, 0.4, d) * smoothstep(0.6, 0.5, d));
//
//    offet = float3(step(0.3, d) * step(d, 0.4));
//    offet = float3(step(0.3, d));
//    /// 使用 fract() 函数来呈现距离场产生的图案。这个距离场不断重复，就像环一样
//    offet = float3(fract(d * 5.0));
//
//    uint2 desGid = sourceGid + uint2(offet.xy * viewportSize / 5.0);
//    half4 inColor = inTexture.read(sourceGid);
//    half2 sizeLimit = half2(outTexture.get_width(), outTexture.get_height());
//    desGid -= uint2(sizeLimit * step(sizeLimit, half2(desGid)));
//    outTexture.write(inColor, desGid);
//}





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

kernel void
grayscaleKernel(texture2d<half, access::read>  inTexture  [[texture(AAPLTextureIndexInput)]],
                texture2d<half, access::write> outTexture [[texture(AAPLTextureIndexOutput)]],
                constant vector_float2      &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
                constant float                 &timer     [[buffer(AAPLVertexInputIndexTimer)]],
                uint2                          sourceGid  [[thread_position_in_grid]]) {
    
    if((sourceGid.x >= outTexture.get_width()) || (sourceGid.y >= outTexture.get_height())) {
        return;
    }
    
    float u_time = timer / 5.0;
    
    float2 position = float2(sourceGid);
    
    float3 noise = curlNoise(float3(position.x * 0.02, position.y * 0.008, u_time));
    float3 offet = float3(position.x * 2.0, position.y, 1.0) * noise * u_time;
    
    uint2 desGid = sourceGid + uint2(offet.xy * viewportSize / 5.0);
    half4 inColor = inTexture.read(sourceGid);
    half2 sizeLimit = half2(outTexture.get_width(), outTexture.get_height());
    desGid -= uint2(sizeLimit * step(sizeLimit, half2(desGid)));
    outTexture.write(inColor, desGid);
}
