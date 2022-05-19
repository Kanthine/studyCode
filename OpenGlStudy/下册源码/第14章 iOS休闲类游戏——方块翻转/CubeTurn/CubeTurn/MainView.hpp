#ifndef MainView_hpp
#define MainView_hpp

#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#include <stdio.h>
#include "BN2DObject.h"
#include "MyData.h"
#include "AboutView.hpp"
class MainView{
public :

    MainView();
    void Initialize(int width, int height);
    void Render() ;
    void OnFingerDown(float locationx,float locationy);
    void initVariables();
    ~MainView();
    
private:
    BN2DObject* startGame;
    BN2DObject* showMenu;
    BN2DObject* menu;
    BN2DObject* music;
    BN2DObject* effect;
    AboutView* aboutView;
    
    const float SHOWMENU_WIDTH = 0.6f;
    const float SHOWMENU_HEIGHT = 0.15f;
    const float SHOWMENU_TRANSY = -0.8f;
    const float MENU_WIDTH = 0.32f;
    const float MENU_HEIGHT = 0.64f;
    const float MENU_STEP = MENU_HEIGHT/5;
    const float START_WIDTH = 0.6f;
    const float START_HEIGHT = 0.24f;
    const float MUSIC_WIDTH = 0.1f;//音乐图标宽度
    const float MUSIC_HEIGHT = 0.1f;
    const float MUSIC_X = 0.45f;//音乐图标位置
    const float MUSIC_Y = -0.9f;
    
    
    float startTransY = 0;//开始游戏平移y值
    float startScaleX = 1;//开始游戏缩放x值
    float startScaleY = 1;//开始游戏缩放y值
    
    bool isDrawMenu = false;//是否绘制菜单标志
};

#endif /* MainView_hpp */
