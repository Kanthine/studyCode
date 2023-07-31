#ifndef AAPLShaderTypes_h
#define AAPLShaderTypes_h

#include <simd/simd.h>


// Attribute index values shared between shader and C code to ensure Metal shader vertex
//   attribute indices match the Metal API vertex descriptor attribute indices
typedef enum AAPLVertexAttributes
{
    AAPLVertexAttributePosition  = 0,
    AAPLVertexAttributeTexcoord  = 1,
    AAPLVertexAttributeNormal    = 2,
    AAPLVertexAttributeTangent   = 3,
    AAPLVertexAttributeBitangent = 4
} AAPLVertexAttributes;

// Buffer index values shared between shader and C code to ensure Metal shader buffer inputs match
//   Metal API buffer set calls
typedef enum AAPLBufferIndices
{
    AAPLBufferIndexMeshPositions     = 0,
    AAPLBufferIndexMeshGenerics      = 1,
    AAPLBufferIndexFrameData         = 2,
    AAPLBufferIndexLightsData        = 3,
    AAPLBufferIndexLightsPosition    = 4,

} AAPLBufferIndices;


typedef enum AAPLTextureIndices {
    AAPLTextureIndexBaseColor = 0,
    AAPLTextureIndexSpecular  = 1,
    AAPLTextureIndexNormal    = 2,
    AAPLTextureIndexShadow    = 3,
    AAPLTextureIndexAlpha     = 4,

    AAPLNumMeshTextures = AAPLTextureIndexNormal + 1

} AAPLTextureIndices;

typedef enum AAPLVertexInputIndex {
    AAPLVertexInputIndexUniforms     = 2,
} AAPLVertexInputIndex;


/// 保存常量数据
typedef struct {
    matrix_float4x4 worldMatrix; /// 物体空间=>世界空间：平移、旋转、缩放等最终形成的复合变换
    matrix_float4x4 cameraMatrix; ///  世界坐标=>摄像机空间：摄像机矩阵
    matrix_float4x4 projectionMatrix; /// 摄像机空间 => 剪裁空间：投影矩阵
} Uniforms;

#endif
