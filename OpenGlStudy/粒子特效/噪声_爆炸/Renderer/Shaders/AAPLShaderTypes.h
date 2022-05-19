#ifndef AAPLShaderTypes_h
#define AAPLShaderTypes_h

#include <simd/simd.h>

typedef enum VertexInputIndices {
    VertexInputVertex = 0,
    VertexInputViewportSize = 1,
    VertexInputUniform = 2,
} VertexInputIndices;

typedef enum FragmentInputIndices {
    FragmentInputTexture = 0,
} FragmentInputIndices;

typedef struct {
    float frameTime;
    float uProgress;
} PEUniform;

#endif

