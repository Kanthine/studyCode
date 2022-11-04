//
//  GlobalType.h
//  HelloTriangle
//
//  Created by wyl on 2018/8/23.
//  Copyright © 2018 Apple. All rights reserved.
//

#ifndef GlobalType_h
#define GlobalType_h

#include <simd/simd.h>

/// 保存常量数据
typedef struct {
    matrix_float4x4 worldMatrix; /// 物体空间=>世界空间：平移、旋转、缩放等最终形成的复合变换
    matrix_float4x4 cameraMatrix; ///  世界坐标=>摄像机空间：摄像机矩阵
    matrix_float4x4 projectionMatrix; /// 摄像机空间 => 剪裁空间：投影矩阵
} Uniforms;

typedef enum SkyVertexAttributes {
    SkyVertexAttributePosition  = 0,
    SkyVertexAttributeNormal    = 1,
    SkyVertexAttributeLightData = 2,
} SkyVertexAttributes;

typedef enum SkyBufferIndices {
    SkyBufferIndexMeshPositions     = 10,
    SkyBufferIndexMeshGenerics      = 11,
    SkyBufferIndexUniformData       = 13,
} SkyBufferIndices;

// Per-light characteristics
typedef struct
{
    vector_float3 light_color;
    float light_radius;
    float light_speed;
} AAPLPointLight;

typedef enum AAPLBufferIndices {
    AAPLBufferIndexMeshPositions     = 0,
    AAPLBufferIndexMeshGenerics      = 1,
    AAPLBufferIndexFrameData         = 2,
    AAPLBufferIndexLightsData        = 3,
    AAPLBufferIndexLightsPosition    = 4,

} AAPLBufferIndices;

typedef enum SkyTextureIndices {
    SkyTextureIndexBaseColor = 0,
    SkyTextureIndexAlpha     = 1,
} SkyTextureIndices;

#endif
