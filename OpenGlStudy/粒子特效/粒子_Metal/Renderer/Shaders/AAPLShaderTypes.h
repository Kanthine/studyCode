#ifndef AAPLShaderTypes_h
#define AAPLShaderTypes_h

#include <simd/simd.h>

typedef enum VertexInputIndices {
    VertexInputVertex    = 0,
    VertexInputUniforms         = 2,
    VertexInputPoint        = 3,
    AAPLBufferIndexLightsPosition    = 4,
} VertexInputIndices;

typedef enum FragmentInputIndices {
    FragmentInputTexture = 0,
} FragmentInputIndices;

typedef enum AAPLRenderTargetIndices {
    AAPLRenderTargetLighting  = 0,
} AAPLRenderTargetIndices;

typedef struct {
    float fairy_size;
    int time;
    matrix_float4x4 worldMatrix; /// 物体空间=>世界空间：平移、旋转、缩放等最终形成的复合变换
    matrix_float4x4 cameraMatrix; ///  世界坐标=>摄像机空间：摄像机矩阵
    matrix_float4x4 projectionMatrix; /// 摄像机空间 => 剪裁空间：投影矩阵
    
} FrameUniforms;

typedef enum PointType {
    PointTypeTree = 0,
    PointTypeGround = 1,
    PointTypeColumn = 2,
} PointType;

typedef struct {
    vector_float4 position;
    vector_float3 color;
    float speed;
    PointType type;
} PointInfo;

typedef struct {
    vector_float2 position;
} AAPLFairyVertex;

#endif

