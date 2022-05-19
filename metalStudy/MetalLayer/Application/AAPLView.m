#import "AAPLView.h"

@implementation AAPLView

#pragma mark - 初始化与配置

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if(self) {
        [self initCommon];
    }
    return self;
}

- (instancetype)initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    if(self) {
        [self initCommon];
    }
    return self;
}

- (void)initCommon {
    _metalLayer = (CAMetalLayer*) self.layer;
    self.layer.delegate = self;
}

#pragma mark - Render Loop Control

#if ANIMATION_RENDERING

/// 渲染事件：启用后以 60 帧渲染；禁用后当 UI 请求重绘时才渲染

- (void)stopRenderLoop {
    // Stubbed out method.  Subclasses need to implement this method.
}

- (void)dealloc {
    [self stopRenderLoop];
}

#else

// Override methods needed to handle event-based rendering
- (void)displayLayer:(CALayer *)layer {
    [self renderOnEvent];
}

- (void)drawLayer:(CALayer *)layer inContext:(CGContextRef)ctx {
    [self renderOnEvent];
}

- (void)drawRect:(CGRect)rect {
    [self renderOnEvent];
}

- (void)renderOnEvent {
    
#if RENDER_ON_MAIN_THREAD
    [self render]; /// 主线程渲染
#else
    /// 后台线程，在并发队列调度渲染事件
    dispatch_queue_t globalQueue = dispatch_get_global_queue(QOS_CLASS_USER_INITIATED, 0);
    dispatch_async(globalQueue, ^(){
        [self render];
    });
#endif
}

#endif

#pragma mark - Resizing

#if AUTOMATICALLY_RESIZE

- (void)resizeDrawable:(CGFloat)scaleFactor {
    CGSize newSize = self.bounds.size;
    newSize.width *= scaleFactor;
    newSize.height *= scaleFactor;

    if(newSize.width <= 0 || newSize.width <= 0) {
        return;
    }

#if RENDER_ON_MAIN_THREAD

    if(newSize.width == _metalLayer.drawableSize.width &&
       newSize.height == _metalLayer.drawableSize.height) {
        return;
    }

    _metalLayer.drawableSize = newSize;

    [_delegate drawableResize:newSize];
    
#else
    
    /// size 改变事件必须在主线程调用，且是原子操作（保证线程安全）
    @synchronized(_metalLayer) {
        if(newSize.width == _metalLayer.drawableSize.width &&
           newSize.height == _metalLayer.drawableSize.height) {
            return;
        }

        _metalLayer.drawableSize = newSize;
        [_delegate drawableResize:newSize];
    }
#endif
}

#endif

#pragma mark - Drawing

- (void)render {
#if RENDER_ON_MAIN_THREAD
    [_delegate renderToMetalLayer:_metalLayer];
#else
    // Must synchronize if rendering on background thread to ensure resize operations from the main thread are complete before rendering which depends on the size occurs.
    @synchronized(_metalLayer) {
        [_delegate renderToMetalLayer:_metalLayer];
    }
#endif
}

@end
