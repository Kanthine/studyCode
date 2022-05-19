#include <metal_stdlib>
#include <simd/simd.h>
using namespace metal;
#import "AAPLShaderTypes.h"
#import "AAPLShaderCommon.h"

typedef struct {
    float3 position [[attribute(AAPLVertexAttributePosition)]];
    float2 texCoord [[attribute(AAPLVertexAttributeTexcoord)]];
} Vertex;

vertex ColorInOut vertexTransform(Vertex in [[stage_in]],
                                  constant AAPLFrameUniforms & frameUniforms [[ buffer(AAPLBufferIndexFrameUniforms) ]]) {
    ColorInOut out;
    float4 position = float4(in.position, 1.0);
    out.position = frameUniforms.projectionMatrix * frameUniforms.modelViewMatrix * position;
    out.texCoord = in.texCoord;
    return out;
}
