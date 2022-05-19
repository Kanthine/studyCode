#ifndef AboutView_hpp
#define AboutView_hpp
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#include <stdio.h>
#include "BN2DObject.h"
class AboutView
{
public:
    bool isDrawAbout = false;
    AboutView();
    void Initialize(int width, int height);
    void initVariables();//初始化变量
    void Render();
    void OnFingerDown(float locationx,float locationy);
    ~AboutView();

private:
    constexpr const static float BACK_WIDTH=0.15f;// 返回按钮的宽度
    constexpr const static float BACK_HEIGHT=0.15f;
    constexpr const static float ABOUT_WIDTH=0.75f;//关于按钮的宽度
    constexpr const static float ABOUT_HEIGHT=0.75f;
    constexpr const static float BACK_X=-0.35f;//返回按钮最小x坐标
    constexpr const static float BACK_Y=-0.35f;//返回按钮最小x坐标
    constexpr const static float BLACK_GAME_WIDTH=0.9f;//按钮的宽度
    constexpr const static float BLACK_GAME_HEIGHT=0.8f;//按钮的高度
    BN2DObject* back;//绘制返回
    BN2DObject* about;
    BN2DObject* ground;//背景
};
#endif
