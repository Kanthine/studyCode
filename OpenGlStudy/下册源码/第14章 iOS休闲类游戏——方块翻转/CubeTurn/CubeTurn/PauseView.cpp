#include "MatrixState2D.hpp"
#include "LoadResourceUtil.h"
#include "PauseView.hpp"
#include "MyData.h"
#include "Constant.hpp"

PauseView::PauseView(){}

void PauseView::Initialize(int width, int height)
{//初始化函数
    
    levelName = new BN2DObject(false,0,LoadResourceUtil::initTexture("onelevel",false),LEVEL_NAME_WIDTH,LEVEL_NAME_HEIGHT);
    backGame = new BN2DObject(false,0,LoadResourceUtil::initTexture("backgame",false),BACK_GAME_WIDTH,BACK_GAME_HEIGHT);
    restart = new BN2DObject(false,0,LoadResourceUtil::initTexture("restart",false),BACK_GAME_WIDTH,BACK_GAME_HEIGHT);
    levelSolution = new BN2DObject(false,0,LoadResourceUtil::initTexture("levelsolution",false),BACK_GAME_WIDTH,BACK_GAME_HEIGHT);
    levelSelect = new BN2DObject(false,0,LoadResourceUtil::initTexture("selectlevel",false),BACK_GAME_WIDTH,BACK_GAME_HEIGHT);
    littleMenu = new BN2DObject(false,0,LoadResourceUtil::initTexture("littlemenu",false),BACK_GAME_WIDTH,BACK_GAME_HEIGHT);
    black = new BN2DObject(false,0,LoadResourceUtil::initTexture("exitbg",false),BLACK_GAME_WIDTH,BLACK_GAME_HEIGHT);
    moveStep = new BN2DObject(false,0,LoadResourceUtil::initTexture("movestep",false),MOVESTEP_WIDTH,MOVESTEP_HEIGHT);
    shortStep = new BN2DObject(false,0,LoadResourceUtil::initTexture("shortstep",false),MOVESTEP_WIDTH,MOVESTEP_HEIGHT);
    GLuint white = LoadResourceUtil::initTexture("whitenumber",false);
    snNumber = new StepNumber(white,white,ICON_WIDTH,ICON_HEIGHT);
    initDatas(false,false);
}
void PauseView::Render(int texId,StepNumber* sn,int BEST_COUNT,int countDrawTrans) {
    if(isDrawInfo){
        glDisable(GL_DEPTH_TEST);//关闭深度检测
        drawMenuView(texId);
        //绘制移动步数
        MatrixState2D::pushMatrix();
        MatrixState2D::translate(0.15f, MOVESTEP_Y, 0.5f);
        snNumber->drawSelf(BEST_COUNT,countDrawTrans,ICON_WIDTH);
        MatrixState2D::popMatrix();
        
        //绘制移动步数
        MatrixState2D::pushMatrix();
        MatrixState2D::translate(0.15f, SHORSTEP_Y, 0.5f);
        snNumber->drawShortStep(BEST_COUNT,ICON_WIDTH);
        MatrixState2D::popMatrix();
        glEnable(GL_DEPTH_TEST);//打开深度检测
    }
}

//绘制暂停时菜单的界面
void PauseView::drawMenuView(int texId)
{
    black->drawSelf(0, -0.02f,1.0f);//绘制背景
    levelName->setTexId(texId);
    levelName->drawSelf(LEVEL_NAME_X, LEVEL_NAME_Y,1.0f);//
    backGame->drawSelf(LEVEL_NAME_X,BACK_GAME_Y,1.0f);//返回游戏按钮绘制者绘制
    restart->drawSelf(LEVEL_NAME_X, RESTART_Y,1.0f);//重新开始按钮绘制者绘制
    levelSolution->drawSelf(LEVEL_NAME_X,LEVEL_SOLUTION,1.0f);
    levelSelect->drawSelf(LEVEL_NAME_X, LEVEL_SELECT,1.0f);//选择关卡按钮绘制者绘制
    littleMenu->drawSelf(LEVEL_NAME_X, LITTLE_MENU,1.0f);//
    moveStep->drawSelf(MOVESTEP_X,MOVESTEP_Y, 1.0f);
    shortStep->drawSelf(MOVESTEP_X,SHORSTEP_Y, 1.0f);
}
void PauseView::OnFingerDown(float x,float y){
    
    if(x>(LEVEL_NAME_X-BACK_GAME_WIDTH/2)&&x<(LEVEL_NAME_X+BACK_GAME_WIDTH/2)
       &&y>(BACK_GAME_Y-BACK_GAME_HEIGHT/2)&&y<(BACK_GAME_Y+BACK_GAME_HEIGHT/2))
    {//返回游戏
        MyData::playSound("bt_press","mp3");//播放声音
        isDrawInfo=false;
    }
    else if(x>(LEVEL_NAME_X-BACK_GAME_WIDTH/2)&&x<(LEVEL_NAME_X+BACK_GAME_WIDTH/2)
            &&y>(RESTART_Y-BACK_GAME_HEIGHT/2)&&y<(RESTART_Y+BACK_GAME_HEIGHT/2))
    {//重新开始
        MyData::playSound("bt_press","mp3");//播放声音
        initDatas(false,true);//初始化游戏中的变量等数据
    }
    else if(x>(LEVEL_NAME_X-BACK_GAME_WIDTH/2)&&x<(LEVEL_NAME_X+BACK_GAME_WIDTH/2)
            &&y>(LEVEL_SOLUTION-BACK_GAME_HEIGHT/2)&&y<(LEVEL_SOLUTION+BACK_GAME_HEIGHT/2))
    {//跳过本关
        MyData::playSound("bt_press","mp3");//播放声音
        MyData::level += 1;
        if(MyData::level == 4)
        {
            MyData::level = 0;
            MyData::switchViewId =  Constant::CHOOSE_VIEW_ID;//选择关卡
        }
        initDatas(false,true);//初始化游戏中的变量等数据
    }
    else if(x>(LEVEL_NAME_X-BACK_GAME_WIDTH/2)&&x<(LEVEL_NAME_X+BACK_GAME_WIDTH/2)
            &&y>(LEVEL_SELECT-BACK_GAME_HEIGHT/2)&&y<(LEVEL_SELECT+BACK_GAME_HEIGHT/2))
    {
        MyData::playSound("bt_press","mp3");//播放声音
        MyData::switchViewId =  Constant::CHOOSE_VIEW_ID;//选择关卡
        initDatas(false,true);//初始化游戏中的变量等数据
    }
    else if(x>(LEVEL_NAME_X-BACK_GAME_WIDTH/2)&&x<(LEVEL_NAME_X+BACK_GAME_WIDTH/2)
            &&y>(LITTLE_MENU-BACK_GAME_HEIGHT/2)&&y<(LITTLE_MENU+BACK_GAME_HEIGHT/2))
    {
        MyData::playSound("bt_press","mp3");//播放声音
        MyData::switchViewId =  Constant::MAIN_VIEW_ID;//显示菜单
        initDatas(false,true);//初始化游戏中的变量等数据
    }
}


void PauseView::initDatas(bool drawInfo,bool initData)
{
    isDrawInfo=drawInfo;
    isInitData=initData;
}


PauseView::~PauseView(){}


