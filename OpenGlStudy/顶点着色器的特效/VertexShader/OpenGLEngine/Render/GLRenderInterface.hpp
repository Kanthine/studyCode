//
//  GLRenderInterface.hpp
//  Ball
//
//  Created by 苏莫离 on 2019/10/17.
//

#ifndef GLRenderInterface_hpp
#define GLRenderInterface_hpp

#include <stdio.h>

struct GLRenderInterface* CreateRenderer();//创建渲染器对象的函数
struct GLRenderInterface{
    //定义纯虚函数
    virtual void Initialize(int width, int height) = 0;//初始化函数
    virtual void Render() const = 0;//渲染函数
    virtual ~GLRenderInterface() {}//析构函数
    
    virtual void OnFingerUp(float locationx,float locationy) = 0;//触控结束的函数
    virtual void OnFingerDown(float locationx,float locationy) = 0;//触控开始的函数
    virtual void OnFingerMove(float previousx,float previousy,float currentx,float current) = 0;//触控点移动的函数
};

#endif
