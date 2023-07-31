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
    matrix_float4x4 worldMatrix;      /// 物体空间=>世界空间：平移、旋转、缩放等最终形成的复合变换
    matrix_float4x4 cameraMatrix;     /// 摄像机矩阵：世界坐标=>摄像机空间；
    matrix_float4x4 projectionMatrix; /// 投影矩阵：摄像机空间 => 剪裁空间
    vector_float3 cameraPos; // 相机位置
    
    bool isDirectionLight;  /// 是否是方向光
    
    /// 环境光：照射效果与光源位置无关，故无论将光源调节到什么位置、光照效果都相同
    vector_float3 ambient;
    
    /// 散射光：与光源的位置密切相关
    vector_float3 diffuse;
    vector_float3 specular;  /// 镜面光
    vector_float3 lightLocation;  /// 定位光：例如白织灯泡，从某个位置向四周发射光
    vector_float3 lightDirection; /// 定向光：例如太阳光，光照方向平行
    
} Uniforms;

typedef enum kVertexAttributes {
    kVertexAttributePosition  = 0,
    kVertexAttributeNormal    = 1,
} kVertexAttributes;

typedef enum kAttributes {
    kAttributeVertexs   = 0,
    kAttributeNormal    = 1,
    kAttributeUniforms  = 2,
} kAttributes;

#endif
