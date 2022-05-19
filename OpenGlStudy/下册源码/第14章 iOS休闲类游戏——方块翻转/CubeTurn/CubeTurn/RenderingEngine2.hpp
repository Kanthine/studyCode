#ifndef RenderingEngine2__h
#define RenderingEngine2__h

#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#include <cmath>
#include <iostream>
#include <vector>
#include "LoadedObjectVertexNormalTexture.h"
#include "IRenderingEngine.hpp"
#include <map>
#include "StepNumber.h"
#include "BN2DObject.h"
#include "GameView.hpp"
#include "MainView.hpp"
#include "ChooseLevelView.hpp"
#include "HelpView.hpp"

using namespace std;

class RenderingEngine2 : public IRenderingEngine {
public:

    //声明该类的方法
    RenderingEngine2();
    void Initialize(int width, int height);
    void Render() ;
    void OnFingerUp(float locationx,float locationy);
    void OnFingerDown(float locationx,float locationy);
    void OnFingerMove(float previousx,float previousy,float currentx,float current);
    void OnFingerPinched(float scale);//多点触摸，扩大或者缩小
    void OnOneFingerTwoTaps();//单手指双击两次，出菜单
    void OnTwoFingersTwoTaps();//双手指双击两次，后退一步
    

private:
    constexpr const static float PI=3.1415926f/180.0f;
    GLuint m_framebuffer;//创建一个帧缓冲区对象
    GLuint m_renderbuffer;//创建一个渲染缓冲区对象
    GLuint m_depthRenderbuffer;
    GameView* gameView;
    MainView* mainView;
    ChooseLevelView* chooseLevelView;
    HelpView* helpView;
    
    int StandardScreenWidth;
    int StandardScreenHeight;
    
    float fromScreenXToNearX(float x,float width,float height);//将屏幕坐标转换成视口坐标
    float fromScreenYToNearY(float y,float height);//将屏幕坐标转换成视口坐标
    
};
struct IRenderingEngine* CreateRenderer2()
{
    return new RenderingEngine2();
}


#endif


