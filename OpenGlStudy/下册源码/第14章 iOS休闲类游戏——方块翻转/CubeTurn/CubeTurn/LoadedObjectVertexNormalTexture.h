#ifndef __Sample14_4__LoadedObjectVertexNormalTexture__
#define __Sample14_4__LoadedObjectVertexNormalTexture__

#include <stdio.h>

#include <iostream>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#include "AABB3.h"
#include <map>
#include "MyData.h"

class LoadedObjectVertexNormalTexture {
private:
    GLuint mProgram;//自定义渲染管线程序id
    GLuint muMVPMatrixHandle;//总变换矩阵引用id
    GLuint muMMatrixHandle;//位置、旋转变换矩阵
    GLuint maPositionHandle; //顶点位置属性引用id
    GLuint maNormalHandle; //顶点法向量属性引用id
    GLuint maLightLocationHandle;//光源位置属性引用id
    GLuint maCameraHandle; //摄像机位置属性引用id
    GLuint maTexCoorHandle; //顶点纹理坐标属性引用id
    
    const GLvoid* mVertexBuffer;//顶点坐标数据缓冲
    const GLvoid* mTexCoorBuffer;//顶点纹理坐标数据缓冲
    const GLvoid* mNormalBuffer;//顶点法向量数据缓冲
    

    static int colAndRow[4];//变色方块的行列值
    static float location[4];//变色方块的位置坐标
    static int X_OFFSET_CHANGE[4];
    static int Z_OFFSET_CHANGE[4];
    static int ROTATE_ANMI_ID[4];
    
    
    int vCount;
    int MAP_COl_CUR;//当前关卡的列数
    int MAP_ROW_CUR;//当前关卡的行数
    float* MAP_DATA_CUR;//当前关卡地图格子的位置数据
    int* MAP_CUR;//当前关卡地图格子是否可通过的数据
    float afterXOffset;
    float afterZOffset;
    map<int,float*> alPoints;//纪录木块每一步移动的位置，便于后退时使用
    map<int, string> alStr;
    map<int, bool>   alBool;
    MyData* md;
    bool cubeOnControl;
 
    void cubeUpAndDown(float x,float z,int level,bool isUpOrDown);//后退时设置可升降方块的标志位
    void keyPress(int keyNumber,int tCol,int tRow,bool isBack);//根据按键状态计算下一步的位置
    void setThirdLevelCubeUpState(int index);//设置第三关的方块可升降状态
    void setForthLevelCubeUpState(int index);//设置第四关的方块可升降状态
    bool isCubeOcclusion(int j,int i);
    void setCubeUpState();//设置可升降方块的升降状态
    int* ifPointsExists(float tempX,float tempZ,int tCol,int tRow,bool go);

    
public:
    AABB3* preBox;
    float *m = new float[16];
    
    LoadedObjectVertexNormalTexture(float* vertices,float* normals,float* texCoors,int vCount,int flag);
    void initVertexData(float* vertices,float* normals,float* texCoors,int count);
    void initShader();
    void drawSelf(int texId);
    AABB3* getCurrBox();
    void copyM();
    //根据摄像机位置更新按键的状态
    void updateKeyStatus(int camearStatus,float distanceX,float distanceY,int tCol,int tRow,bool isBack);
    void keyChange(int state,bool isCube);
    void reCoverVariables(int viewId,int level,bool isBack); //旋转动画完成后恢复扰动
    void initVariables(float* data);//重置变量
    void initCubes(float * data);
    void reCoverHelpView();
    //变色的情况下，改变该物体的类型
    void changeType(int flagTemp);
    void setInitCubeLocation(MyData* md,float* data,int col,int row,float* map_data,int* map);//初始化方块位置
    void back(int level);  //后退一步的方法
    
    float angleZ=0;//实时旋转扰动角度   绕Z轴
    float angleX=0;//实时旋转扰动角度   绕X轴
    float angleY=0;//实时旋转扰动角度   绕Y轴
    
    float tempCenterX=0;//旋转临时X扰动值
    float tempCenterY=0;//旋转临时Y扰动值
    float tempCenterZ=0;//旋转临时Z扰动值
    
    float mCubeTransX=0;//方块的x坐标
    float mCubeTransY=0;//方块的y坐标
    float mCubeTransZ=0;//方块的z坐标
    
    float angleDraw=0;//木块旋转的角度
    
    bool isTwoSteps=false;//每次是否移动两步
    bool isTouch=false;//是否被触摸
    bool isMove=false;
    bool isChangeColor=false;//是否改变颜色
    int flag;//类型：0表示蓝色立方体，1表示绿色立方体，2表示红色立方体
    int keyState=-1;
    int currCol=-1;
    int currRow=-1;
    
    bool isFirstFinish = false;
    float tempX = 0;
    float tempY = 0;
    float tempZ = 0;
    int count=0;//移动的步数
    
    ~LoadedObjectVertexNormalTexture();
};

#endif /* defined(__Sample14_4__LoadedObjectVertexNormalTexture__) */
