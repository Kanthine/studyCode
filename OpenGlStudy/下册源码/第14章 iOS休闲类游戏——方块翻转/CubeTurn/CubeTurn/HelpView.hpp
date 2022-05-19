#ifndef HelpView_hpp
#define HelpView_hpp
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#include <stdio.h>
#include "LoadedObjectVertexNormalTexture.h"
#include <vector>
#include "MyData.h"
#include "BN2DObject.h"
class HelpView
{
public:
    int levelHelp = 0;//关卡
    bool isInitDatas = false;
    HelpView();
    void Initialize(int width, int height);
    void initVariables();//初始化变量
    void Render() ;
    void OnFingerDown(float locationx,float locationy);
    ~HelpView();
private:

    float left;
    float right;
    float bottom;
    float top;
    float near;
    float far;
    float angleCube;//小方块的旋转角度
    float fingerMoveX;//手指移动坐标
    float fingerMoveY;
    float sj=1;//透明度因子
    int fingerCount;//手指移动的次数
    
    int countDrawTastHelp ;//任务执行方法的计数器
    bool anmiFlag = true;
        
    //纹理id
    GLuint planeTextureId;
    GLuint cube1TextureId;
    GLuint redCubeTextureId;
    GLuint greenCubeTextureId;
    GLuint blockCubeTextureId;
    GLuint fingerTexureId;
    GLuint  texId1;
    GLuint  texId2;
    GLuint  texId3;
    GLuint  texId4;
    
    
    BN2DObject* finger;//绘制手指
    BN2DObject* back;//绘制返回
    BN2DObject* next;//绘制下一关
    BN2DObject* levelName;
    BN2DObject* reSet;//重放
    static float firstDrawDatas[14];
    static float secDrawDatas[42];
    static float thirdDrawDatas[35];
    static float fourDrawDatas[49];
    static float upCubeData[6];
    static float saveUpCubeData[6];

    constexpr const static float FINGER_WIDTH=0.3f;//关卡名称的宽度
    constexpr const static float FINGER_HEIGHT=0.3f;//关卡名称的高度
    
    constexpr const static float BACK_X=-0.35f;//返回按钮最小x坐标
    constexpr const static float BACK_Y=-0.8f;//返回按钮最小x坐标
    
    constexpr const static float NEXT_X=0.37f;//返回按钮最小x坐标
    constexpr const static float NEXT_Y=-0.8f;//返回按钮最小x坐标
    
    constexpr const static float RESET_X=0.035f;//重放按钮最小x坐标
    constexpr const static float RESET_Y=-0.8f;//重放按钮最小x坐标
    
    constexpr const static float BACK_WIDTH=0.32f;//返回菜单界面按钮的宽度
    constexpr const static float BACK_HEIGHT=0.2f;
    
    constexpr const static float NEXT_WIDTH=0.3f;//下一关按钮的宽度
    constexpr const static float NEXT_HEIGHT=0.2f;
    
    constexpr const static float RESET_WIDTH=0.2f;//重放按钮的宽度
    constexpr const static float RESET_HEIGHT=0.2f;
    
    constexpr const static float LEVEL_NAME_WIDTH=0.8f;//关卡名称的宽度
    constexpr const static float LEVEL_NAME_HEIGHT=0.2f;//关卡名称的高度
    
    constexpr const static float LEVEL_NAME_Y=0.75f;//关卡名称的y坐标
    constexpr const static float LEVEL_NAME_X=0;//关卡名称的x坐标

    LoadedObjectVertexNormalTexture* plane;
    LoadedObjectVertexNormalTexture* smallCube;
    LoadedObjectVertexNormalTexture* thinCube;
    LoadedObjectVertexNormalTexture* blockCube;
    map<int, vector<LoadedObjectVertexNormalTexture*>> mapLOVN;
    map<int, float*> mapObject;//目标点坐标数据
    map<int, float*> mapOrign;//起点坐标数据
    
    void moveFinger();//移动手指
    void doTast();
    void initHelpViewFirst();
    void initHelpViewSecond();
    void initHelpViewThird();
    void initHelpViewFourth();
    void initHelpViewDatas();//初始化绘制数据
    void drawSenceMoveCubes(LoadedObjectVertexNormalTexture* temp);
    void drawGameViewNotCubes(float *currData,int length,int currCol);
    void drawBN2DObject();//绘制2D物体
    
};

#endif /* HelpView_hpp */
