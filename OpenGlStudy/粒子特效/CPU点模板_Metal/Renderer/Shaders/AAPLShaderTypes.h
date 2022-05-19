#ifndef AAPLShaderTypes_h
#define AAPLShaderTypes_h

#include <simd/simd.h>

typedef enum VertexInputIndices {
    VertexInputVertex       = 0,
    VertexInputPoint        = 1,
    VertexInputViewportSize = 2,
} VertexInputIndices;

typedef enum FragmentInputIndices {
    FragmentInputTexture = 0,
} FragmentInputIndices;

typedef struct {
    vector_float2 oldPosition;
    vector_float2 position;
    vector_float2 rate;
} PointInfo;

typedef struct {
    float uTime;
    float uProgress;
    
    matrix_float4x4 worldMatrix;
    matrix_float4x4 cameraMatrix;
    matrix_float4x4 projectionMatrix;
} PEUniform;

#endif

