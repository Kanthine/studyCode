//
//  MyData.cpp
//  Sample14_4
//
//  Created by wuyafeng on 15/9/13.
//  Copyright (c) 2015年 wuyafeng. All rights reserved.
//

#include "MyData.h"
#include "Constant.hpp"
#include <string>
#include "LoadedObjectVertexNormalTexture.h"
#include "LoadUtil.h"
#include "LoadResourceUtil.h"
using namespace std;


bool MyData::everCubeUp[2] = {false,false};
bool MyData::isCubeUp[8] = {false,false,false,false,true,true,true,true};
bool MyData::saveIsCubeUp[8] = {false,false,false,false,true,true,true,true};
float MyData::upCubeRowAndColData[16]={-1,-1,-1,-1,-1,-1,-1,-1,1,4,3,4,4,2,2,6};
float MyData::controlCubeData[16]={-100,-100,-100,-100,-100,-100,-100,-100,5,-30,25,-20,-5,-50,-5,-10};
int MyData::switchViewId = Constant::MAIN_VIEW_ID;
int MyData::level=0;
bool MyData::musicOff=false;
bool MyData::effectOff=false;
MyData::MyData(){}

MyData::~MyData(){}

void MyData::initFirstLevelDatas(){
    //=========第一关卡的绘制数据  start===============
    dataMapRow[0] = 26;
    dataMapCol[0] = 12;
    float temp1First[26*12]=
    {
        0,-15.0f,-16.0f,-40.0f,2.0f,2.0f,2.0f,0,0,0,0,0,
        0,-5.0f,-16.0f,-40.0f,2.0f,2.0f,2.0f,0,0,0,0,0,
        0,5.0f,-16.0f,-40.0f,2.0f,2.0f,2.0f,0,0,0,0,0,
        0,-15.0f,-16.0f,-30.0f,2.0f,2.0f,2.0f,0,0,0,0,0,
        0,-5.0f,-16.0f,-30.0f,2.0f,2.0f,2.0f,0,0,0,0,0,
        0,5.0f,-16.0f,-30.0f,2.0f,2.0f,2.0f,0,0,0,0,0,
        0,-15.0f,-16.0f,-20.0f,2.0f,2.0f,2.0f,0,0,0,0,0,
        0,-5.0f,-16.0f,-20.0f,2.0f,2.0f,2.0f,0,0,0,0,0,
        0,5.0f,-16.0f,-20.0f,2.0f,2.0f,2.0f,0,0,0,0,0,
        0,15.0f,-16.0f,-30.0f,2.0f,2.0f,2.0f,0,0,0,0,1,

        1,-5.0f,-16.0f,-15.0f,0.6f,1,1,90,0,1,0,0,
        1,-5.0f,-16.0f,-45.0f,0.6f,1,1,90,0,1,0,0,
        1,15.0f,-16.0f,-35.0f,0.2f,1,1,90,0,1,0,0,
        1,15.0f,-16.0f,-25.0f,0.2f,1,1,90,0,1,0,0,
        1,10.0f,-16.0f,-40.0f,1,1,0.2f,0,0,1,0,0,
        1,10.0f,-16.0f,-20.0f,1,1,0.2f,0,0,1,0,0,
        1,-20.0f,-16.0f,-30.0f,1,1,0.6f,0,0,1,0,0,
        1,20.0f,-16.0f,-30.0f,1,1,0.2f,0,0,1,0,0,

        2,-20.0f,-16.0f,-15.0f,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,-20.0f,-16.0f,-45.0f,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,10.0f,-16.0f,-45.0f,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,10.0f,-16.0f,-15.0f,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,10.0f,-16.0f,-35.0f,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,10.0f,-16.0f,-25.0f,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,20.0f,-16.0f,-35.0f,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,20.0f,-16.0f,-25.0f,0.6f,0.6f,0.6f,0,0,0,0,0
    };
    float* temp2First = new float[26*12];
    for(int i = 0; i < (26*12); i++){
        temp2First[i] = temp1First[i];
    }
    dataMapDraw[0] = temp2First;
    //=========第一关卡的绘制数据  end====================
    
    //=========第一关卡的地图数据  start==================
    mapPaneRow[0]=5;//第一关卡地图数据的行数
    mapPaneCol[0]=6;//第一关卡地图数据的列数
    int MAP_FIRST[6*5]=//第一关卡的地图数据
    {
        0,0,0,0,0,0,
        0,1,1,1,0,0,
        0,1,1,1,1,0,
        0,1,1,1,0,0,
        0,0,0,0,0,0
    };
    int* tempMap3FIRST=new int[30];
    for(int i=0;i<30;i++)
    {
        tempMap3FIRST[i]=MAP_FIRST[i];
    }
    mapPaneData[0] = tempMap3FIRST;
    //=========第一关卡的地图数据  end======================
    
    //=========第一关卡目标格子可否通过的数据  start==========
    int* tempMap4FIRST=new int[30];
    int MAP_OBJECT_FIRST[30]=//第一关卡的目标格子的数据
    {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,1,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0
    };
    for(int i=0;i<30;i++)
    {
        tempMap4FIRST[i]=MAP_OBJECT_FIRST[i];
    }
    mapObjectPaneData[0]=tempMap4FIRST;
    //=========第一关卡目标格子可否通过的数据  end===========
    
    mapObjectPaneNum[0]=1;//第一关卡中目标格子的数量
    
    mapBestCount[0]=2;//第一关卡中目标格子的最少步数
    
    
    //=========第一关卡目标格子的位置坐标数据  start==========
    mapMIN_X[0]=-25;
    mapMIN_Z[0]=-50;
    float  *tempMAPDATA_FIRST=new float[30*2];
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<6;j++)
        {
            tempMAPDATA_FIRST[(i*6+j)*2+0]=-25+Constant::CubeSize*j;
            tempMAPDATA_FIRST[(i*6+j)*2+1]=-50+Constant::CubeSize*i;
        }
    }
    mapDataXZ[0]=tempMAPDATA_FIRST;
    //=========第一关卡目标格子的位置坐标数据  end============
    
    float *SaveCubeData=new float[5];
    SaveCubeData[0]=-5;
    SaveCubeData[1]=-11;
    SaveCubeData[2]=-30;
    SaveCubeData[3]=3;
    SaveCubeData[4]=3;
    vector<float*> vectorFirst;
    vectorFirst.push_back(SaveCubeData);
    mapCubeLocation[0] = vectorFirst;//方块的初始位置
    
    
    float SaveCamera[9]={-5,60,110,   0,0,0,   0,0.94f,-0.33f};
    float* tempCamera=new float[9];
    for(int i=0;i<9;i++)
    {
        tempCamera[i]=SaveCamera[i];
    }
    mapCamera[0]=tempCamera;//摄像机的初始位置
    
    
    float *tempLight=new float[3];
    tempLight[0]=-5;
    tempLight[1]=50;
    tempLight[2]=20;
    mapLight[0]=tempLight;//光源的初始位置
    
    LoadedObjectVertexNormalTexture* firstLOVN=LoadUtil::loadFromFile("cube1",Constant::BLUE_CUBEID);
    vector<LoadedObjectVertexNormalTexture*> tempFirst;
    tempFirst.push_back(firstLOVN);
    mapLOVN[0]=tempFirst;
    
    mapTexIds[0] = LoadResourceUtil::initTexture("onelevel", false);
    //=======================first  end============================
}

void MyData::initSecLevelDatas(){
    //=========第2关卡的绘制数据  start===============
    dataMapRow[1] = 26;
    dataMapCol[1] = 12;
    float temp1SEC[26*12]=
    {
        //地面方格数据
        0,-15,-16,-40,2,2,2,0,0,0,0,0,
        0,-5,-16,-40,2,2,2,0,0,0,0,0,
        0,5,-16,-40,2,2,2,0,0,0,0,0,
        0,-15,-16,-30,2,2,2,0,0,0,0,0,
        0,-5,-16,-30,2,2,2,0,0,0,0,0,
        0,5,-16,-30,2,2,2,0,0,0,0,1,
        0,-15,-16,-20,2,2,2,0,0,0,0,0,
        0,-5,-16,-20,2,2,2,0,0,0,0,0,
        0,5,-16,-20,2,2,2,0,0,0,0,0,
        0,15,-16,-30,2,2,2,0,0,0,0,2,
        //绘制周围边框
        1,-5,-16,-15,0.6f,1,1,90,0,1,0,0,
        1,-5,-16,-45,0.6f,1,1,90,0,1,0,0,
        1,15,-16,-35,0.2f,1,1,90,0,1,0,0,
        1,15,-16,-25,0.2f,1,1,90,0,1,0,0,
        1,10,-16,-40,1,1,0.2f,0,0,1,0,0,
        1,10,-16,-20,1,1,0.2f,0,0,1,0,0,
        1,-20,-16,-30,1,1,0.6f,0,0,1,0,0,
        1,20,-16,-30,1,1,0.2f,0,0,1,0,0,
        //绘制黑边角
        2,-20,-16,-15,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,-20,-16,-45,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,10,-16,-45,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,10,-16,-15,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,10,-16,-35,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,10,-16,-25,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,20,-16,-35,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,20,-16,-25,0.6f,0.6f,0.6f,0,0,0,0,0
    };
    float* temp2SEC = new float[26*12];
    for(int i = 0; i < 286; i++){
        temp2SEC[i] = temp1SEC[i];
    }
    dataMapDraw[1] = temp2SEC;
    //=========第2关卡的绘制数据  end====================
    
    //=========第2关卡的地图数据  start==================
    mapPaneRow[1]=5;//第2关卡地图数据的行数
    mapPaneCol[1]=6;//第2关卡地图数据的列数
    int MAP_SEC[6*5]=//第2关卡的地图数据
    {
        0,0,0,0,0,0,
        0,1,1,1,0,0,
        0,1,1,1,1,0,
        0,1,1,1,0,0,
        0,0,0,0,0,0
    };
    int* tempMap3SEC=new int[30];
    for(int i=0;i<30;i++)
    {
        tempMap3SEC[i]=MAP_SEC[i];
    }
    mapPaneData[1] = tempMap3SEC;
    //=========第2关卡的地图数据  end======================
    
    //=========第2关卡目标格子可否通过的数据  start==========
    int* tempMap4SEC=new int[30];
    int MAP_OBJECT_SEC[30]=//第一关卡的目标格子的数据
    {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,1,2,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0
    };
    for(int i=0;i<30;i++)
    {
        tempMap4SEC[i]=MAP_OBJECT_SEC[i];
    }
    mapObjectPaneData[1]=tempMap4SEC;
    //=========第2关卡目标格子可否通过的数据  end===========
    
    mapObjectPaneNum[1]=2;//第2关卡中目标格子的数量
    
    mapBestCount[1]=6;//第2关卡中目标格子的最少步数
    
    
    //=========第2关卡目标格子的位置坐标数据  start==========
    mapMIN_X[1]=-25;
    mapMIN_Z[1]=-50;
    float  *tempMAPDATA_SEC=new float[30*2];
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<6;j++)
        {
            tempMAPDATA_SEC[(i*6+j)*2+0]=-25+Constant::CubeSize*j;
            tempMAPDATA_SEC[(i*6+j)*2+1]=-50+Constant::CubeSize*i;
        }
    }
    mapDataXZ[1]=tempMAPDATA_SEC;
    //=========第2关卡目标格子的位置坐标数据  end============
    
    float *SaveCubeDataSec=new float[5];
    SaveCubeDataSec[0]=-5;
    SaveCubeDataSec[1]=-11;
    SaveCubeDataSec[2]=-30;
    SaveCubeDataSec[3]=3;
    SaveCubeDataSec[4]=3;
    
    float *SaveCubeDataRedSec=new float[5];
    SaveCubeDataRedSec[0]=-15;
    SaveCubeDataRedSec[1]=-11;
    SaveCubeDataRedSec[2]=-30;
    SaveCubeDataRedSec[3]=2;
    SaveCubeDataRedSec[4]=1;
    vector<float*> vecSec;
    vecSec.push_back(SaveCubeDataSec);
    vecSec.push_back(SaveCubeDataRedSec);
    mapCubeLocation[1]=vecSec;//方块的初始位置
    
    
    float SaveCameraSec[9]={-5,60,110,   0,0,0,   0,0.94f,-0.33f};
    float* tempCameraSec=new float[9];
    for(int i=0;i<9;i++)
    {
        tempCameraSec[i]=SaveCameraSec[i];
    }
    mapCamera[1]=tempCameraSec;//摄像机的初始位置
    
    
    float *tempLightSec=new float[3];
    tempLightSec[0]=-5;
    tempLightSec[1]=50;
    tempLightSec[2]=20;
    mapLight[1]=tempLightSec;//光源的初始位置
    
    LoadedObjectVertexNormalTexture* firstLOVN=LoadUtil::loadFromFile("cube1",Constant::BLUE_CUBEID);
    LoadedObjectVertexNormalTexture* secLOVN=LoadUtil::loadFromFile("cube1",Constant::RED_CUBEID);
    vector<LoadedObjectVertexNormalTexture*> tempSec;
    tempSec.push_back(firstLOVN);
    tempSec.push_back(secLOVN);
    mapLOVN[1]=tempSec;
    mapTexIds[1] = LoadResourceUtil::initTexture("twolevel", false);
    //===============sec   end=========================
}
void MyData::initThiLevelDatas()
{
    //=========第3关卡的绘制数据  start===============
    dataMapRow[2] = 37;
    dataMapCol[2] = 12;
    float transY=-16;
    float temp1THI[37*12]=
    {
        //最上排
        0,-35,transY,-40,2,2,2,0,0,0,0,2,
        0,-25,transY,-40,2,2,2,0,0,0,0,0,
        0,-15,transY,-40,2,2,2,0,0,0,0,0,
        0,-5,transY,-40,2,2,2,0,0,0,0,0,
        0,5,transY,-40,2,2,2,0,0,0,0,0,
        0,15,transY,-40,2,2,2,0,0,0,0,0,
        0,25,transY,-40,2,2,2,0,0,0,0,0,
        //中间排
        0,5,transY,-30,2,2,2,0,0,0,0,0,
        0,15,transY,-30,2,2,2,0,0,0,0,1,
        0,25,transY,-30,2,2,2,0,0,0,0,0,
        //最下排
        0,-35,transY,-20,2,2,2,0,0,0,0,0,
        0,-25,transY,-20,2,2,2,0,0,0,0,0,
        0,-15,transY,-20,2,2,2,0,0,0,0,0,
        0,-5,transY,-20,2,2,2,0,0,0,0,0,
        0,5,transY,-20,2,2,2,0,0,0,0,0,
        0,15,transY,-20,2,2,2,0,0,0,0,0,
        0,25,transY,-20,2,2,2,0,0,0,0,0,
        
        1,-5,transY,-45,1.4f,1,1,90,0,1,0,0,
        1,30,transY,-30,1,1,0.6f,0,0,1,0,0,
        1,-5,transY,-15,1.4f,1,1,90,0,1,0,0,
        1,-40,transY,-20,1,1,0.2f,0,0,1,0,0,
        1,-20,transY,-25,0.8f,1,1,90,0,1,0,0,
        1,0,transY,-30,1,1,0.2f,0,0,1,0,0,
        1,-20,transY,-35,0.8f,1,1,90,0,1,0,0,
        1,-40,transY,-40,1,1,0.2f,0,0,1,0,0,
        
        2,-40,transY,-45,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,30,transY,-45,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,30,transY,-15,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,-40,transY,-15,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,-40,transY,-25,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,0,transY,-25,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,0,transY,-35,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,-40,transY,-35,0.6f,0.6f,0.6f,0,0,0,0,0,
        
        //薄长方体
        4,5,-15.7f,-30,1,1,1,0,0,0,0,1,//蓝色小长方体
        4,25,-15.7f,-20,1,1,1,0,0,0,0,1,//蓝色小长方体
        
        //边框立方体
        8,-5,-11,-40,1,1,1,0,0,0,0,1,//蓝色小长方体
        8,-5,-11,-20,1,1,1,0,0,0,0,1,//红色小长方体
    };
    float* temp2THI = new float[37*12];
    for(int i = 0; i < (37*12); i++){
        temp2THI[i] = temp1THI[i];
    }
    dataMapDraw[2] = temp2THI;
    //=========第3关卡的绘制数据  end====================
    
    //=========第3关卡的地图数据  start==================
    mapPaneRow[2]=5;//第3关卡地图数据的行数
    mapPaneCol[2]=9;//第3关卡地图数据的列数
    int MAP_THI[45]=//第3关卡的地图数据
    {
        0,0,0,0,0,0,0,0,0,
        0,1,1,1,1,1,1,1,0,
        0,0,0,0,0,1,1,1,0,
        0,1,1,1,1,1,1,1,0,
        0,0,0,0,0,0,0,0,0
    };
    int* tempMap3THI=new int[45];
    for(int i=0;i<45;i++)
    {
        tempMap3THI[i]=MAP_THI[i];
    }
    mapPaneData[2] = tempMap3THI;
    //=========第3关卡的地图数据  end======================
    
    //=========第3关卡目标格子可否通过的数据  start==========
    int* tempMap4THI=new int[45];
    int MAP_OBJECT_THI[45]=//第一关卡的目标格子的数据
    {
        0,0,0,0,0,0,0,0,0,
        0,2,0,0,0,0,0,0,0,
        0,0,0,0,0,0,1,0,0,
        0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0
    };
    for(int i=0;i<45;i++)
    {
        tempMap4THI[i]=MAP_OBJECT_THI[i];
    }
    mapObjectPaneData[2]=tempMap4THI;
    //=========第3关卡目标格子可否通过的数据  end===========
    
    mapObjectPaneNum[2]=2;//第3关卡中目标格子的数量
    
    mapBestCount[2]=10;//第3关卡中目标格子的最少步数
    
    
    //=========第3关卡目标格子的位置坐标数据  start==========
    mapMIN_X[2]=-45;
    mapMIN_Z[2]=-50;
    float  *tempMAPDATA_THI=new float[45*2];
    for(int i=0;i<mapPaneRow[2];i++)
    {
        for(int j=0;j<mapPaneCol[2];j++)
        {
            tempMAPDATA_THI[(i*mapPaneCol[2]+j)*2+0]=mapMIN_X[2]+Constant::CubeSize*j;
            tempMAPDATA_THI[(i*mapPaneCol[2]+j)*2+1]=mapMIN_Z[2]+Constant::CubeSize*i;
        }
    }
    mapDataXZ[2]=tempMAPDATA_THI;
    //=========第3关卡目标格子的位置坐标数据  end============
    float *SaveCubeDataThi=new float[5];
    SaveCubeDataThi[0]=25;
    SaveCubeDataThi[1]=-11;
    SaveCubeDataThi[2]=-40;
    SaveCubeDataThi[3]=7;
    SaveCubeDataThi[4]=1;
    
    float *SaveCubeDataRedThi=new float[5];
    SaveCubeDataRedThi[0]=-35;
    SaveCubeDataRedThi[1]=-11;
    SaveCubeDataRedThi[2]=-20;
    SaveCubeDataRedThi[3]=1;
    SaveCubeDataRedThi[4]=3;
    vector<float*> vecTHI;
    vecTHI.push_back(SaveCubeDataThi);
    vecTHI.push_back(SaveCubeDataRedThi);
    mapCubeLocation[2]=vecTHI;//方块的初始位置
    
    
    float SaveCameraThi[9]={-5,60,110,   0,0,0,   0,0.94f,-0.33f};
    float* tempCameraThi=new float[9];
    for(int i=0;i<9;i++)
    {
        tempCameraThi[i]=SaveCameraThi[i];
    }
    mapCamera[2]=tempCameraThi;//摄像机的初始位置
    
    
    float *tempLightThi=new float[3];
    tempLightThi[0]=-5;
    tempLightThi[1]=50;
    tempLightThi[2]=20;
    mapLight[2]=tempLightThi;//光源的初始位置
    
    LoadedObjectVertexNormalTexture* firstLOVN=LoadUtil::loadFromFile("cube1",Constant::BLUE_CUBEID);
    LoadedObjectVertexNormalTexture* secLOVN=LoadUtil::loadFromFile("cube1",Constant::RED_CUBEID);
    vector<LoadedObjectVertexNormalTexture*> tempThi;
    tempThi.push_back(firstLOVN);
    tempThi.push_back(secLOVN);
    mapLOVN[2]=tempThi;
    mapTexIds[2] = LoadResourceUtil::initTexture("threelevel", false);
    //===============third   end=========================
}

void MyData::initForLevelDatas()
{
    //=========第4关卡的绘制数据  start===============
    dataMapRow[3] = 91;
    dataMapCol[3] = 12;
    float temp1FOR[91*12]=
    {
        //地面方格数据
        0,-35,-16,-50,2,2,2,0,0,0,0,0,
        0,-25,-16,-50,2,2,2,0,0,0,0,0,
        0,-5,-16,-50,2,2,2,0,0,0,0,0,
        0,15,-16,-50,2,2,2,0,0,0,0,0,
        0,25,-16,-50,2,2,2,0,0,0,0,0,
        
        0,-35,-16,-40,2,2,2,0,0,0,0,0,
        0,-25,-16,-40,2,2,2,0,0,0,0,0,
        0,-15,-16,-40,2,2,2,0,0,0,0,0,
        0,-5,-16,-40,2,2,2,0,0,0,0,0,
        0,5,-16,-40,2,2,2,0,0,0,0,0,
        0,15,-16,-40,2,2,2,0,0,0,0,0,
        0,25,-16,-40,2,2,2,0,0,0,0,0,
        
        0,-25,-16,-30,2,2,2,0,0,0,0,0,
        0,-15,-16,-30,2,2,2,0,0,0,0,0,
        0,-5,-16,-30,2,2,2,0,0,0,0,0,
        0,5,-16,-30,2,2,2,0,0,0,0,0,
        0,15,-16,-30,2,2,2,0,0,0,0,0,
        
        0,-35,-16,-20,2,2,2,0,0,0,0,0,
        0,-25,-16,-20,2,2,2,0,0,0,0,0,
        0,-15,-16,-20,2,2,2,0,0,0,0,0,
        0,-5,-16,-20,2,2,2,0,0,0,0,0,
        0,5,-16,-20,2,2,2,0,0,0,0,0,
        0,15,-16,-20,2,2,2,0,0,0,0,0,
        0,25,-16,-20,2,2,2,0,0,0,0,0,
        
        0,-35,-16,-10,2,2,2,0,0,0,0,1,
        0,-25,-16,-10,2,2,2,0,0,0,0,0,
        0,-5,-16,-10,2,2,2,0,0,0,0,0,
        0,15,-16,-10,2,2,2,0,0,0,0,0,
        0,25,-16,-10,2,2,2,0,0,0,0,3,
        //绘制周围边框
        1,-30,-16,-55,0.4f,1,1,90,0,1,0,0,
        1,20,-16,-55,0.4f,1,1,90,0,1,0,0,
        1,-30,-16,-5,0.4f,1,1,90,0,1,0,0,
        1,20,-16,-5,0.4f,1,1,90,0,1,0,0,
        
        1,-40,-16,-45,1,1,0.4f,0,0,1,0,0,
        1,-40,-16,-15,1,1,0.4f,0,0,1,0,0,
        
        //边框立方体
        8,-25,-11,-20,1,1,1,0,0,0,0,1,//蓝色小长方体
        8,15,-11,-40,1,1,1,0,0,0,0,3,//绿色小长方体
        
        1,30,-16,-45,1,1,0.4f,0,0,1,0,0,
        1,30,-16,-15,1,1,0.4f,0,0,1,0,0,
        
        1,-5,-16,-55,0.2f,1,1,90,0,1,0,0,
        1,-5,-16,-5,0.2f,1,1,90,0,1,0,0,
        1,-15,-16,-45,0.2f,1,1,90,0,1,0,0,
        1,5,-16,-45,0.2f,1,1,90,0,1,0,0,
        1,-15,-16,-15,0.2f,1,1,90,0,1,0,0,
        1,5,-16,-15,0.2f,1,1,90,0,1,0,0,
        1,25,-16,-35,0.2f,1,1,90,0,1,0,0,
        1,25,-16,-25,0.2f,1,1,90,0,1,0,0,
        1,-35,-16,-35,0.2f,1,1,90,0,1,0,0,
        1,-35,-16,-25,0.2f,1,1,90,0,1,0,0,
        
        1,-20,-16,-50,1,1,0.2f,0,0,1,0,0,
        1,-10,-16,-50,1,1,0.2f,0,0,1,0,0,
        1,0,-16,-50,1,1,0.2f,0,0,1,0,0,
        1,10,-16,-50,1,1,0.2f,0,0,1,0,0,
        1,-20,-16,-10,1,1,0.2f,0,0,1,0,0,
        1,-10,-16,-10,1,1,0.2f,0,0,1,0,0,
        1,0,-16,-10,1,1,0.2f,0,0,1,0,0,
        1,10,-16,-10,1,1,0.2f,0,0,1,0,0,
        1,-30,-16,-30,1,1,0.2f,0,0,1,0,0,
        1,20,-16,-30,1,1,0.2f,0,0,1,0,0,
        //绘制黑边角
        2,-40,-16,-55,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,-20,-16,-55,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,-10,-16,-55,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,0,-16,-55,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,10,-16,-55,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,30,-16,-55,0.6f,0.6f,0.6f,0,0,0,0,0,
        
        2,-20,-16,-45,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,-10,-16,-45,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,0,-16,-45,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,10,-16,-45,0.6f,0.6f,0.6f,0,0,0,0,0,
        
        2,-40,-16,-35,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,-30,-16,-35,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,20,-16,-35,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,30,-16,-35,0.6f,0.6f,0.6f,0,0,0,0,0,
        
        2,-40,-16,-25,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,-30,-16,-25,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,20,-16,-25,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,30,-16,-25,0.6f,0.6f,0.6f,0,0,0,0,0,
        
        2,-20,-16,-15,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,-10,-16,-15,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,0,-16,-15,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,10,-16,-15,0.6f,0.6f,0.6f,0,0,0,0,0,
        
        2,-40,-16,-5,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,-20,-16,-5,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,-10,-16,-5,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,0,-16,-5,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,10,-16,-5,0.6f,0.6f,0.6f,0,0,0,0,0,
        2,30,-16,-5,0.6f,0.6f,0.6f,0,0,0,0,0,
        
        //旋转的小立方体
        3,-15,-14,-30,1,1,1,45,1,0,1,3,//绿色小立方体  0.6f,0.6f,0.6f
        3,5,-14,-30,1,1,1,45,1,0,1,1,//蓝色小立方体
        
        //薄长方体
        4,-5,-15.7f,-10,1,1,1,0,0,0,0,3,//绿色小长方体
        4,-5,-15.7f,-50,1,1,1,0,0,0,0,1,//蓝色小长方体
    };
    float* temp2FOR = new float[91*12];
    for(int i = 0; i < (91*12); i++){
        temp2FOR[i] = temp1FOR[i];
    }
    dataMapDraw[3] = temp2FOR;
    //=========第4关卡的绘制数据  end====================
    
    //=========第4关卡的地图数据  start==================
    mapPaneRow[3]=7;//第3关卡地图数据的行数
    mapPaneCol[3]=9;//第3关卡地图数据的列数
    int MAP_FOR[63]=//第3关卡的地图数据
    {
        0,0,0,0,0,0,0,0,0,
        0,1,1,0,1,0,1,1,0,
        0,1,1,1,1,1,1,1,0,
        0,0,1,1,1,1,1,0,0,
        0,1,1,1,1,1,1,1,0,
        0,1,1,0,1,0,1,1,0,
        0,0,0,0,0,0,0,0,0
    };
    int* tempMap3FOR=new int[63];
    for(int i=0;i<63;i++)
    {
        tempMap3FOR[i]=MAP_FOR[i];
    }
    mapPaneData[3] = tempMap3FOR;
    //=========第4关卡的地图数据  end======================
    
    //=========第4关卡目标格子可否通过的数据  start==========
    int* tempMap4FOR=new int[63];
    int MAP_OBJECT_FOR[63]=//第一关卡的目标格子的数据
    {
        0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,
        0,1,0,0,0,0,0,2,0,
        0,0,0,0,0,0,0,0,0
    };
    for(int i=0;i<63;i++)
    {
        tempMap4FOR[i]=MAP_OBJECT_FOR[i];
    }
    mapObjectPaneData[3]=tempMap4FOR;
    //=========第4关卡目标格子可否通过的数据  end===========
    
    mapObjectPaneNum[3]=2;//第4关卡中目标格子的数量
    
    mapBestCount[3]=38;//第4关卡中目标格子的最少步数
    
    
    //=========第4关卡目标格子的位置坐标数据  start==========
    mapMIN_X[3]=-45;
    mapMIN_Z[3]=-60;
    float  *tempMAPDATA_FOR=new float[63*2];
    for(int i=0;i<mapPaneRow[3];i++)
    {
        for(int j=0;j<mapPaneCol[3];j++)
        {
            tempMAPDATA_FOR[(i*mapPaneCol[3]+j)*2+0]=mapMIN_X[3]+Constant::CubeSize*j;
            tempMAPDATA_FOR[(i*mapPaneCol[3]+j)*2+1]=mapMIN_Z[3]+Constant::CubeSize*i;
        }
    }
    mapDataXZ[3]=tempMAPDATA_FOR;
    //=========第4关卡目标格子的位置坐标数据  end============
    float *SaveCubeDataBlueFOR=new float[5];
    SaveCubeDataBlueFOR[0]=-35;
    SaveCubeDataBlueFOR[1]=-11;
    SaveCubeDataBlueFOR[2]=-50;
    SaveCubeDataBlueFOR[3]=1;
    SaveCubeDataBlueFOR[4]=1;
    
    float *SaveCubeDataGreenFOR=new float[5];
    SaveCubeDataGreenFOR[0]=25;
    SaveCubeDataGreenFOR[1]=-11;
    SaveCubeDataGreenFOR[2]=-50;
    SaveCubeDataGreenFOR[3]=7;
    SaveCubeDataGreenFOR[4]=1;
    vector<float*> vecFOR;
    vecFOR.push_back(SaveCubeDataBlueFOR);
    vecFOR.push_back(SaveCubeDataGreenFOR);
    mapCubeLocation[3]=vecFOR;//方块的初始位置
    
    
    float SaveCameraFOR[9]={-5,60,110,   0,0,0,   0,0.94f,-0.33f};
    float* tempCameraFOR=new float[9];
    for(int i=0;i<9;i++)
    {
        tempCameraFOR[i]=SaveCameraFOR[i];
    }
    mapCamera[3]=tempCameraFOR;//摄像机的初始位置
    
    
    float *tempLightFOR=new float[3];
    tempLightFOR[0]=-5;
    tempLightFOR[1]=50;
    tempLightFOR[2]=20;
    mapLight[3]=tempLightFOR;//光源的初始位置
    
    LoadedObjectVertexNormalTexture* firstLOVN=LoadUtil::loadFromFile("cube1",Constant::BLUE_CUBEID);
    LoadedObjectVertexNormalTexture* secLOVN=LoadUtil::loadFromFile("cube1",Constant::GREEN_CUBEID);
    vector<LoadedObjectVertexNormalTexture*> tempFOR;
    tempFOR.push_back(firstLOVN);
    tempFOR.push_back(secLOVN);
    mapLOVN[3]=tempFOR;
    mapTexIds[3] = LoadResourceUtil::initTexture("fourlevel", false);
    //===============FOURTH   end=========================
}


void MyData::initAllLevelsDatas()
{
    initFirstLevelDatas();
    initSecLevelDatas();
    initThiLevelDatas();
    initForLevelDatas();
}

void MyData::playSound(const string& name,const string& type)
{
    if(!effectOff){
        LoadResourceUtil::playSound(name,type);
    }
}