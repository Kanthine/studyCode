#include "IRenderingEngine.hpp"
#include "RenderingEngine2.hpp"
#include "MatrixState2D.hpp"
#include "LoadResourceUtil.h"
RenderingEngine2::RenderingEngine2()
{
}

void RenderingEngine2::Initialize(int width, int height)
{//初始化函数
    glGenRenderbuffers(1, &m_renderbuffer);//创建一个渲染缓冲区对象
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);//将该渲染缓冲区对象绑定到管线上
    glGenRenderbuffers(1, &m_depthRenderbuffer);//创建深度缓冲区对象
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderbuffer);//将该深度缓冲区对象绑定到管线上
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,width, height);//设定渲染缓存的存储类型
    glGenFramebuffers(1, &m_framebuffer);//创建一个帧缓冲区对象
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);//将该帧染缓冲区对象绑定到管线上
    //将创建的渲染缓冲区绑定到帧缓冲区上，并使用颜色填充
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,m_renderbuffer);
    //将创建的深度缓冲区绑定到帧缓冲区上
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, m_depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);//将该渲染缓冲区对象绑定到管线上
    glViewport(0, 0, width, height);//设置适口
    glEnable(GL_CULL_FACE);//启用背面剪裁
    glEnable(GL_DEPTH_TEST);//启用深度测试
    
    StandardScreenWidth=width;
    StandardScreenHeight=height;
    float ratio = (float) width/height;//计算宽高比
    MatrixState2D::setInitStack();   //初始化矩阵
    MatrixState2D::setProjectOrtho(-ratio, ratio, -1, 1, 1, 100);
    MatrixState2D::setCamera(0,0,5,0,0,0,0,1.0f,0);
    MatrixState2D::setLightLocationSun(-5,50,20);
    
    gameView=new GameView();
    gameView->Initialize(width, height);
    
    mainView=new MainView();
    mainView->Initialize(width, height);
    
    chooseLevelView = new ChooseLevelView();
    chooseLevelView->Initialize(width, height);
    
    helpView = new HelpView();
    helpView->Initialize(width, height);
    
    LoadResourceUtil::pauseMusic();
}


void RenderingEngine2::Render() {
    
    switch(MyData::switchViewId)
    {
        case Constant::GAME_VIEW_ID://游戏界面
            gameView->Render();
            break;
        case Constant::MAIN_VIEW_ID://主菜单界面
            mainView->Render();
            break;
        case Constant::CHOOSE_VIEW_ID://选择关卡界面
            chooseLevelView->Render();
            break;
        case Constant::HELP_VIEW_ID://帮助界面
            helpView->Render();
            break;
    }
}
void RenderingEngine2::OnFingerDown(float locationx,float locationy){
    float x =  fromScreenXToNearX(locationx,StandardScreenWidth,StandardScreenHeight);
    float y =  fromScreenYToNearY(locationy,StandardScreenHeight);
    switch(MyData::switchViewId)
    {
        case Constant::GAME_VIEW_ID://游戏界面
            gameView->OnFingerDown(x,y,locationx, locationy,StandardScreenWidth,StandardScreenHeight);
            break;
        case Constant::MAIN_VIEW_ID://主菜单界面
            mainView->OnFingerDown(x, y);
            break;
        case Constant::HELP_VIEW_ID://帮助界面
            helpView->OnFingerDown(x, y);
            if(helpView->isInitDatas)
            {
                helpView->levelHelp = 0;//重新进入帮助界面时，初始化第一关卡的数据
                helpView->initVariables();//初始化变量
            }
            break;
        case Constant::CHOOSE_VIEW_ID://选择关卡界面
            chooseLevelView->OnFingerDown(x ,y);
            //选择关卡后，初始化当前关卡的数据
            gameView->initSceneData(MyData::level);
            break;
    }
}
void RenderingEngine2::OnFingerUp(float locationx,float locationy){
    gameView->OnFingerUp(locationx ,locationy);
}
void RenderingEngine2::OnFingerMove(float previousx,float previousy,float currentx,float currenty){
    gameView->OnFingerMove(previousx, previousy, currentx, currenty);
}
void RenderingEngine2::OnFingerPinched(float scale){
    gameView->OnFingerPinched(scale);
}
void RenderingEngine2::OnOneFingerTwoTaps(){
   gameView->OnOneFingerTwoTaps();
}
void RenderingEngine2::OnTwoFingersTwoTaps(){
    gameView->OnTwoFingersTwoTaps();
}

float RenderingEngine2::fromScreenXToNearX(float x,float width,float height)//将屏幕坐标转换成视口坐标
{
    return (x-width/2)/(height/2);
}
float RenderingEngine2::fromScreenYToNearY(float y,float height)//将屏幕坐标转换成视口坐标
{
    return -(y-height/2)/(height/2);
}




