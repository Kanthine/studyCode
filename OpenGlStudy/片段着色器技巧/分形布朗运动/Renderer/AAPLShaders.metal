#include <metal_stdlib>
#include "AAPLShaderTypes.h"

using namespace metal;

struct RasterizerData {
    float4 position [[position]];
    float4 color;
};

vertex RasterizerData
vertexShader(uint vertexID [[vertex_id]],
             constant AAPLVertex *vertices [[buffer(AAPLVertexInputIndexVertices)]],
             constant vector_float2 &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]]) {
    RasterizerData out;
    float2 pixelSpacePosition = vertices[vertexID].position.xy;
    out.position = vector_float4(0.0, 0.0, 0.0, 1.0);
    out.position.xy = pixelSpacePosition / (viewportSize / 2.0);
    out.color = vertices[vertexID].color;
    return out;
}


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

//fragment float4 fragmentShader(RasterizerData in [[stage_in]],
//                               constant vector_float2 &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
//                               constant float &timer [[buffer(AAPLVertexInputIndexTimer)]]) {
//
//    float2 postion = in.position.xy / viewportSize;
//    postion.x *= viewportSize.x / viewportSize.y;
//    float3 color = float3(fbm(postion * 3.0));
//    return float4(color, 1.0);
//}


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

fragment float4 fragmentShader(RasterizerData in [[stage_in]],
                               constant vector_float2 &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
                               constant float &timer [[buffer(AAPLVertexInputIndexTimer)]]) {
    
    float u_time = timer / 50.0;
    
    float2 postion = in.position.xy / viewportSize;

    float3 color = float3(0.0);

    float2 q = float2(0.);
    q.x = fbm_2(postion + 0.00 * u_time);
    q.y = fbm_2(postion + float2(1.0));

    float2 r = float2(0.);
    r.x = fbm(postion + 1.0 * q + float2(1.7, 9.2) + 0.15 * u_time);
    r.y = fbm(postion + 1.0 * q + float2(8.3, 2.8) + 0.126 * u_time);

    float f = fbm_2(postion + r);

    color = mix(float3(0.101961,0.619608,0.666667),
                float3(0.666667,0.666667,0.498039),
                clamp((f*f)*4.0,0.0,1.0));

    color = mix(color,
                float3(0,0,0.164706),
                clamp(length(q),0.0,1.0));

    color = mix(color,
                float3(0.666667,1,1),
                clamp(length(r),0.0,1.0));
    color = (f*f*f+.6*f*f+.5*f)*color;
    
    return float4(color, 1.0);
}
