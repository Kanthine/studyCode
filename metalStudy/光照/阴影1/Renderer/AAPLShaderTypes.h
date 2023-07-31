#ifndef AAPLShaderTypes_h
#define AAPLShaderTypes_h

#include <simd/simd.h>

typedef enum VertexInputIndex {
    VertexInputIndexVertices     = 0,
    VertexInputIndexViewportSize = 1,
} VertexInputIndex;

typedef enum TextureIndex {
    TextureIndexOutput = 0,
    TextureIndexTimer,
} TextureIndex;

#endif
