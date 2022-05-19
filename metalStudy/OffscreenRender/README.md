# 离屏渲染

什么是离屏渲染？离屏渲染是相对于实时渲染而言的：
* 在实时渲染场景中，我们直接使用 `CAMetalLayer.drawable.texture` 作为渲染目标；
* 在离屏渲染中，我们自定义 `renderPass` 、指定额外的 `texture` 作为渲染目标；

离屏渲染常用于复杂的渲染场景，如首先通过离屏渲染处理光照和阴影信息，然后在下一阶段计算最终场景的光照。离屏渲染也可以用于处理不需要显示在屏幕上的数据。

## 简单示例

本文叙述一个简单的[离屏渲染程序](https://github.com/Kanthine/MetalCode/tree/main/OffscreenRender) 实现思路，经过两个`renderPass`渲染视图：
* 第一阶段自定义 `renderPass`，设定渲染目标为创建的纹理;
* 第二阶段使用 `MTKView.renderPass`，将数据渲染到 `drawable.texture`;

当命令缓存区有多个`renderPass`时，期望这些 `renderPass` 按顺序执行命令；必须保证在一个 RenderPass 开始之前、完成上一个 RenderPass 的命令编码！

当提交命令缓冲区时，Metal 依次执行多个`renderPass` 的命令。在这种情况下：
* Metal 检测到第一个渲染管道写入离屏纹理，第二个管道读取它；
* 当 Metal 检测到这种依赖时，它会阻止后续的 pass 执行，直到GPU完成第一个 pass 的执行!


# 1、配置离屏渲染

## 1.1、为离屏渲染设定一个渲染目标

离屏渲染区别于实时渲染，通过自定义一个目标纹理，将数据绘制到该目标！

``` 
{
    /// 自定义一个纹理，用于离屏渲染的渲染目标！ 
    MTLTextureDescriptor *texDescriptor = [MTLTextureDescriptor new];
    texDescriptor.textureType = MTLTextureType2D;
    texDescriptor.width = 512;
    texDescriptor.height = 512;
    texDescriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
    texDescriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
}
```

精确设置 _纹理访问策略_ `MTLTextureUsage` 可以提高渲染性能，因为 Metal 需要为指定纹理策略的配置底层数据。作为离屏渲染的纹理，需要配置：
* 需要作为渲染目标来承接数据 `MTLTextureUsageRenderTarget`;
* 允许其它渲染管线读取该纹理的数据 `MTLTextureUsageShaderRead`;

## 1.2、负责离屏渲染的管线

渲染管线是一系列绘图命令的集合，包括顶点着色器和片段着色器等；也包括在这条流水线使用的像素格式！

```
{
    MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineStateDescriptor.label = @"离屏·渲染管线";
    pipelineStateDescriptor.sampleCount = 1;
    pipelineStateDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"simpleVertexShader"];
    pipelineStateDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"simpleFragmentShader"];
    
    /// 注意：保证像素格式的匹配
    pipelineStateDescriptor.colorAttachments[0].pixelFormat = _renderTargetTexture.pixelFormat; 
    _renderToTextureRenderPipeline = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];
}
```

## 1.3、自定义 `RenderPass`

> RenderPass 是一系列绘制纹理的渲染命令集合!

在前面创建了离屏渲染的渲染目标，还需要将这个纹理配置到渲染管线上！通过自定义一个 `RenderPass`，为离屏渲染做一些命令配置：

```
{
    _offscreenRenderPass = [MTLRenderPassDescriptor new];
    _offscreenRenderPass.colorAttachments[0].texture = _renderTargetTexture; /// 设定渲染目标
    
    /// loadAction 决定 GPU 执行渲染命令时，纹理的初始内容
    _offscreenRenderPass.colorAttachments[0].loadAction = MTLLoadActionClear;
    _offscreenRenderPass.colorAttachments[0].clearColor = MTLClearColorMake(1, 1, 1, 1);
    
    /// storeAction 决定渲染结束后 GPU 是否将最终图像写入纹理
    /// 由于后面阶段还需要读取离屏渲染的结果，因此需要保存
    _offscreenRenderPass.colorAttachments[0].storeAction = MTLStoreActionStore;
}
```

# 2、渲染命令

在渲染缓冲区中，执行了两段渲染命令
* 首先是离屏渲染；
* 然后实时渲染；

``` 
- (void)drawInMTKView:(nonnull MTKView *)view {
    /// 使用命令队列创建一个缓冲区
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"Command Buffer";

    /// 阶段一：离屏渲染阶段
    {
        static const AAPLSimpleVertex triVertices[] = {
            // Positions     ,  Colors
            { {  0.5,  -0.5 },  { 1.0, 0.0, 0.0, 1.0 } },
            { { -0.5,  -0.5 },  { 0.0, 1.0, 0.0, 1.0 } },
            { {  0.0,   0.5 },  { 0.0, 0.0, 1.0, 0.0 } },
        };
        
        /// 使用自定义的 RenderPass 创建一个绘图命令编码器
        id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:_offscreenRenderPass];
        renderEncoder.label = @"Offscreen Render Pass";
        [renderEncoder setRenderPipelineState:_renderToTextureRenderPipeline];
        [renderEncoder setVertexBytes:&triVertices length:sizeof(triVertices) atIndex:AAPLVertexInputIndexVertices];
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
        
        /// 当有多个 RenderPass 分别渲染，期望这些 RenderPass 按顺序执行命令
        /// 必须保证在 RenderPass 开始之前、完成上一个 RenderPass 的命令编码
        [renderEncoder endEncoding];
    }
    
    /// 阶段二：实时渲染阶段
    /// 通过 view 获取的 RenderPass 最终渲染到 CAMetalLayer.drawable.texture，因此是实时渲染
    MTLRenderPassDescriptor *drawableRenderPassDescriptor = view.currentRenderPassDescriptor;
    if(drawableRenderPassDescriptor != nil) {
        static const AAPLTextureVertex quadVertices[] = {
            // Positions     , Texture coordinates
            { {  0.5,  -0.5 },  { 1.0, 1.0 } },
            { { -0.5,  -0.5 },  { 0.0, 1.0 } },
            { { -0.5,   0.5 },  { 0.0, 0.0 } },

            { {  0.5,  -0.5 },  { 1.0, 1.0 } },
            { { -0.5,   0.5 },  { 0.0, 0.0 } },
            { {  0.5,   0.5 },  { 1.0, 0.0 } },
        };
        
        /// 创建实时渲染编码器
        id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:drawableRenderPassDescriptor];
        renderEncoder.label = @"Drawable Render Pass";
        [renderEncoder setRenderPipelineState:_drawableRenderPipeline];
        [renderEncoder setVertexBytes:&quadVertices length:sizeof(quadVertices) atIndex:AAPLVertexInputIndexVertices];
        [renderEncoder setVertexBytes:&_aspectRatio length:sizeof(_aspectRatio) atIndex:AAPLVertexInputIndexAspectRatio];

        // 离屏渲染的目标纹理，作为实时渲染的源数据
        [renderEncoder setFragmentTexture:_renderTargetTexture atIndex:AAPLTextureInputIndexColor];
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
        [renderEncoder endEncoding];
        [commandBuffer presentDrawable:view.currentDrawable];
    }
    [commandBuffer commit];
}
```
