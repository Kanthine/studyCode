#ifndef AAPLShaderTypes_h
#define AAPLShaderTypes_h

#include <simd/simd.h>

typedef enum AAPLVertexInputIndex {
    AAPLVertexInputIndexVertices = 0,
    AAPLVertexInputIndexUniforms = 1,
} AAPLVertexInputIndex;

typedef struct {
    vector_float2 position;
    vector_float3 color;
} AAPLVertex;

typedef struct {
    float scale;
    vector_float2 viewportSize;
} AAPLUniforms;

#endif
