#pragma once

#include <simd/simd.h>

typedef enum AAPLVertexInputIndex {
    AAPLVertexInputIndexVertices = 0,
    AAPLVertexInputIndexTexture = 10,
} AAPLVertexInputIndex;

#define AAPLTileWidth 16
#define AAPLTileHeight 16
#define AAPLTileDataSize 256
#define AAPLThreadgroupBufferSize (AAPLTileWidth * AAPLTileHeight * sizeof(uint32_t))
