#include <metal_stdlib>
#import "AAPLShaderTypes.h"
using namespace metal;

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

/// 片段着色器
fragment float4 samplingShader(RasterizerData  in           [[stage_in]],
                               texture2d<half> colorTexture [[ texture(TextureIndexOutput) ]]) {
    constexpr sampler textureSampler(mag_filter::linear, min_filter::linear); /// 采样器
    const half4 colorSample = colorTexture.sample (textureSampler, in.textureCoordinate); // 对纹理采样
    return float4(colorSample);
}



/// 用于获取两个距离之间的差异
float differenceOp(float d0, float d1) {
    return max(d0, -d1);
}

/// 用于确定给定的点是在内部还是外部
float distanceToRect( float2 point, float2 center, float2 size ) {
    point -= center;    /// 用于抵消给定当前坐标的中心位置
    point = abs(point); /// 得到给定点的对称坐标
    point -= size / 2.; /// 得到距离任何边缘的距离
    return max(point.x, point.y); /// 用来获取与场景中任意对象之间的最近的距离
}

float distanceToScene( float2 point ) {
    float d2r1 = distanceToRect( point, float2(0.), float2(0.45, 0.85) );
    float2 mod = point - 0.1 * floor(point / 0.1);
    float d2r2 = distanceToRect( mod, float2( 0.05 ), float2(0.02, 0.04) );
    float diff = differenceOp(d2r1, d2r2);
    return diff;
}

float getShadow(float2 point, float2 lightPos) {
    
    float2 lightDir = normalize(lightPos - point); // 从点到光源的方向
    float dist2light = length(lightDir); /// 点到光源之间的距离
    
    float distAlongRay = 0.0;
    for (float i = 0.0; i < 80.; i++) {
        float2 currentPoint = point + lightDir * distAlongRay;
        float d2scene = distanceToScene(currentPoint);
        if (d2scene <= 0.001) {
            return 0.0;
        }
        distAlongRay += d2scene;
        if (distAlongRay > dist2light) {
            break;
        }
    }
    return 1.;
}




kernel void
grayscaleKernel(texture2d<half, access::write> outTexture [[texture(TextureIndexOutput)]],
                constant float &timer [[buffer(TextureIndexTimer)]],
                uint2                          gid        [[thread_position_in_grid]]) {
    int width = outTexture.get_width();
    int height = outTexture.get_height();
    float2 uv = float2(gid) / float2(width, height);
    uv = uv * 2.0 - 1.0;
    float d2scene = distanceToScene(uv);
    bool i = d2scene < 0.0;
    
    /// 1、画出基本形状
    float4 color = i ? float4( .1, .5, .5, 1. ) : float4( .7, .8, .8, 1. );
    
    /// 2、增加光照效果
    float2 lightPos = float2(1.3 * sin(timer), 1.3 * cos(timer));
    float dist2light = length(lightPos - uv); /// 获得和光线之间的距离
    color *= max(0.0, 2. - dist2light ); /// max()函数：为了避免灯光的亮度出现负数
    
    /// 3、增加阴影效果
    float shadow = getShadow(uv, lightPos);
    shadow = shadow * 0.5 + 0.5; // 使用0.5来减弱阴影效果，也可以使用其他的数值
    color *= shadow;
    outTexture.write(half4(color), gid);
}
