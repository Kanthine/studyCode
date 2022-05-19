#import "AAPLUIView.h"
#import "AAPLConfig.h"

@implementation AAPLUIView
{
    CADisplayLink *_displayLink;

#if !RENDER_ON_MAIN_THREAD
    // Secondary thread containing the render loop
    NSThread *_renderThread; /// 渲染线程

    // Flag to indcate rendering should cease on the main thread
    BOOL _continueRunLoop; /// 显示主线程应该停止的标志
#endif
}

#pragma mark - Initialization and Setup

+ (Class)layerClass {
    return [CAMetalLayer class];
}

/// 窗口移动时调用（主线程调用）
- (void)didMoveToWindow {
    [super didMoveToWindow];
    
#if ANIMATION_RENDERING
    if(self.window == nil) {
        // 如果窗口移动，则释放 displayLink
        [_displayLink invalidate];
        _displayLink = nil;
        return;
    }
    
    [self setupCADisplayLinkForScreen:self.window.screen];

#if RENDER_ON_MAIN_THREAD


    /// 将 CADisplayLink 加入 mainRunLoop
    [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];

#else // IF !RENDER_ON_MAIN_THREAD

    /// 使用 @synchronized 保证 _continueRunLoop 是原子操作
    @synchronized(self) {
        /// 停止动画循环（允许循环完成，如果它正在进行）
        _continueRunLoop = NO;
    }
    
    
    // 创建并启动一个子线程，它将有另一个 runloop
    _renderThread =  [[NSThread alloc] initWithTarget:self selector:@selector(runThread) object:nil];
    _continueRunLoop = YES;
    [_renderThread start];

#endif // END !RENDER_ON_MAIN_THREAD
#endif // ANIMATION_RENDERING
    
    /// size 改变时需要的操作，
#if AUTOMATICALLY_RESIZE
    [self resizeDrawable:self.window.screen.nativeScale];
#else
    /// DrawableSize 的改变回调 delegate
    CGSize defaultDrawableSize = self.bounds.size;
    defaultDrawableSize.width *= self.layer.contentsScale;
    defaultDrawableSize.height *= self.layer.contentsScale;
    [self.delegate drawableResize:defaultDrawableSize];
#endif
}

#pragma mark - Render Loop Control

#if ANIMATION_RENDERING

- (void)setPaused:(BOOL)paused {
    super.paused = paused;
    _displayLink.paused = paused;
}

- (void)setupCADisplayLinkForScreen:(UIScreen*)screen {
    [self stopRenderLoop];

    _displayLink = [screen displayLinkWithTarget:self selector:@selector(render)];

    _displayLink.paused = self.paused;

    _displayLink.preferredFramesPerSecond = 60;
}

- (void)didEnterBackground:(NSNotification*)notification {
    self.paused = YES;
}

- (void)willEnterForeground:(NSNotification*)notification {
    self.paused = NO;
}

- (void)stopRenderLoop {
    [_displayLink invalidate];
}

#if !RENDER_ON_MAIN_THREAD

/// 创建的子线程
- (void)runThread {
    // Set the display link to the run loop of this thread so its call back occurs on this thread
    NSRunLoop *runLoop = [NSRunLoop currentRunLoop];
    [_displayLink addToRunLoop:runLoop forMode:@"AAPLDisplayLinkMode"];

    // The '_continueRunLoop' ivar is set outside this thread, so it must be synchronized.  Create a
    // 'continueRunLoop' local var that can be set from the _continueRunLoop ivar in a @synchronized block
    BOOL continueRunLoop = YES;

    // Begin the run loop
    while (continueRunLoop)
    {
        // Create autorelease pool for the current iteration of loop.
        @autoreleasepool
        {
            // Run the loop once accepting input only from the display link.
            [runLoop runMode:@"AAPLDisplayLinkMode" beforeDate:[NSDate distantFuture]];
        }

        // Synchronize this with the _continueRunLoop ivar which is set on another thread
        @synchronized(self)
        {
            // Anything accessed outside the thread such as the '_continueRunLoop' ivar
            // is read inside the synchronized block to ensure it is fully/atomically written
            continueRunLoop = _continueRunLoop;
        }
    }
}
#endif // END !RENDER_ON_MAIN_THREAD

#endif // END ANIMATION_RENDERING

#pragma mark - size 改变时

#if AUTOMATICALLY_RESIZE

- (void)setContentScaleFactor:(CGFloat)contentScaleFactor {
    [super setContentScaleFactor:contentScaleFactor];
    [self resizeDrawable:self.window.screen.nativeScale];
}

- (void)layoutSubviews {
    [super layoutSubviews];
    [self resizeDrawable:self.window.screen.nativeScale];
}

- (void)setFrame:(CGRect)frame {
    [super setFrame:frame];
    [self resizeDrawable:self.window.screen.nativeScale];
}

- (void)setBounds:(CGRect)bounds {
    [super setBounds:bounds];
    [self resizeDrawable:self.window.screen.nativeScale];
}

#endif

@end
