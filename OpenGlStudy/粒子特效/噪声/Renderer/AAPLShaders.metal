#include <metal_stdlib>
#import "AAPLShaderTypes.h"
using namespace metal;

struct RasterizerData {
    float4 clipSpacePosition [[position]];
    float2 textureCoordinate;
};

vertex RasterizerData
vertexShader(uint                   vertexID             [[ vertex_id ]],
             constant AAPLVertex   *vertexArray          [[ buffer(AAPLVertexInputIndexVertices) ]],
             constant vector_float2 *viewportSizePointer  [[ buffer(AAPLVertexInputIndexViewportSize) ]]) {
    RasterizerData out;
    float2 pixelSpacePosition = vertexArray[vertexID].position.xy;
    float2 viewportSize = float2(*viewportSizePointer);
    out.clipSpacePosition.xy = pixelSpacePosition / (viewportSize / 2.0);
    out.clipSpacePosition.z = 0.0;
    out.clipSpacePosition.w = 1.0;
    out.textureCoordinate = vertexArray[vertexID].textureCoordinate;
    return out;
}

fragment float4 samplingShader(RasterizerData  in           [[stage_in]],
                               texture2d<half> colorTexture [[ texture(AAPLTextureIndexOutput) ]]) {
    constexpr sampler textureSampler (mag_filter::linear, min_filter::linear);
    const half4 colorSample = colorTexture.sample (textureSampler, in.textureCoordinate);
    return float4(colorSample);
}





float random(float2 p) { /// [0,1] 之间的随机数
    return fract(sin(dot(p, float2(15.79, 81.93)) * 45678.9123));
}

/// 将双线性插值格子（网格）并返回平滑值。 双线性插值允许我们将1D随机函数转换为基于2D网格的值：
float noise(float2 p) {
    float2 i = floor(p);
    float2 f = fract(p);
    f = f * f * (3.0 - 2.0 * f);
    float bottom = mix(random(i + float2(0)), random(i + float2(1.0, 0.0)), f.x);
    float top = mix(random(i + float2(0.0, 1.0)), random(i + float2(1)), f.x);
    float t = mix(bottom, top, f.y);
    return t;
}

/// 云雾实现
float fbm_mist(float2 uv) {
    uv *= 3.0;
    float sum = 0;
    float amp = 0.7;
    for(int i = 0; i < 50; ++i) {
        sum += noise(uv) * amp;
        uv += uv * 1.2;
        amp *= 0.4;
    }
    return sum;
}

/// 分形布朗运动，简称fbm，是通过将不同频率和振幅的噪声函数进行操作，
/// 最常用的方法是：将频率乘2的倍数，振幅除2的倍数，线性相加。
///  fbm = noise(st) + 0.5 * noise(2*st) + 0.25 * noise(4*st)
float fbm5(float2 uv) {
    float sum = 0;
    uv *= 4.0;
    float amp = 1.0;
    for(int i = 0; i < 5; i++) {
        sum += noise(uv) * amp;
        uv *= 4.0;
        amp /= 4.0;
    }
    return sum;
}

/// 翘曲域噪声用来模拟卷曲、螺旋状的纹理，比如烟雾、大理石等，实现公式如下：
/// f(p) = fbm( p + fbm( p + fbm( p ) ) )
float domain_wraping(float2 uv) {
    float2 q = float2(fbm5(uv), fbm5(uv));
    float2 r = float2(fbm5(uv + q), fbm5(uv + q));
    return fbm5(uv + r);
}

kernel void compute(texture2d<float, access::write> output [[texture(AAPLTextureIndexOutput)]],
                    constant float &timer [[buffer(0)]],
                    uint2 gid [[thread_position_in_grid]]) {
    
    if((gid.x >= output.get_width()) || (gid.y >= output.get_height())) return;
    
    int width = output.get_width();
    int height = output.get_height();
    float2 uv = float2(gid) / float2(width, height);
    uv = uv * 2.0 - 1.0;
    uv = fmod(uv + float2(0, timer * 0.2), float2(width, height));
    float t = fbm_mist(uv);
    output.write(float4(float3(t), 0.5), gid);
    
//    float radius = 1.0;
//    float distance = length(uv) - radius;
//    output.write(distance < 0 ? float4(float3(t), 1) : float4(0), gid); /// 圆形
}
