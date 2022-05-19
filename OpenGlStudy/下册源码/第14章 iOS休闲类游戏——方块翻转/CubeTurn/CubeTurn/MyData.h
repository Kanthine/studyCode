//
//  MyData.h
//  Sample14_4
//
//  Created by wuyafeng on 15/9/13.
//  Copyright (c) 2015年 wuyafeng. All rights reserved.
//

#ifndef __Sample14_4__MyData__
#define __Sample14_4__MyData__

#include <stdio.h>
#include <map>
#include <vector>
#include <iostream>
#include "Constant.hpp"


using namespace std;

class LoadedObjectVertexNormalTexture;
class MyData{
private:
    
public :
    map<int, int> dataMapRow;//所有关卡绘制数据的行数
    map<int, int> dataMapCol;//所有关卡绘制数据的列数
    map<int, float*> dataMapDraw;//所有关卡的绘制数据
    
    map<int, int>   mapPaneRow;//所有关卡的地图格子的行数
    map<int, int>   mapPaneCol;//所有关卡的地图格子的列数
    map<int, int*>  mapPaneData;//所有关卡的地图格子数据
    
    map<int, int*>  mapObjectPaneData;//所有关卡的目标格子数据，1表示目标格子，0表示不是目标格子
    map<int, int>   mapObjectPaneNum;//所有关卡的目标格子的数量
    map<int, int>   mapBestCount;//所有关卡中移动到目标格子的最少步数
    
    map<int, float>  mapMIN_X;//所有关卡中地图格子X坐标的最小值
    map<int, float>  mapMIN_Z;//所有关卡中地图格子Z坐标的最小值
    
    map<int, float*>  mapDataXZ;//所有关卡中地图格子的位置数据X，Z
    map<int, vector<float*> >  mapCubeLocation;//所有关卡中木块的位置
    map<int, float*>  mapCamera;//所有关卡中摄像机的位置
    map<int, float*>  mapLight;//所有关卡中光源的位置
    map<int, vector<LoadedObjectVertexNormalTexture*>> mapLOVN;
    map<int, int>  mapTexIds;//四个关卡的纹理
    
    static bool everCubeUp[2];
    static bool isCubeUp[8];
    static bool saveIsCubeUp[8];   
    static float upCubeRowAndColData[16]; //可升降方块的行列值
    static float controlCubeData[16];//机关方块的坐标值
    static int switchViewId ;//当前界面的id
    static int level;//关卡
    static bool musicOff;//音乐关闭
    static bool effectOff;//音效关闭
    
    bool isGreenScale;
    float scaleGreen;
    bool isBlueScale;
    float scaleBlue;
    bool isStop;//下一步不能移动时为停止
    
    
    MyData();
    ~MyData();
    void initAllLevelsDatas();
    void initFirstLevelDatas();
    void initSecLevelDatas();
    void initThiLevelDatas();
    void initForLevelDatas();

    static void playSound(const string& name,const string& type);
};

#endif /* defined(__Sample14_4__MyData__) */
