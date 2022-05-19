#ifndef AAPLShaderTypes_h
#define AAPLShaderTypes_h
#include <simd/simd.h>

typedef enum AAPLVertexInputIndex {
    AAPLVertexInputIndexVertices = 0,
    AAPLVertexInputIndexViewport = 1,
} AAPLVertexInputIndex;

typedef struct {
    vector_float3 position;
    vector_float4 color;
} AAPLVertex;

#endif
