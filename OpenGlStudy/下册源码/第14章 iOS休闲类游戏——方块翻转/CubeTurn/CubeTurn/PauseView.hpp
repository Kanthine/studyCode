#ifndef PauseView_hpp
#define PauseView_hpp

#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#include <stdio.h>
#include "BN2DObject.h"
#include "StepNumber.h"
class PauseView{
    public :
    bool isDrawInfo=false;
    bool isInitData=false;//是否初始化数据
    PauseView();
    void Initialize(int width, int height);
    void Render(int texId,StepNumber* sn,int BEST_COUNT,int countDrawTrans) ;
    void OnFingerDown(float locationx,float locationy);
    void initDatas(bool drawInfo,bool initData);//初始化变量
    ~PauseView();
    
private:
    BN2DObject* backGame;//返回游戏绘制者
    BN2DObject* restart;//重新开始绘制者
    BN2DObject* levelSolution;//跳过本关绘制者
    BN2DObject* levelSelect;//选择关卡绘制者
    BN2DObject* littleMenu;//显示菜单绘制者
    BN2DObject* levelName;	//关卡名称绘制者
    BN2DObject* black;
    BN2DObject* moveStep;//移动步数
    BN2DObject* shortStep;//最短步数
    StepNumber* snNumber;
    
    constexpr const static float ICON_WIDTH=0.13f;//宽度
    constexpr const static float ICON_HEIGHT=0.13f;//高度
    constexpr const static float LEVEL_NAME_WIDTH=0.8f;//关卡名称的宽度
    constexpr const static float LEVEL_NAME_HEIGHT=0.2f;//关卡名称的高度
    constexpr const static float BACK_GAME_WIDTH=0.56f;//按钮的宽度
    constexpr const static float BACK_GAME_HEIGHT=0.18f;//按钮的高度
    constexpr const static float BLACK_GAME_WIDTH=1.1f;//按钮的宽度
    constexpr const static float BLACK_GAME_HEIGHT=1.9f;//按钮的高度
    constexpr const static float MOVESTEP_HEIGHT=0.15f;//移动步数的高度
    constexpr const static float MOVESTEP_WIDTH=0.525f;//移动步数的宽度
    constexpr const static float SPAN=0.25f;
    constexpr const static float LEVEL_NAME_Y=0.75f;//关卡名称的y坐标
    constexpr const static float LEVEL_NAME_X=0;//关卡名称的x坐标
    constexpr const static float MOVESTEP_Y=-0.7f;//移动步数的y坐标
    constexpr const static float MOVESTEP_X=-0.2f;//移动步数的x坐标
    constexpr const static float SHORSTEP_Y=-0.85f;//移动步数的y坐标
    constexpr const static float BACK_GAME_Y=LEVEL_NAME_Y-SPAN;//返回游戏按钮的y坐标
    constexpr const static float RESTART_Y=BACK_GAME_Y-SPAN;//重新开始按钮的y坐标
    constexpr const static float LEVEL_SOLUTION=RESTART_Y-SPAN;//跳过本关按钮的y坐标
    constexpr const static float LEVEL_SELECT=LEVEL_SOLUTION-SPAN;//选择关卡按钮的y坐标
    constexpr const static float LITTLE_MENU=LEVEL_SELECT-SPAN;//显示菜单按钮的y坐标
    void drawMenuView(int texId);//绘制暂停时菜单界面
};


#endif /* PauseView_hpp */
