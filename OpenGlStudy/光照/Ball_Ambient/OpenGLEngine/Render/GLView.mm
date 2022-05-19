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
    EAGLContext *mContext;//指向 EAGLContext 对象的指针
}
@end


@implementation GLView

GLRenderInterface *_render; //指向渲染器接口实现对象的指针

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        CAEAGLLayer* eaglLayer = (CAEAGLLayer *)super.layer; //创建CAEAGLLayer对象
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
    _render->Render();//调用mRenderingEngine对象的渲染方法
    [mContext presentRenderbuffer:GL_RENDERBUFFER];
}

+ (Class)layerClass{
    return [CAEAGLLayer class];
}

@end
