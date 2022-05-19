#ifndef AAPLShaderTypes_h
#define AAPLShaderTypes_h

#include <simd/simd.h>

typedef enum AAPLVertexInputIndex {
    AAPLVertexInputIndexVertices     = 0,
    AAPLVertexInputIndexViewportSize = 1,
    AAPLVertexInputIndexTimer        = 2,
} AAPLVertexInputIndex;

typedef enum AAPLTextureIndex {
    AAPLTextureIndexInput  = 0,
    AAPLTextureIndexOutput = 1,
} AAPLTextureIndex;

typedef struct {
    vector_float2 position;
    vector_float2 textureCoordinate;
} AAPLVertex;

#endif
