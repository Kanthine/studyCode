#ifndef AAPLShaderTypes_h
#define AAPLShaderTypes_h

#include <simd/simd.h>

typedef enum VertexInputIndex {
    VertexInputIndexVertices     = 0,
    VertexInputIndexViewportSize,
    VertexInputIndexTexture,
    VertexInputIndexTextureSDF,
    VertexInputIndexTimer,
} VertexInputIndex;

#endif
