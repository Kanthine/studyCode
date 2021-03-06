#import "GLView.h"
#import "IRenderingEngine.hpp"
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
@implementation GLView

//AVAudioPlayer *player;

IRenderingEngine* mRenderingEngine;//指向渲染器接口实现对象的指针
- (id)initWithFrame:(CGRect)frame {//传入CGRect对象
    if (self = [super initWithFrame:frame]) {
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*) super.layer;//创建CAEAGLLayer对象
        eaglLayer.opaque = YES;//设为不透明才能让其可见
        EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES3;//设置使用OPENGL ES3.0
        //使用OPENGL ES3.0版初始化EAGLContext
        mContext = [[EAGLContext alloc] initWithAPI:api]; 
        //使用3.0版初始化EAGLContext失败
        if (!mContext || ![EAGLContext setCurrentContext:mContext]){ 
            return nil;//返回空值
        }else{
            mRenderingEngine = CreateRenderer2();//创建mRenderingEngine对象
        }
        mRenderingEngine->
                Initialize(CGRectGetWidth(frame), CGRectGetHeight(frame));
        [mContext renderbufferStorage:GL_RENDERBUFFER fromDrawable: eaglLayer]; 
        [self drawView: nil];//调用drawView函数
        CADisplayLink* displayLink;//声明指向CADisplayLink对象的指针
        displayLink = [CADisplayLink displayLinkWithTarget:self
                                        selector:@selector(drawView:)];//创建CADisplayLink对象
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop]
                          forMode:NSDefaultRunLoopMode];//设置循环方式
        
    }
    return self;
}


- (void) drawView: (CADisplayLink*) displayLink{//drawView函数
    mRenderingEngine->Render();//调用mRenderingEngine对象的渲染方法
    [mContext presentRenderbuffer:GL_RENDERBUFFER];
}
+ (Class) layerClass{
    return [CAEAGLLayer class];//返回CAEAGLLayer
}
- (void) touchesBegan: (NSSet*) touches withEvent: (UIEvent*) event{
    UITouch* touch = [touches anyObject];//获取事件
    CGPoint location  = [touch locationInView: self];//获取触摸点的坐标
    mRenderingEngine->OnFingerDown(location.x, location.y);//调用监控按下屏幕的函数
}

- (void) touchesEnded: (NSSet*) touches withEvent: (UIEvent*) event{
    UITouch* touch = [touches anyObject];//获取事件
    CGPoint location  = [touch locationInView: self];//获取触摸点的坐标
    mRenderingEngine->OnFingerUp(location.x, location.y);//调用监控抬起屏幕的函数
}
- (void) touchesMoved: (NSSet*) touches withEvent: (UIEvent*) event{
    UITouch* touch = [touches anyObject];//获取事件
    CGPoint previous  = [touch previousLocationInView: self];//获取上一个触摸点的坐标
    CGPoint current = [touch locationInView: self];//获取当前触摸点的坐标
    mRenderingEngine->OnFingerMove(previous.x, previous.y,//调用监控触摸点移动的函数
                                    current.x, current.y);
}


- (void) twoFingerPinch:(UIPinchGestureRecognizer *)recognizer//多点触摸，扩大或者缩小
{
    mRenderingEngine->OnFingerPinched(recognizer.scale);
}

- (void) oneFingerTwoTaps//单手指双击两次，出菜单
{
    mRenderingEngine->OnOneFingerTwoTaps();
}
- (void) twoFingersTwoTaps//双手指双击两次，后退一步
{
    mRenderingEngine->OnTwoFingersTwoTaps();
}
@end
