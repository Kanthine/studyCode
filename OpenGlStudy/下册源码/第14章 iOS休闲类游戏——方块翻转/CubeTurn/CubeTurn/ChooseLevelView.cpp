#include "ChooseLevelView.hpp"
#include "MatrixState2D.hpp"
#include "LoadResourceUtil.h"
#include "MyData.h"
#include "Constant.hpp"


ChooseLevelView::ChooseLevelView(){
}

void ChooseLevelView::Initialize(int width, int height)
{//初始化函数
    chooseLevel=new BN2DObject(false,0,LoadResourceUtil::initTexture("chooselevel",false),CHOOSE_WIDTH,CHOOSE_HEIGHT);
    first = new BN2DObject(false,0,LoadResourceUtil::initTexture("fir",false),FIRST_WIDTH,FIRST_HEIGHT);
    sec = new BN2DObject(false,0,LoadResourceUtil::initTexture("sec",false),FIRST_WIDTH,FIRST_HEIGHT);
    third = new BN2DObject(false,0,LoadResourceUtil::initTexture("thi",false),FIRST_WIDTH,FIRST_HEIGHT);
    four = new BN2DObject(false,0,LoadResourceUtil::initTexture("fou",false),FIRST_WIDTH,FIRST_HEIGHT);
    back = new BN2DObject(false,0,LoadResourceUtil::initTexture("back",false),FIRST_WIDTH,FIRST_HEIGHT);
}

void ChooseLevelView::Render() {
    glClearColor(1.0f, 1.0f, 1.0f, 1);//设置背景颜色
    //清除深度缓冲与颜色缓冲
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    drawChoose();
}

void ChooseLevelView::drawChoose()
{
    MatrixState2D::pushMatrix();
    chooseLevel->drawSelf(0, CHOOSE_Y,1.0f);
    MatrixState2D::popMatrix();
    
    MatrixState2D::pushMatrix();
    first->drawSelf(0, FIRST_Y,1.0f);
    MatrixState2D::popMatrix();
    
    MatrixState2D::pushMatrix();
    sec->drawSelf(0, SEC_Y,1.0f);
    MatrixState2D::popMatrix();
    
    MatrixState2D::pushMatrix();
    third->drawSelf(0, THI_Y,1.0f);
    MatrixState2D::popMatrix();
    
    MatrixState2D::pushMatrix();
    four->drawSelf(0, FOU_Y,1.0f);
    MatrixState2D::popMatrix();
    
    MatrixState2D::pushMatrix();
    back->drawSelf(0, BACK_Y,1.0f);
    MatrixState2D::popMatrix();
}

void ChooseLevelView::OnFingerDown(float x,float y){

    if(x>(-FIRST_WIDTH/2)&&x<(FIRST_WIDTH/2)&&y>(FIRST_Y-FIRST_HEIGHT/2)&&y<(FIRST_Y+FIRST_HEIGHT/2))
    {
        MyData::playSound("bt_press","mp3");//播放声音
        MyData::level=Constant::LEVEL_FIRST;//第一关卡
        MyData::switchViewId =  Constant::GAME_VIEW_ID;
    }else if(x>(-FIRST_WIDTH/2)&&x<(FIRST_WIDTH/2)&&y>(SEC_Y-FIRST_HEIGHT/2)&&y<(SEC_Y+FIRST_HEIGHT/2))
    {
        MyData::playSound("bt_press","mp3");//播放声音
        MyData::level=Constant::LEVEL_SEC;//第二关卡
        MyData::switchViewId= Constant::GAME_VIEW_ID;
    }
    else if(x>(-FIRST_WIDTH/2)&&x<(FIRST_WIDTH/2)&&y>(THI_Y-FIRST_HEIGHT/2)&&y<(THI_Y+FIRST_HEIGHT/2))
    {
        MyData::playSound("bt_press","mp3");//播放声音
        MyData::level=Constant::LEVEL_THIRD;//第三关卡
        MyData::switchViewId= Constant::GAME_VIEW_ID;
    }
    else if(x>(-FIRST_WIDTH/2)&&x<(FIRST_WIDTH/2)&&y>(FOU_Y-FIRST_HEIGHT/2)&&y<(FOU_Y+FIRST_HEIGHT/2))
    {
        MyData::playSound("bt_press","mp3");//播放声音
        MyData::level=Constant::LEVEL_FOUTH;//第四关卡
        MyData::switchViewId= Constant::GAME_VIEW_ID;
    }
    else if(x>BACK_MIN_X&&x<(-BACK_MIN_X)&&y>(BACK_Y-FIRST_HEIGHT/2)&&y<(BACK_Y+FIRST_HEIGHT/2))
    {
        MyData::playSound("bt_press","mp3");//播放声音
        MyData::switchViewId= Constant::MAIN_VIEW_ID;
    }
    
}

ChooseLevelView::~ChooseLevelView(){}
