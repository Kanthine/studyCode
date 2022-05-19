#include <metal_stdlib>

using namespace metal;

#include "AAPLShaderTypes.h"

struct FairyInOut {
    float4 position [[position]];
    half3 color;
    half2 tex_coord;
};

matrix_float4x4 matrix4x4_rotationY(float angle) {
    return (matrix_float4x4){
        {
            {cos(angle), 0, -sin(angle), 0},
            {         0, 1,           0, 0},
            {sin(angle), 0,  cos(angle), 0},
            {         0, 0,           0, 1}
        }
    };
}

vertex FairyInOut
fairy_vertex(uint                          vid             [[ vertex_id ]],
             uint                          iid             [[ instance_id ]],
             constant AAPLFairyVertex    * vertices        [[ buffer(VertexInputVertex) ]],
             const device PointInfo      * light_data      [[ buffer(VertexInputPoint) ]],
             constant FrameUniforms      & uniforms       [[ buffer(VertexInputUniforms) ]]) {
    
    float rotationRadians = light_data[iid].speed * uniforms.time;  /// step * time
    vector_float4 currentPosition;
    if(light_data[iid].type == PointTypeTree) {
        float lightPeriod = rotationRadians + light_data[iid].position.y;
        lightPeriod -= floor(lightPeriod);  // floor() 获取浮点型的整数部分
        
        float r = 1.2 + 10.0 * pow(lightPeriod, 5.0); /// 幂函数，x 属于 [0, 1]，x 越大，函数值越大
        
        currentPosition.x = light_data[iid].position.x * r;
        currentPosition.y = 200.0f + lightPeriod * 400.0f;
        currentPosition.z = light_data[iid].position.z * r;
        currentPosition.w = 1;
    } else {
        currentPosition = matrix4x4_rotationY(rotationRadians) * light_data[iid].position;
    }
    
    
    FairyInOut out;    
    float3 vertex_position = float3(vertices[vid].position.xy,0);
    float4 fairy_eye_pos = uniforms.worldMatrix * currentPosition;
    fairy_eye_pos = uniforms.cameraMatrix * fairy_eye_pos;
    float4 vertex_eye_position = float4(uniforms.fairy_size * vertex_position + fairy_eye_pos.xyz, 1);
    
    out.position = uniforms.projectionMatrix * vertex_eye_position;
    out.color = half3(light_data[iid].color.xyz);
    //将模型位置(范围为[- 1,1]) 转换为纹理坐标 (范围为[0-1])
    out.tex_coord = 0.5 * (half2(vertices[vid].position.xy) + 1);
    return out;
}


fragment half4
fairy_fragment(FairyInOut in [[ stage_in ]],
               texture2d<half> colorMap [[ texture(FragmentInputTexture) ]]) {
    constexpr sampler linearSampler (mip_filter::linear,
                                     mag_filter::linear,
                                     min_filter::linear);
    half4 c = colorMap.sample(linearSampler, float2(in.tex_coord));
    half3 fragColor = in.color * c.x;
    return half4(fragColor, c.x);
}

