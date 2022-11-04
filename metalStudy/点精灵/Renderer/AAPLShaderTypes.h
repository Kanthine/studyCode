#ifndef AAPLShaderTypes_h
#define AAPLShaderTypes_h

#include <simd/simd.h>

typedef enum VertexInputIndices {
    VertexInputPoint    = 0,
    VertexInputViewport = 1,
} VertexInputIndices;

typedef enum FragmentInputIndices {
    FragmentInputTexture_1 = 1,
    FragmentInputTexture_2 = 2,
    FragmentInputTexture_3 = 3,
} FragmentInputIndices;

#endif

