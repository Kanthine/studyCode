//
//  GLView.m
//  Ball
//
//  Created by 苏莫离 on 2019/10/17.
//

#import "GLView.h"
#import <OpenGLES/EAGL.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#include "GLRenderInterface.hpp"

@interface GLView ()
{
    EAGLContext* mContext;//指向EAGLContext对象的指针
}
@end


@implementation GLView

GLRenderInterface *_render; //指向渲染器接口实现对象的指针

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*) super.layer; //创建CAEAGLLayer对象
        eaglLayer.opaque = YES;//设为不透明才能让其可见
        EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES3;//设置使用OPENGL ES3.0
        //使用OPENGL ES3.0版初始化EAGLContext
        mContext = [[EAGLContext alloc] initWithAPI:api];
        //使用3.0版初始化EAGLContext失败
        if (!mContext || ![EAGLContext setCurrentContext:mContext]){
            return nil;//返回空值
        }else{
            _render = CreateRenderer();//创建mRenderingEngine对象
        }
        _render-> Initialize(CGRectGetWidth(frame), CGRectGetHeight(frame));
        [mContext renderbufferStorage:GL_RENDERBUFFER fromDrawable: eaglLayer];
        [self drawView:nil];//调用drawView函数
        CADisplayLink* displayLink;//声明指向CADisplayLink对象的指针
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];//创建CADisplayLink对象
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];//设置循环方式
    }
    return self;
}

- (void)drawView:(CADisplayLink *)displayLink{
    _render -> Render(); //调用mRenderingEngine对象的渲染方法
    [mContext presentRenderbuffer:GL_RENDERBUFFER];
}

+ (Class)layerClass{
    return [CAEAGLLayer class];
}

- (void) touchesBegan: (NSSet*) touches withEvent: (UIEvent*) event{//触控开始
    UITouch* touch = [touches anyObject];//获取事件
    CGPoint location  = [touch locationInView: self];//获取触摸点的坐标
    _render->OnFingerDown(location.x, location.y);//调用用户按下屏幕时的相关处理函数
}
- (void)touchesEnded: (NSSet*) touches withEvent: (UIEvent*) event{//触控结束
    UITouch* touch = [touches anyObject];//获取事件
    CGPoint location  = [touch locationInView: self];//获取触摸点的坐标
    _render->OnFingerUp(location.x, location.y);//调用用户抬起手指时的相关处理函数
}
- (void) touchesMoved: (NSSet*) touches withEvent: (UIEvent*) event{
    UITouch* touch = [touches anyObject];//获取事件
    CGPoint previous  = [touch previousLocationInView: self];//获取上一个触摸点的坐标
    CGPoint current = [touch locationInView: self];//获取当前触摸点的坐标
    _render->OnFingerMove(previous.x, previous.y,//调用用户在屏幕上移动手指时的相关处理函数
                                    current.x, current.y);
}

@end
