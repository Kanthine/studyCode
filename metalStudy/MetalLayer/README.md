# CAMetalLayer 定制轻量级渲染视图

虽然 MTKView 提供了重要的功能、方便开发者快速编写代码，但某些时候我们想要更多地控制如何进行渲染。

这个[示例程序](https://github.com/Kanthine/MetalCode/tree/main/MetalLayer)主要演示了如何使用 CAMetalLayer 来定制一个轻量级的 Metal 渲染视图！
* 如何触发一帧渲染；
* 如何在 size 改变时通知渲染器；

## 配置项

这个示例提供了许多选项，在构建应用程序时启用，例如是否动画视图的内容或通过系统事件处理更新。

``` objective-c
/// 在主线程渲染，在绘制期间的 UI 事件更容易管理，因为 UIU 调用必须在主线程
/// 也可以后台渲染，由于事件可以在 GPU 异步处理、某些情况下 UI更快地响应
#define RENDER_ON_MAIN_THREAD 1

/// 渲染事件：启用后以 60 帧渲染；禁用后当 UI 请求重绘时才渲染
#define ANIMATION_RENDERING   1

/// 开启大小自适应：当视图 size 改变时更新视图
/// 关闭后，当视图 size 改变时、需要显式更新视图
#define AUTOMATICALLY_RESIZE  1

/// 开启深度测试：渲染器创建深度缓冲区
#define CREATE_DEPTH_BUFFER   1
```

## 为自定义View配置一个 CAMetalLayer

CAMetalLayer 是 Apple 提供的 Metal 渲染图层，因此在自定义 View 中需要提供 CAMetalLayer！

在 UIKit 中，可以重写 `+layerClass` 类方法，返回一个 `CAMetalLayer` 类型！

``` objective-c
+ (Class)layerClass {
    return [CAMetalLayer class];
}
```

在 AppKit 中的 NSView 由于历史原因，并不像 UIKit 中的 UIView 一样天然带有一个 CALayer 图层。如果想在 NSView 中使用图层，首先需要设置 `wantsLayer`。  

``` objective-c
self.wantsLayer = YES;
```

这触发了对 `NSView -makeBackingLayer` 的调用，使用该方法返回一个 `CAMetalLayer` 对象。

``` objective-c
- (CALayer *)makeBackingLayer {
    return [CAMetalLayer layer];
}
```


## 视图渲染

为了将像素数据渲染到 view 上，我们需要创建一个 `renderPass` 来提供一个渲染目标！
* 设置目标纹理每帧开始渲染时，擦写为 `(0, 1, 1, 1)` 色；
* 存储目标纹理的数据，渲染到 view 上；

``` objective-c
_drawableRenderDescriptor = [MTLRenderPassDescriptor new];
_drawableRenderDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
_drawableRenderDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 1, 1, 1);
_drawableRenderDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
```

在每一帧渲染之前，还需要获取一个渲染目标 `Drawable`，然后将这个渲染目标设置为 `renderPass` 的目标纹理！
* CAMetalDrawable 是 CoreAnimation 提供的将内容渲染到屏幕的关键载体；

``` objective-c
id<CAMetalDrawable> currentDrawable = [metalLayer nextDrawable];
if(!currentDrawable) return;  /// 如果获取不到 drawable，则跳过此帧

/// 设置目标纹理
_drawableRenderDescriptor.colorAttachments[0].texture = currentDrawable.texture;
```

至于具体的渲染命令等细节，本文不做过多阐述，请参阅 [绘制三角形](https://github.com/Kanthine/MetalCode/tree/main/DrawTriangle)!

## 实现渲染调度

为了使渲染具有动画效果，在程序中设置了一个 displayLink，以指定的间隔调用视图，视图调用渲染器来渲染新的动画帧。


``` UIKit

- (void)didMoveToWindow {
    [super didMoveToWindow];
    ...
    /// 需要确定在哪个窗口进行渲染，当窗口改变时，停止渲染，并开始一个新的 displayLink
    [self setupCADisplayLinkForScreen:self.window.screen];
    ...
}

- (void)setupCADisplayLinkForScreen:(UIScreen*)screen {
    [self stopRenderLoop];
    
    _displayLink = [screen displayLinkWithTarget:self selector:@selector(render)];
    _displayLink.paused = self.paused;
    _displayLink.preferredFramesPerSecond = 60;
}
```

AppKit 使用 CVDisplayLink 而不是 CADisplayLink
* CVDisplayLink 和 CADisplayLink 看似不同，但都有相同的目标，那就是允许回调与显示同步。
* 当窗口改变时，AppKit 调用 `-viewDidMoveToWindow` 重新设置 CVDisplayLink；

``` objective-c
- (BOOL)setupCVDisplayLinkForScreen:(NSScreen*)screen {
#if RENDER_ON_MAIN_THREAD

    // The CVDisplayLink callback, DispatchRenderLoop, never executes
    // on the main thread. To execute rendering on the main thread, create
    // a dispatch source using the main queue (the main thread).
    // DispatchRenderLoop merges this dispatch source in each call
    // to execute rendering on the main thread.
    _displaySource = dispatch_source_create(DISPATCH_SOURCE_TYPE_DATA_ADD, 0, 0, dispatch_get_main_queue());
    __weak AAPLView* weakSelf = self;
    dispatch_source_set_event_handler(_displaySource, ^(){
        @autoreleasepool
        {
            [weakSelf render];
        }
    });
    dispatch_resume(_displaySource);

#endif // END RENDER_ON_MAIN_THREAD

    CVReturn cvReturn;

    // Create a display link capable of being used with all active displays
    cvReturn = CVDisplayLinkCreateWithActiveCGDisplays(&_displayLink);

    if(cvReturn != kCVReturnSuccess)
    {
        return NO;
    }

#if RENDER_ON_MAIN_THREAD

    // Set DispatchRenderLoop as the callback function and
    // supply _displaySource as the argument to the callback.
    cvReturn = CVDisplayLinkSetOutputCallback(_displayLink, &DispatchRenderLoop, (__bridge void*)_displaySource);

#else // IF !RENDER_ON_MAIN_THREAD

    // Set DispatchRenderLoop as the callback function and
    // supply this view as the argument to the callback.
    cvReturn = CVDisplayLinkSetOutputCallback(_displayLink, &DispatchRenderLoop, (__bridge void*)self);

#endif // END !RENDER_ON_MAIN_THREAD

    if(cvReturn != kCVReturnSuccess)
    {
        return NO;
    }

    // Associate the display link with the display on which the
    // view resides
    CGDirectDisplayID viewDisplayID =
        (CGDirectDisplayID) [self.window.screen.deviceDescription[@"NSScreenNumber"] unsignedIntegerValue];;

    cvReturn = CVDisplayLinkSetCurrentCGDisplay(_displayLink, viewDisplayID);

    if(cvReturn != kCVReturnSuccess)
    {
        return NO;
    }

    CVDisplayLinkStart(_displayLink);

    NSNotificationCenter* notificationCenter = [NSNotificationCenter defaultCenter];

    // Register to be notified when the window closes so that you
    // can stop the display link
    [notificationCenter addObserver:self
                           selector:@selector(windowWillClose:)
                               name:NSWindowWillCloseNotification
                             object:self.window];

    return YES;
}
```

The macOS version of this code performs a few additional steps. After creating the display link, it sets the callback and a parameter to pass to the callback. If you want rendering to happen on the main thread, it passes a dispatch source object; otherwise, it passes a reference to the view itself. Finally, it tells the display link which display the window is located on, and sets a notification to be called when the window is closed.


[MTKView]: https://developer.apple.com/documentation/metalkit/mtkview
[CAMetalLayer]: https://developer.apple.com/documentation/quartzcore/cametallayer
[CAMetalDrawable]: https://developer.apple.com/documentation/quartzcore/cametaldrawable
[MTLRenderPassDescriptor]: https://developer.apple.com/documentation/metal/mtlrenderpassdescriptor
[CADisplayLink]: https://developer.apple.com/documentation/quartzcore/cadisplaylink
[CVDisplayLink]: https://developer.apple.com/documentation/corevideo/cvdisplaylink-k0k
[didMoveToWindow]: https://developer.apple.com/documentation/uikit/uiview/1622527-didmovetowindow
[viewDidMoveToWindow]: https://developer.apple.com/documentation/appkit/nsview/1483329-viewdidmovetowindow
