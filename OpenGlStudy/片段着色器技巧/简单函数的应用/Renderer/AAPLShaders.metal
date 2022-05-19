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

/// 周期函数：如 sin() 函数 cos()函数 等，使颜色做周期性变换
//fragment float4 fragmentShader(RasterizerData in [[stage_in]],
//                               constant vector_float2 &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
//                               constant float &timer [[buffer(AAPLVertexInputIndexTimer)]]) {
//    float2 postion = in.position.xy / viewportSize;
//    return float4(postion.x, postion.y, abs(sin(timer / 100.0)),1);
//}

/// 把 X 或 Y 的值映射到红色或者绿色通道
//fragment float4 fragmentShader(RasterizerData in [[stage_in]],
//                               constant vector_float2 &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
//                               constant float &timer [[buffer(AAPLVertexInputIndexTimer)]]) {
//
//    float2 postion = in.position.xy / viewportSize;
//    return float4(postion,0,1);
//}


/** float smoothstep(float edge0, float edge1, float x) 平滑阶梯函数
 *  主要用来在两个值之间 (0~1) 进行平滑过渡
 *  @param edge0, edge1 在这两个值之间平滑过度
 *  @return x <= edge0 则返回 0.0
 *          x >= edge1 则返回 1.0
 *   edge0 < x < edge1 返回 0 与 1 之间平滑插值
 *
 *   smoothstep(0.01, 0.0, 0.0)   => 1.000000
 *   smoothstep(0.01, 0.0, 0.2)   => 0.000000
 *   smoothstep(0.01, 0.0, 0.009) => 0.028000
 *
 *   smoothstep(0.0, 0.01, 0.0)   => 0.000000
 *   smoothstep(0.0, 0.01, 0.2)   => 1.000000
 *   smoothstep(0.0, 0.01, 0.009) => 0.972000
 */

// 用 0 - 1.0 之间的值在Y上画一条直线
//float plot(float2 postion) {
//    return smoothstep(0.01, 0.0, abs(postion.y - postion.x));
//}
//
//fragment float4 fragmentShader(RasterizerData in [[stage_in]],
//                               constant vector_float2 &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
//                               constant float &timer [[buffer(AAPLVertexInputIndexTimer)]]) {
//
//    float2 postion = in.position.xy / viewportSize;
//
//    float y = postion.x;
////    return float4(y, y, y, 1); /// x 轴的渐变色
//
//    float pct = plot(postion);
////    return float4(pct, pct, pct, 1); /// 一条线
//
//    float3 color = float3(y);
//    color = (1.0 - pct) * color + pct * float3(1.0, 0.0, 0.0);
//    return float4(color, 1);
//}

// 用 0 - 1.0 之间的值在Y上画一条曲线
//float plot(float2 postion, float pct){
//    return smoothstep(pct - 0.01, pct, postion.y) - smoothstep(pct, pct + 0.01, postion.y);
//}
//
//fragment float4 fragmentShader(RasterizerData in [[stage_in]],
//                               constant vector_float2 &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
//                               constant float &timer [[buffer(AAPLVertexInputIndexTimer)]]) {
//    float2 postion = in.position.xy / viewportSize;
//
//    /// 光晕
//    ///return smoothstep(0.5, 0.2, length(postion - 0.5));
//
//    
//    float y = pow(postion.x, 5.0);
////    y = smoothstep(0.1, 0.9, postion.x);
////    y = sin((postion.x + timer / 200.0) * 6.28 * 2) / 4.0 + 0.5; /// sin 曲线运动
//    float3 color = float3(y);
//    float pct = plot(postion, y);
//    color = (1.0 - pct) * color + pct * float3(0.0,1.0,0.0);
//    return float4(pct, pct, pct, 1);
//}

/// 使用 step() 函数替代逻辑运算符 if
//fragment float4 fragmentShader(RasterizerData in [[stage_in]],
//                               constant vector_float2 &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
//                               constant float &timer [[buffer(AAPLVertexInputIndexTimer)]]) {
//
//    float2 postion = in.position.xy / viewportSize;
//
//
////    // x < edge => 0
////    float left =   step(0.1,postion.x) * (1 - step(0.9,postion.x));
////    float bottom = step(0.1,postion.y) * (1 - step(0.9,postion.y));
////    // left * bottom 类似于 &&
////    color = float3(left * bottom);
//
//    float2 mark = step(0.1, postion) * (1 - step(0.9, postion));
//    float3 color = float3(mark.x * mark.y);
//    return float4(color, 1);
//}


/// 圆
//fragment float4 fragmentShader(RasterizerData in [[stage_in]],
//                               constant vector_float2 &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
//                               constant float &timer [[buffer(AAPLVertexInputIndexTimer)]]) {
//
//    float2 postion = in.position.xy / viewportSize;
//
//    /// 不同效果
//    float pct = 0;
//    pct = distance(postion,float2(0.4)) + distance(postion,float2(0.6));
//    pct = distance(postion,float2(0.4)) * distance(postion,float2(0.6));
//    pct = min(distance(postion,float2(0.4)),distance(postion,float2(0.6)));
//    pct = max(distance(postion,float2(0.4)),distance(postion,float2(0.6)));
//    pct = pow(distance(postion,float2(0.4)),distance(postion,float2(0.6)));
//    pct = step(0.2 + 0.1 * sin(timer / 50.0), distance(postion, float2(0.5)));
//    pct = smoothstep(0.0, 0.1 + 0.1 * sin(timer / 100.0), distance(postion, float2(0.5)));
//
//    return float4(pct, pct, pct, 1);
//}

/// 等高线
//fragment float4 fragmentShader(RasterizerData in [[stage_in]],
//                               constant vector_float2 &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
//                               constant float &timer [[buffer(AAPLVertexInputIndexTimer)]]) {
//
//    float2 postion = in.position.xy / viewportSize;
//    postion.x *= viewportSize.x / viewportSize.y;
//    // 将空间重新映射为 [-1, 1]
//    postion = postion * 2.0 - 1.0;
//
//    /// 计算点到四象限的距离：abs() 在起作用
//    float d = length(abs(postion) + sin(timer / 50.0));
//    float p0 = 0.2 * abs(sin(timer / 50.0)) + 0.2;
//    d = length(min(abs(postion) - p0, 0.0));
//    d = length(max(abs(postion) - p0, 0.0));
//
//    return float4(float3(smoothstep(0.3, 0.4, d) * smoothstep(0.6, 0.5, d)), 1.0);
//    return float4(float3(step(0.3, d) * step(d, 0.4)), 1.0);
//    return float4(float3(step(0.3, d)), 1.0);
//
//
//    /// 使用 fract() 函数来呈现距离场产生的图案。这个距离场不断重复，就像环一样
//    return float4(float3(fract(d * 10.0)), 1.0);
//}


/// 极坐标系
fragment float4 fragmentShader(RasterizerData in [[stage_in]],
                               constant vector_float2 &viewportSize [[buffer(AAPLVertexInputIndexViewportSize)]],
                               constant float &timer [[buffer(AAPLVertexInputIndexTimer)]]) {

    float2 postion = in.position.xy / viewportSize;
    
    float2 pos = float2(0.5) - postion;
    float r = length(pos) * 2.0;
    float a = atan2(pos.y,pos.x);
    float rT = timer / 50.0;
    float f = cos(a * 3.0 + rT);
//    f = abs(cos(a * 3.0 + rT));
//    f = abs(cos(a * 2.5 + rT)) * 0.5 + 0.3;
//    f = abs(cos(a * 12.0 + rT) * sin(a * 3.0 + rT)) * 0.8 + 0.1;
//    f = smoothstep(-0.5, 1.0, cos(a * 10.0 + rT)) * 0.2 + 0.5;
    
    float3 color = float3( 1.0 - smoothstep(f, f + 0.02, r));
    return float4(color, 1.0);
}


//uniform vec2 u_resolution; // 画布尺寸（宽，高）
//uniform vec2 u_mouse;      // 鼠标位置（在屏幕上哪个像素）
//uniform float u_time;     // 时间（加载后的秒数）

//uniform vec3 iResolution;   // 视口分辨率（以像素计）
//uniform vec4 iMouse;        // 鼠标坐标 xy： 当前位置, zw： 点击位置
//uniform float iTime;        // shader 运行时间（以秒计）
