@import MetalKit;
@import ModelIO;

#import "TeapotMesh.h"
#import "AAPLShaderTypes.h"


@implementation TeapotMesh

- (nonnull instancetype) initWithModelIOMesh:(nonnull MDLMesh *)modelIOMesh
                     modelIOVertexDescriptor:(nonnull MDLVertexDescriptor *)vertexDescriptor
                       metalKitTextureLoader:(nonnull MTKTextureLoader *)textureLoader
                                 metalDevice:(nonnull id<MTLDevice>)device
                                       error:(NSError * __nullable * __nullable)error {
    self = [super init];
    if(!self) {
        return nil;
    }
    
    // 平面的法向量：是一个长度为 1 并且垂直于这个平面的向量；
    // 顶点的法向量，是包含该顶点的所有三角形的法向的均值；
    // 因为在顶点着色器中，处理的是顶点，而不是三角形；所以在顶点处有信息是很好的。并且在OpenGL中，我们没有任何办法获得三角形信息。
    // 在三角形三个顶点间，法线是平滑过渡的

    // 使用纹理坐标和法线、计算切线
    [modelIOMesh addTangentBasisForTextureCoordinateAttributeNamed:MDLVertexAttributeTextureCoordinate
                                              normalAttributeNamed:MDLVertexAttributeNormal
                                             tangentAttributeNamed:MDLVertexAttributeTangent];
    // 使用纹理坐标和切线、计算二重切线
    [modelIOMesh addTangentBasisForTextureCoordinateAttributeNamed:MDLVertexAttributeTextureCoordinate
                                             tangentAttributeNamed:MDLVertexAttributeTangent
                                           bitangentAttributeNamed:MDLVertexAttributeBitangent];

    // Note：ModelIO must create tangents and bitangents (as done above) before this relayout occur
    // This is because Model IO's addTangentBasis methods only works with vertex data is all in 32-bit floating-point.
    // The vertex descriptor applied, changes those floats into 16-bit floats or other types from which ModelIO cannot produce tangents

    /// 顶点描述符：顶点数据的内存布局；
    modelIOMesh.vertexDescriptor = vertexDescriptor;

    // 创建 MTKMesh：包含带有网格顶点数据的缓冲区和带有信息的子网格来绘制网格
    MTKMesh* metalKitMesh = [[MTKMesh alloc] initWithMesh:modelIOMesh device:device error:error];
    _metalKitMesh = metalKitMesh;
    assert(metalKitMesh.submeshes.count == modelIOMesh.submeshes.count); // submeshes 的数量应该相同
    return self;
}

+ (NSArray<TeapotMesh*> *)newMeshesFromObject:(nonnull MDLObject*)object
                      modelIOVertexDescriptor:(nonnull MDLVertexDescriptor*)vertexDescriptor
                        metalKitTextureLoader:(nonnull MTKTextureLoader *)textureLoader
                                  metalDevice:(nonnull id<MTLDevice>)device
                                        error:(NSError * __nullable * __nullable)error {

    NSMutableArray<TeapotMesh *> *newMeshes = [NSMutableArray new];
    if ([object isKindOfClass:[MDLMesh class]]) {
        MDLMesh* mesh = (MDLMesh*) object;
        TeapotMesh *newMesh = [[TeapotMesh alloc] initWithModelIOMesh:mesh
                                              modelIOVertexDescriptor:vertexDescriptor
                                                metalKitTextureLoader:textureLoader
                                                          metalDevice:device
                                                                error:error];
        [newMeshes addObject:newMesh];
    }
    return newMeshes;
}

/// Uses Model I/O to load a model file at the given URL, create Model I/O vertex buffers, index buffers
///   and textures, applying the given Model I/O vertex descriptor to layout vertex attribute data
///   in the way that the Metal vertex shaders expect.
+ (nullable NSArray<TeapotMesh *> *)newMeshesFromURL:(nonnull NSURL *)url
                             modelIOVertexDescriptor:(nonnull MDLVertexDescriptor *)vertexDescriptor
                                         metalDevice:(nonnull id<MTLDevice>)device
                                               error:(NSError * __nullable * __nullable)error {

    // Create a MetalKit mesh buffer allocator so that ModelIO will load mesh data directly into
    // Metal buffers accessible by the GPU
    MTKMeshBufferAllocator *bufferAllocator = [[MTKMeshBufferAllocator alloc] initWithDevice:device];

    // Use ModelIO to load the model file at the URL.  This returns a ModelIO asset object, which
    // contains a hierarchy of ModelIO objects composing a "scene" described by the model file.
    // This hierarchy may include lights, cameras, but, most importantly, mesh and submesh data
    // rendered with Metal
    MDLAsset *asset = [[MDLAsset alloc] initWithURL:url vertexDescriptor:nil bufferAllocator:bufferAllocator];
    NSAssert(asset, @"Failed to open model file with given URL: %@", url.absoluteString);

    // Create a MetalKit texture loader to load material textures from files or the asset catalog into Metal textures
    MTKTextureLoader *textureLoader = [[MTKTextureLoader alloc] initWithDevice:device];
    NSMutableArray<TeapotMesh *> *newMeshes = [NSMutableArray new];

    // Traverse the ModelIO asset hierarchy to find ModelIO meshes and create app-specific AAPLMesh objects from those ModelIO meshes
    for(MDLObject* object in asset) {
        NSArray<TeapotMesh *> *assetMeshes;

        assetMeshes = [TeapotMesh newMeshesFromObject:object
                              modelIOVertexDescriptor:vertexDescriptor
                                metalKitTextureLoader:textureLoader
                                          metalDevice:device
                                                error:error];
        [newMeshes addObjectsFromArray:assetMeshes];
    }
    return newMeshes;
}

@end
