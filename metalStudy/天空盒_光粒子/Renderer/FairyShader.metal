//
//  FairyShader.metal
//  HelloTriangle
//
//  Created by wyl on 2018/8/23.
//  Copyright © 2018 Apple. All rights reserved.
//

#include <metal_stdlib>
#include "GlobalType.h"
using namespace metal;

struct FairyInOut {
    float4 position [[position]];
    half3 color;
    float pointSize [[point_size]];
};

vertex FairyInOut fairy_vertex(uint                          vid             [[ vertex_id ]],
                               const device AAPLPointLight * light_data      [[ buffer(AAPLBufferIndexLightsData) ]],
                               const device vector_float4  * light_positions [[ buffer(AAPLBufferIndexLightsPosition) ]],
                               constant Uniforms &uniformData [[buffer(SkyBufferIndexUniformData)]]) {
    FairyInOut out;
    float4 fairy_eye_pos = light_positions[vid];
    float4 vertex_eye_position = float4(fairy_eye_pos.xyz, 1);
    out.position =  uniformData.projectionMatrix * uniformData.cameraMatrix * uniformData.worldMatrix * vertex_eye_position;
    out.pointSize = 35.0;
    out.color = half3(light_data[vid].light_color.xyz);
    return out;
}

fragment half4 fairy_fragment(FairyInOut      in       [[ stage_in ]],
                              float2 textureCoord [[point_coord]],
                              texture2d<half> colorMap [[ texture(SkyTextureIndexBaseColor) ]]) {
    constexpr sampler linearSampler (mip_filter::linear, mag_filter::linear, min_filter::linear);
    half4 c = colorMap.sample(linearSampler, textureCoord);
    half3 fragColor = in.color * c.x;
    return half4(fragColor, c.x);
}
