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

/// 电视噪点
//fragment float4 fragmentShader(RasterizerData in [[stage_in]],
//                               constant vector_float2 &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
//                               constant float &timer [[buffer(AAPLVertexInputIndexTimer)]]) {
//    float2 postion = in.position.xy / viewportSize;
//    return float4(random(postion), random(postion * timer / 100), random(postion / timer * 100),1);
//}

/// 灰白斑
//fragment float4 fragmentShader(RasterizerData in [[stage_in]],
//                               constant vector_float2 &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
//                               constant float &timer [[buffer(AAPLVertexInputIndexTimer)]]) {
//    float2 postion = in.position.xy / viewportSize;
//    postion *= 10.0; // 将坐标系缩放10倍
//    float2 ipos = floor(postion);  // 获取坐标的整数部分
//    float2 fpos = fract(postion);  // 获取坐标的小数部分
//
//    //指定一个基于整数坐标的随机值
//    float3 color = float3(random(ipos));
//    //取消注释查看细分网格
////    color = float3(fpos,0.0);
//
//    return float4(color,1);
//}



float2 truchetPattern(float2 position, float index){
    index = fract(((index - 0.5) * 2.0));
    if (index > 0.75) {
        position = float2(1.0) - position;
    } else if (index > 0.5) {
        position = float2(1.0 - position.x, position.y);
    } else if (index > 0.25) {
        position = 1.0 - float2(1.0 - position.x, position.y);
    }
    return position;
}

fragment float4 fragmentShader(RasterizerData in [[stage_in]],
                               constant vector_float2 &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
                               constant float &timer [[buffer(AAPLVertexInputIndexTimer)]]) {
    float2 postion = in.position.xy / viewportSize;
    postion *= 10.0; // 将坐标系缩放10倍
    float2 ipos = floor(postion);  // 获取坐标的整数部分
    float2 fpos = fract(postion);  // 获取坐标的小数部分
    
    float2 tile = truchetPattern(fpos, random(ipos));
    float value = smoothstep(tile.x-0.3,tile.x,tile.y) - smoothstep(tile.x,tile.x+0.3,tile.y); // 迷宫

    // 圆迷宫
    value = (step(length(tile),0.6) - step(length(tile),0.4) ) +
            (step(length(tile-float2(1.)),0.6) - step(length(tile-float2(1.)),0.4) );
        
//    value = step(tile.x,tile.y); // 积木
    return float4(float3(value),1);
}


//uniform vec2 u_resolution; // 画布尺寸（宽，高）
//uniform vec2 u_mouse;      // 鼠标位置（在屏幕上哪个像素）
//uniform float u_time;     // 时间（加载后的秒数）

//uniform vec3 iResolution;   // 视口分辨率（以像素计）
//uniform vec4 iMouse;        // 鼠标坐标 xy： 当前位置, zw： 点击位置
//uniform float iTime;        // shader 运行时间（以秒计）
