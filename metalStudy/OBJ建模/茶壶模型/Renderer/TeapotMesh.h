@import Foundation;
@import MetalKit;
@import simd;

// 茶壶网格：包含描述网格的顶点数据和描述如何绘制网格部分的子网格对象
@interface TeapotMesh : NSObject

// Constructs an array of meshes from the provided file URL, which indicate the location of a model
//  file in a format supported by Model I/O, such as OBJ, ABC, or USD.  mdlVertexDescriptor defines
//  the layout ModelIO will use to arrange the vertex data while the bufferAllocator supplies
//  allocations of Metal buffers to store vertex and index data
+ (nullable NSArray<TeapotMesh *> *) newMeshesFromURL:(nonnull NSURL *)url
                            modelIOVertexDescriptor:(nonnull MDLVertexDescriptor *)vertexDescriptor
                                        metalDevice:(nonnull id<MTLDevice>)device
                                              error:(NSError * __nullable * __nullable)error;

// A MetalKit mesh containing vertex buffers describing the shape of the mesh
@property (nonatomic, readonly, nonnull) MTKMesh *metalKitMesh;

@end



/**
 从上述 obj 文件片段中可以看出，其内容是以行为基本单位进行组织的，每种不同前缀开头的行有不同的含义，具体情况如下所列。
 * “#”号开头的行为注释，在程序加载的过程中可以略过。
 * “v”开头的行用于存放顶点坐标，其后面的 3 个数值分别表示一个顶点的 x、y、z 坐标。
 * “vt”开头的行用于存放顶点纹理坐标，其后面的 3 个数值分别表示纹理坐标的 S、T、P 分量。
 * “vn”开头的行用于存放顶点法向量，其后面的 3 个数值分别表示一个顶点的法向量在 x 轴、y 轴、z 轴上的分量。
 * g”开头的行表示一组的开始，后面的字符串为此组的名称。所谓组是指由顶点组成的 一些面的集合。只包含“g”的行表示一组的结束，与“g”开头的行对应。
 * “f”开头的行表示组中的一个面，如果是三角形(由于 OpenGL ES 仅支持三角形，故本 书案例中采用的都是三角形)则后面有 3 组用空格分隔的数据，代表三角形的 3 个顶点。每组数 据中包含 3 个数值，用“/”分隔， 依次表示顶点坐标数据索引、顶点纹理坐标数据索引、顶点法 向量数据索引。
 */
