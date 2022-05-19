#ifndef GameView_hpp
#define GameView_hpp

#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#include <stdio.h>
#include "MyData.h"
#include "StepNumber.h"
#include "Constant.hpp"
#include "PauseView.hpp"

using namespace std;
class GameView
{
public:
    GameView();
    void Initialize(int width, int height);
    void Render() ;
    void OnFingerUp(float locationx,float locationy);
    void OnFingerDown(float x,float y,float locationx,float locationy,int width,int height);
    void OnFingerMove(float previousx,float previousy,float currentx,float current);
    void OnFingerPinched(float scale);//多点触摸，扩大或者缩小
    void OnOneFingerTwoTaps();//单手指双击两次，出菜单
    void OnTwoFingersTwoTaps();//双手指双击两次，后退一步
    void initSceneData(int level);//初始化场景数据
    ~GameView();
    
private:

    LoadedObjectVertexNormalTexture *cube5;
    LoadedObjectVertexNormalTexture *cube4;
    LoadedObjectVertexNormalTexture *cube3;
    LoadedObjectVertexNormalTexture *cube2;
    LoadedObjectVertexNormalTexture *cylinder;
    LoadedObjectVertexNormalTexture *plane;
    
    GLuint cube1TextureId;
    GLuint redCubeTextureId;
    GLuint greenCubeTextureId;
    GLuint cube2TextureId;
    GLuint cylinderTextureId;
    GLuint planeTextureId;
    GLuint numberBlueTexId;
    GLuint numberRedTexId;
    GLuint greenBlockTextureId;
    GLuint blueBlockTextureId;

    
    static float upCubeMoveData[48];
    static float saveUpCubeMoveData[48];
    static int stepCount[2];
    
    
    bool isCubeTouch=false;//是否触摸可移动方块
    bool anmiFlag=false;//是否移动木块
    bool isFinish=false;//是否木块移动到指定位置
    bool isScaleScene=false;//是否缩放
    bool isBack=false;//是否后退
    bool isBanTouch = false;//是否禁止触控方块
    
    float direction = 180;//摄像机方向角
    float angleCamera = 63;//摄像机仰角
    float distance = 158;//摄像机与目标点距离
    
    float currScale = 1;
    float left;
    float right;
    float bottom;
    float top;
    float near;
    float far;
    float cameraX=-5;//摄像机位置
    float cameraY=60;
    float cameraZ=110;
    float targetX=0;//摄像机目标点
    float targetY=0;
    float targetZ=0;
    float upX=0;
    float upY=0.94f;
    float upZ=-0.33f;
    float lightX=-5;//灯光位置
    float lightY=160;
    float lightZ=110;
    
    float moveStep=0;//木块移动的次数
    float angleCube=0;//小立方体不断旋转的角度
    
    float touchX;
    float touchY;
    
    int CAMERA_DIRECTION = 0;
    int countDrawTast=0;//调用任务方法的次数
    int waitCount = 0;//结束后的等待计数器
    
    
    int *currMAP_PANE;//地图格子是否通过的数据
    int *currMapObjectPane;//获取地图中目标格子的数据
    float *currMAP_DATA;//获取地图中每个格子的坐标数据
    vector<float *> cubesData;//方块的初始坐标，x,y,z
    int currMapPaneRow;//获取地图格子的行数
    int currMapPaneCol;//获取地图格子的列数
    
    float* currData;//当前绘制数据
    int currRow;//当前绘制数据的行数
    int currCol;//当前绘制数据的列数
    int currBestCount;
    int currLevelTexId;//当前关卡的纹理
    
    StepNumber* sn;
    MyData* md;
    PauseView* mPauseView;
    float* SaveCamera;
    float* SaveLight;
    
    
    vector<LoadedObjectVertexNormalTexture *> vectorLOVN;
    bool judgeFinish(int tempCol,int tempRow,int object_value);//判断木块是否移动到指定位置
    void DrawCubeDownOrUp(int index);
    void drawSenceMoveCubes();//绘制场景中可移动的木块
    void drawNotMoveObjects();//绘制不可移动的物体
    void DrawGameViewPlane(int i);//绘制游戏场景中的地板
    void DrawGameViewFrame(int i);//绘制游戏场景中的边框
    void DrawGameViewCorner(int i);//绘制游戏场景中的黑边角
    void DrawGameViewSmallCube(int i);//绘制游戏场景中的旋转立方体
    void DrawGameViewThinCube(int i);//绘制游戏场景中的机关方块
    void DrawGameViewBlockCube(int i);//绘制游戏场景中的可升降方块
    void initObjectData();//初始化非可移动木块的方法
    void calCamera(float dx,float dy);//重新计算摄像机参数
    void updateEvery(int state,float disX,float disY,bool isBack);
    void doTast();//任务方法

};


#endif /* GameView_hpp */
