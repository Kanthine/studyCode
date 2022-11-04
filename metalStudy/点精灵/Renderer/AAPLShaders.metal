//
//  PointSprite.metal
//  PointSprite
//
//  Created by wyl on 2020/10/15.
//  Copyright © 2020 wyl. All rights reserved.
//

#include <metal_stdlib>
#include "AAPLShaderTypes.h"
using namespace metal;

struct FPData {
    float4 position [[position]];
    float pointSize [[point_size]];
    int textureID;
    float scale; /// width/height
};

vertex FPData
vertexShader(uint                     vid      [[ vertex_id ]],
            const device vector_float4 *points [[ buffer(VertexInputPoint) ]]) {
    
    FPData out;
    out.textureID = points[vid].w;
    out.scale = points[vid].z;
    out.pointSize = 200.0;
    
    float2 position = points[vid].xy * 2.0;
    position.x = position.x - 1.0;
    position.y = 1.0 - position.y;
    out.position = float4(position, 0, 1);
    
    return out;
}

fragment float4 fragmentShader(FPData in [[stage_in]],
                               float2 textureCoord [[point_coord]],
                               texture2d<half> texture_1 [[texture(FragmentInputTexture_1)]],
                               texture2d<half> texture_2 [[texture(FragmentInputTexture_2)]],
                               texture2d<half> texture_3 [[texture(FragmentInputTexture_3)]]) {
    constexpr sampler textureSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);

    half4 colorSample = half4(0.0);
    if (in.textureID == 1) {
        colorSample = texture_1.sample(textureSampler, textureCoord);
    } else if (in.textureID == 2) {
        colorSample = texture_2.sample(textureSampler, textureCoord);
    } else {
        colorSample = texture_3.sample(textureSampler, textureCoord);
    }
    return float4(colorSample);
}
