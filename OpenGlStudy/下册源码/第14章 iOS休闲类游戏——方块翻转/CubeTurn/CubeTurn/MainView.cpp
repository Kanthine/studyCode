#include "MainView.hpp"
#include "MatrixState2D.hpp"
#include "LoadResourceUtil.h"
#include "MyData.h"

MainView::MainView(){
}
void MainView::Initialize(int width, int height)
{//初始化函数
    aboutView = new AboutView();
    aboutView->Initialize(width, height);
    startGame = new BN2DObject(false,0,LoadResourceUtil::initTexture("start",false),START_WIDTH,START_HEIGHT);
    showMenu = new BN2DObject(false,0,LoadResourceUtil::initTexture("showmenu",false),SHOWMENU_WIDTH,SHOWMENU_HEIGHT);
    menu = new BN2DObject(false,0,LoadResourceUtil::initTexture("menu",false),MENU_WIDTH,MENU_HEIGHT);
    music = new BN2DObject(false,0,LoadResourceUtil::initTexture("open_music",false),MUSIC_WIDTH,MUSIC_HEIGHT);
    effect = new BN2DObject(false,0,LoadResourceUtil::initTexture("open_volume",false),MUSIC_WIDTH,MUSIC_HEIGHT);
    LoadResourceUtil::loadMusic("background", "mp3", -1);
    initVariables();
}



void MainView::Render() {
    glClearColor(1.0f, 1.0f, 1.0f, 1);//设置背景颜色
    //清除深度缓冲与颜色缓冲
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    MatrixState2D::pushMatrix();
    MatrixState2D::translate(0, startTransY, 0);
    MatrixState2D::scale(startScaleX, startScaleY, 1);
    startGame->drawSelf(0,0,1.0f);
    MatrixState2D::popMatrix();

    if(!isDrawMenu)
    {
        MatrixState2D::pushMatrix();
        showMenu->drawSelf(0,SHOWMENU_TRANSY,1.0f);
        MatrixState2D::popMatrix();
    }else
    {
        MatrixState2D::pushMatrix();
        menu->drawSelf(0,0,1.0f);
        MatrixState2D::popMatrix();
        if(!MyData::effectOff)
        {
            effect->setTexId(LoadResourceUtil::initTexture("open_volume",false));
        }else{
            effect->setTexId(LoadResourceUtil::initTexture("close_volume",false));
        }
        effect->drawSelf(-MUSIC_X,MUSIC_Y ,1.0f);
        if(!MyData::musicOff)
        {
            music->setTexId(LoadResourceUtil::initTexture("open_music",false));
        }else{
            music->setTexId(LoadResourceUtil::initTexture("close_music",false));
        }
        music->drawSelf(MUSIC_X,MUSIC_Y ,1.0f);
    }
    if(aboutView->isDrawAbout)
    {
        aboutView->Render();
    }

}
void MainView::OnFingerDown(float x,float y){
    if(!aboutView->isDrawAbout)
    {
        if(!isDrawMenu&&x>=-START_WIDTH/2&&x<=START_WIDTH/2&&y>=-START_HEIGHT/2&&y<=START_HEIGHT/2)
        {//开始游戏
            MyData::playSound("bt_press","mp3");//播放声音
            MyData::switchViewId =  Constant::GAME_VIEW_ID;
        }else if(isDrawMenu&&x>=-START_WIDTH/2&&x<=START_WIDTH/2&&y>=(startTransY-START_HEIGHT/2)&&y<=(startTransY+START_HEIGHT/2))
        {
            MyData::playSound("bt_press","mp3");//播放声音
            MyData::switchViewId =  Constant::GAME_VIEW_ID;
        }else if(!isDrawMenu&&x>=-SHOWMENU_WIDTH/2&&x<=SHOWMENU_WIDTH/2
                 &&y>=(SHOWMENU_TRANSY-SHOWMENU_HEIGHT/2)&&y<=(SHOWMENU_TRANSY+SHOWMENU_HEIGHT/2))//点击显示菜单
        {
            MyData::playSound("bt_press","mp3");//播放声音
            startTransY = 0.5f;//设置开始游戏的平移值
            startScaleX = 0.8f;//设置开始游戏的缩放值
            startScaleY = 0.8f;
            isDrawMenu = true;//绘制菜单
        }else if(isDrawMenu&&x>=-MENU_WIDTH/2&&x<=MENU_WIDTH/2
                 &&y>=(MENU_HEIGHT/2-MENU_STEP*1)&&y<=(MENU_HEIGHT/2-MENU_STEP*0))//点击隐藏菜单
        {
            MyData::playSound("bt_press","mp3");//播放声音
            startTransY = 0;//设置开始游戏的平移值
            startScaleX = 1;//设置开始游戏的缩放值
            startScaleY = 1;
            isDrawMenu = false;//不绘制菜单
        }else if(isDrawMenu&&x>=-MENU_WIDTH/2&&x<=MENU_WIDTH/2
                 &&y>=(MENU_HEIGHT/2-MENU_STEP*2)&&y<=(MENU_HEIGHT/2-MENU_STEP*1))//点击选择关卡
        {
            MyData::playSound("bt_press","mp3");//播放声音
            MyData::switchViewId =  Constant::CHOOSE_VIEW_ID;//选择关卡
        }else if(isDrawMenu&&x>=-MENU_WIDTH/2&&x<=MENU_WIDTH/2
                 &&y>=(MENU_HEIGHT/2-MENU_STEP*3)&&y<=(MENU_HEIGHT/2-MENU_STEP*2))//点击帮助
        {
            MyData::playSound("bt_press","mp3");//播放声音
            MyData::switchViewId = Constant::HELP_VIEW_ID;//帮助界面
            
        }else if(isDrawMenu&&x>=-MENU_WIDTH/2&&x<=MENU_WIDTH/2
                 &&y>=(MENU_HEIGHT/2-MENU_STEP*4)&&y<=(MENU_HEIGHT/2-MENU_STEP*3))//点击关于
        {
            MyData::playSound("bt_press","mp3");//播放声音
            aboutView->isDrawAbout = true;
        }else if(isDrawMenu&&x>=-MENU_WIDTH/2&&x<=MENU_WIDTH/2
                 &&y>=(MENU_HEIGHT/2-MENU_STEP*5)&&y<=(MENU_HEIGHT/2-MENU_STEP*4))//点击退出
        {
            MyData::playSound("bt_press","mp3");//播放声音
            exit(0);
        }else if(isDrawMenu&&x>=(MUSIC_X-MUSIC_WIDTH)&&x<=(MUSIC_X+MUSIC_WIDTH)
                 &&y>=(MUSIC_Y-MUSIC_HEIGHT)&&y<=(MUSIC_Y+MUSIC_HEIGHT))
        {//点击背景音乐按钮
            MyData::musicOff = !MyData::musicOff;
            MyData::playSound("bt_press","mp3");//播放声音
            LoadResourceUtil::pauseMusic();
        }else if(isDrawMenu&&x>=(-MUSIC_X-MUSIC_WIDTH)&&x<=(-MUSIC_X+MUSIC_WIDTH)
                 &&y>=(MUSIC_Y-MUSIC_HEIGHT)&&y<=(MUSIC_Y+MUSIC_HEIGHT))
        {//点击音效按钮
            MyData::effectOff = !MyData::effectOff;
            MyData::playSound("bt_press","mp3");//播放声音
        }
    }else{//关于界面
        aboutView->OnFingerDown(x, y);
    }
}

void MainView::initVariables()
{

    startTransY = 0;//开始游戏平移y值
    startScaleX = 1;//开始游戏缩放x值
    startScaleY = 1;//开始游戏缩放y值
    isDrawMenu = false;//是否绘制菜单标志位
}
MainView::~MainView(){}


