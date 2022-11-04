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
    SkyVertexAttributeNormal    = 2,
} SkyVertexAttributes;

typedef enum SkyBufferIndices {
    SkyBufferIndexMeshPositions     = 0,
    SkyBufferIndexMeshGenerics      = 1,
    SkyBufferIndexUniformData       = 3,
} SkyBufferIndices;

typedef enum SkyTextureIndices {
    SkyTextureIndexBaseColor = 0,
} SkyTextureIndices;

#endif
