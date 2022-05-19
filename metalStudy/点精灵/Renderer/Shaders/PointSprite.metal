//
//  PointSprite.metal
//  PointSprite
//
//  Created by 苏莫离 on 2020/10/15.
//  Copyright © 2020 苏莫离. All rights reserved.
//

#include <metal_stdlib>
#include "AAPLShaderTypes.h"
using namespace metal;

struct FPData {
    float4 position [[position]];
    float pointSize [[point_size]];  /// 顶点着色器约束点的大小
};

vertex FPData
vertexShaderFP(uint                     vid           [[ vertex_id ]],
               constant vector_float2   &viewportSize [[buffer(VertexInputViewport)]],
               const device vector_float2 *points     [[ buffer(VertexInputPoint) ]]) {
    
    FPData out;
    float2 position = points[vid].xy / viewportSize * 2.0;
    out.position = float4(position, 0, 1);
    out.pointSize = 50.0;
    return out;
}

fragment float4 fragmentShaderFP(FPData in [[stage_in]],
                                 float2 textureCoord [[point_coord]], /// 片段着色器确定片元的纹理坐标
                                 texture2d<half> colorTexture [[texture(FragmentInputTexture)]]) {
    constexpr sampler textureSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
    const half4 colorSample = colorTexture.sample(textureSampler, textureCoord);
    return float4(colorSample);
}
