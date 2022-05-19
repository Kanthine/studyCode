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

/// 2D 随机: 将一个二维向量转化为一维浮点数
float random(float2 position) {
    return fract(sin(dot(position, float2(12.9898,78.233))) * 43758.5453123);
}

float2 random2(float2 position) {
    return fract(sin(float2(dot(position,float2(127.1,311.7)),dot(position,float2(269.5,183.3))))*43758.5453);
}

/// 细胞噪声基于距离场，即到一组特征点中最近一个的距离。
/// 假设我们想要制作一个 4 点的距离场。我们需要做什么？
/// 好吧，对于每个像素，我们要计算到最近点的距离。这意味着我们需要遍历所有点，计算它们到当前像素的距离并存储最接近的点的值。
/// 将空间细分为方形；每个像素都会计算到自己方形中的点与周围8个瓦片的距离；存储最近的距离。结果是一个距离场，类似于以下示例：
fragment float4 fragmentShader(RasterizerData in [[stage_in]],
                               constant vector_float2 &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
                               constant float &timer [[buffer(AAPLVertexInputIndexTimer)]]) {
    float2 postion = in.position.xy / viewportSize;
    float a = postion.y;
    postion.x *= viewportSize.x / viewportSize.y;
    
    float3 color = float3(.0);

    // Scale
    postion *= 9.;

    // Tile the space
    float2 i_st = floor(postion);
    float2 f_st = fract(postion);

    float m_dist = 1.;  // minimum distance

    for (int y= -1; y <= 1; y++) {
        for (int x= -1; x <= 1; x++) {
            // Neighbor place in the grid
            float2 neighbor = float2(float(x),float(y));

            // Random position from current + neighbor place in the grid
            float2 point = random2(i_st + neighbor);

            // Animate the point
            point = 0.5 + 0.5*sin(timer / 100.0 + 6.2831 * point);

            // Vector between the pixel and the point
            float2 diff = neighbor + point - f_st;

            // Distance to the point
            float dist = length(diff);

            // Keep the closer distance
            m_dist = min(m_dist, dist);
        }
    }

    // Draw the min distance (distance field)
    color += m_dist;

    // Draw cell center
    color += 1.-step(.02, m_dist);

    // Draw grid
    color.r += step(.98, f_st.x) + step(.98, f_st.y);
    a = 1 - a;
    if (a < 0) {
        a = 0;
    }
    
    
    return float4(float3(color), a);
}


//uniform vec2 u_resolution; // 画布尺寸（宽，高）
//uniform vec2 u_mouse;      // 鼠标位置（在屏幕上哪个像素）
//uniform float u_time;     // 时间（加载后的秒数）

//uniform vec3 iResolution;   // 视口分辨率（以像素计）
//uniform vec4 iMouse;        // 鼠标坐标 xy： 当前位置, zw： 点击位置
//uniform float iTime;        // shader 运行时间（以秒计）
