#ifndef AAPLShaderTypes_h
#define AAPLShaderTypes_h

#include <simd/simd.h>

typedef enum AAPLVertexInputIndex {
    AAPLVertexInputIndexVertices     = 0,
    AAPLVertexInputIndexViewportSize = 1,
    AAPLVertexInputIndexTimer        = 2,
} AAPLVertexInputIndex;

typedef struct {
    vector_float2 position;
    vector_float4 color;
} AAPLVertex;

#endif
