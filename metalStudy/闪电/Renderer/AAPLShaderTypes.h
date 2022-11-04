#ifndef AAPLShaderTypes_h
#define AAPLShaderTypes_h

#include <simd/simd.h>

typedef enum AAPLInputIndex {
    AAPLInputIndexVertices     = 0,
    AAPLInputIndexViewportSize = 1,
    AAPLInputIndexUniforms = 2,
} AAPLInputIndex;

typedef struct {
    vector_float2 position;
    vector_float4 color;
} AAPLVertex;

typedef struct {
    vector_float2 viewportSize;
    float   timeStep;
} Uniforms;

#endif /* AAPLShaderTypes_h */
