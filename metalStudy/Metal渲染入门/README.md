# Metal 渲染入门

在 Demo 中，创建一个 MTKView 视图，并将背景色刷成指定色！

## 为 Draw 准备一个 MTKView

MetalKit 提供了一个视图 MTKView，它是 NSView 或者 UIView 的子类。
* 首先需要为 MTKView 提供一个设备 [device](https://developer.apple.com/documentation/metalkit/mtkview/1536011-device)

``` objective-c
_view.device = MTLCreateSystemDefaultDevice();
```

要将视图内容擦除为指定色，可以设置它的属性 [clearColor](https://developer.apple.com/documentation/metalkit/mtkview/1536036-clearcolor)。
* 通过 [MTLClearColorMake](https://developer.apple.com/documentation/metal/1437971-mtlclearcolormake) 函数来创建颜色，指定红色、绿色、蓝色和alpha值。

``` objective-c
_view.clearColor = MTLClearColorMake(0.0, 0.5, 1.0, 1.0);
```

由于在 Demo 中仅仅展示 `clearColor` 的用法，而没有做复杂的渲染命令，因此不必每帧刷新视图，只需在视图 frame 改变时刷新一次即可！

``` objective-c

/// 指示视图是否响应 setNeedsDisplay，默认为 NO
/// 如果这个值和 pause 的值是YES，视图的行为类似于UIView对象，响应对setNeedsDisplay的调用。在这种情况下，视图的内部绘制循环被暂停，更新是事件驱动的
_view.enableSetNeedsDisplay = YES;
```


## MTKView 的委托

MTKView 通过设置 delegate 来通知业务层何时绘制、如何绘制！

``` objective-c
_view.delegate = _renderer;
```

MTKView 的委托方法有两个：

* 当视图的 size 改变时、设备方向改变时，会调用 [-mtkView:drawableSizeWillChange:](https://developer.apple.com/documentation/metalkit/mtkviewdelegate/1536015-mtkview) 适应绘制窗口的分辨率；
*  在 [-drawInMTKView:](https://developer.apple.com/documentation/metalkit/mtkviewdelegate/1535942-drawinmtkview) 中更新视图内容；
    * 创建命令缓冲区，
    * 创建命令编码器，对绘图命令编码
    * 这些绘图命令告诉 GPU 要绘制什么，排队等待 GPU 执行命令缓冲区，什么时候显示在屏幕上；

## 创建一个渲染通道

为了将图像数据绘制到目标纹理，我们需要创建一个渲染通道（一个渲染序列，包含很多绘图命令），渲染通道中的纹理被称为目标纹理。在合适的时机，Metal 将目标纹理渲染到屏幕上！

``` objective-c
MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;
if (renderPassDescriptor == nil) { /// 有可能为 nil，需要判断一下
    return;
}
```

* 在渲染开始时，可以将目标纹理擦写为一个纯色，类似于一个干净的画布；
* 渲染时，向目标纹理绘制图像数据；


每一个渲染管道 renderPass 都需要创建一个命令编码器来编码命令：

``` objective-c
id<MTLRenderCommandEncoder> commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
```

当该渲染通道结束命令时，必须调用命令编码器结束编码！

``` objective-c
[commandEncoder endEncoding];
```

## 将 Drawable 呈现到屏幕

MTKView 会自动创建一个 drawable 来管理目标纹理，它的属性 [currentDrawable](https://developer.apple.com/documentation/metalkit/mtkview/1535971-currentdrawable) 可以获取一个作为渲染通道目标纹理的 drawable。 

目标纹理不会自动显示到屏幕上：
* 需要获取一个可用的 `drawable`；
* 这个 CAMetalDrawable 是连接到 Core Animation 关键所在；
* 然后调用 [-presentDrawable:](https://developer.apple.com/documentation/metal/mtlcommandbuffer/1443029-presentdrawable)；

``` objective-c
id<MTLDrawable> drawable = view.currentDrawable;



[commandBuffer presentDrawable:drawable];
```

当命令缓冲区被调度执行时，Metal 应该与 Core Animation 协调，在渲染完成后显示纹理。当Core Animation呈现纹理时，它成为视图的新内容。

## 创建命令缓冲区

将命令提交到命令缓冲区！

``` objective-c
[commandBuffer commit];
```


[MTLDevice](https://developer.apple.com/documentation/metal/mtldevice)

[MTLRenderPassDescriptor](https://developer.apple.com/documentation/metal/mtlrenderpassdescriptor)

[MTLRenderCommandEncoder](https://developer.apple.com/documentation/metal/mtlrendercommandencoder)

[MetalKit](https://developer.apple.com/documentation/metalkit)

[MTKView](https://developer.apple.com/documentation/metalkit/mtkview)

[MTKViewDelegate](https://developer.apple.com/documentation/metalkit/mtkviewdelegate)

[MetalComputeBasic](https://developer.apple.com/documentation/metal)

[CAMetalDrawable](https://developer.apple.com/documentation/quartzcore/cametaldrawable)
