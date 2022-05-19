@import MetalKit;

@interface AAPLRenderer : NSObject
<MTKViewDelegate>

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView;

@property (nonatomic, readonly, nonnull) id <MTLDevice> device;

@property (nonatomic, readonly, nullable, weak) MTKView *view;

@end



/** 生成以坐标系原点为中心的椭球的顶点数据
 * @param radii 包含要生成的椭球边界框的宽度(x分量)、高度(y分量)和深度(z分量)的矢量。如果所有组件都相等，则该方法生成一个球体。
 * @param radialSegments 围绕椭球的水平周长(即它在xz平面上的横截面)生成的点数。点数越多，渲染保真度越高，但渲染性能越差。
 * @param verticalSegments 沿椭球高度生成的点数。点数越多，渲染保真度越高，但渲染性能越差。
 * @param geometryType 用来构造网格的图元类型
 * @param inwardNormals YES 生成指向椭球中心的法向量; NO 生成向外的法向量
 *        决定了网格生成的顶点法向量的方向。如果网格从内部查看(例如，用于天空效果)，则指定YES，如果网格从外部查看，则指定NO。
 * @param hemisphere YES 只生成椭球体或球体的上半部分(一个圆屋顶); NO 生成一个完整的椭球或球体。
 * @param allocator 控制网格的顶点数据分配。如果为nil，则Model I/O使用一个内部分配器对象。
 *   例如，要使用MetalKit框架将顶点数据加载到GPU缓冲区中，以便使用Metal进行渲染，需要传递一个MTKMeshBufferAllocator对象。
 *   通过指定一个分配器，你可以确保在从文件读取和加载到GPU内存渲染之间，网格数据被复制最少的次数。
 *
 * newEllipsoidWithRadii
 *   
 */
