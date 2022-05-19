#include "HelpView.hpp"
#include "MatrixState.hpp"
#include "MatrixState2D.hpp"
#include "LoadUtil.h"
#include "LoadResourceUtil.h"
#include "Constant.hpp"

float HelpView::firstDrawDatas[14]={
    0,0,-16,-30,2,2,2,
    0,-10,-16,-30,2,2,2
};
float HelpView::secDrawDatas[42]={
    0,-15,-24,-30,2,2,2,
    0,-5,-24,-30,2,2,2,
    0,5,-24,-30,2,2,2,
    0,-15,-8,-30,2,2,2,
    0,-5,-8,-30,2,2,2,
    0,5,-8,-30,2,2,2
};
float HelpView::thirdDrawDatas[35]={
    0,-15,-16,-30,2,2,2,
    0,-5,-16,-30,2,2,2,
    0,5,-16,-30,2,2,2,
    
    8,-15,0,-45,1,1,1,
    4,-15,-15.7f,-30,1,1,1
};
float HelpView::fourDrawDatas[49]={
    0,-15,-24,-30,2,2,2,
    0,-5,-24,-30,2,2,2,
    0,5,-24,-30,2,2,2,
    3,-5,-22,-30,1,1,1,
    0,-15,-8,-30,2,2,2,
    0,-5,-8,-30,2,2,2,
    0,5,-8,-30,2,2,2
};
float HelpView::upCubeData[6] = {
    0,0,0,1,1,1
};
float HelpView::saveUpCubeData[6] = {
    0,0,0,1,1,1
};
HelpView::HelpView(){
}

void HelpView::Initialize(int width, int height)
{//初始化函数
    float ratio = (float) width/height;//计算宽高比
    MatrixState::setInitStack();   //初始化矩阵
    left=right=ratio;
    bottom=top=1;
    near=2;
    far=500;
    //设置投影矩阵
    MatrixState::setProjectFrustum(-left, right, -bottom, top, near, far);
    //设置社摄像机
    MatrixState::setCamera(-5,20,75, -5,-11,-30, 0,0.94f,-0.33f);
    //设置太阳灯光的初始位置
    MatrixState::setLightLocationSun(-5,50,20);
    initHelpViewDatas();//
    initVariables();//初始化变量
}
void HelpView::Render() {
    moveFinger();//移动手指
    doTast();
    glClearColor(1.0f, 1.0f, 1.0f, 1);//设置背景颜色
    //清除深度缓冲与颜色缓冲
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    
    if(levelHelp == Constant::LEVEL_FIRST || levelHelp ==Constant::LEVEL_THIRD)
    {
        MatrixState::setCamera(-58.0190f,38.4463f,61.8310f, -5,-11,-30, 0.2113f,0.9063f,-0.3660f);
    }else{
        MatrixState::setCamera(-5,20,75, -5,-11,-30, 0,0.94f,-0.33f);
    }
    
    switch(levelHelp){
        case Constant::LEVEL_FIRST:
        {
            int length1 = sizeof(firstDrawDatas)/sizeof(float);
            drawGameViewNotCubes(firstDrawDatas,length1,7);//绘制游戏场景中的地板
        }
        break;
        case Constant::LEVEL_SEC:
        {
            int length2 = sizeof(secDrawDatas)/sizeof(float);
            drawGameViewNotCubes(secDrawDatas,length2,7);//绘制游戏场景中的地板
        }
        break;
        case Constant::LEVEL_THIRD:
        {
            int length3 = sizeof(thirdDrawDatas)/sizeof(float);
            drawGameViewNotCubes(thirdDrawDatas,length3,7);//绘制游戏场景中的地板
        }
        break;
        case  Constant::LEVEL_FOUTH:
        {
            int length4 = sizeof(fourDrawDatas)/sizeof(float);
            drawGameViewNotCubes(fourDrawDatas,length4,7);//绘制游戏场景中的地板
        }
        break;
    }    
    for(int i=0;i<mapLOVN[levelHelp].size();i++)
    {
        drawSenceMoveCubes(mapLOVN[levelHelp][i]);//绘制场景中可移动的木块
    }
    drawBN2DObject();//绘制2D物体
}

void HelpView::OnFingerDown(float x,float y){
    if(x>(BACK_X-BACK_WIDTH/2)&&x<(BACK_X+BACK_WIDTH/2)&&y>(BACK_Y-BACK_HEIGHT/2)&&y<(BACK_Y+BACK_HEIGHT/2))
    {//返回
        isInitDatas = true;
        MyData::playSound("bt_press","mp3");//播放声音
        MyData::switchViewId= Constant::MAIN_VIEW_ID;
    }else if(x>(NEXT_X-NEXT_WIDTH/2)&&x<(NEXT_X+NEXT_WIDTH/2)&&y>(NEXT_Y-NEXT_HEIGHT/2)&&y<(NEXT_Y+NEXT_HEIGHT/2))
    {//下一关
        isInitDatas = false;
        MyData::playSound("bt_press","mp3");//播放声音
        levelHelp = (levelHelp++)==3 ? 0:levelHelp;
        initVariables();
    }
    else if(x>(RESET_X-RESET_WIDTH/2)&&x<(RESET_X+RESET_WIDTH/2)&&y>(RESET_Y-RESET_HEIGHT/2)&&y<(RESET_Y+RESET_HEIGHT/2))
    {//重放
        MyData::playSound("bt_press","mp3");//播放声音
        initVariables();
        isInitDatas = false;
    }
}

void HelpView::drawBN2DObject()
{
    glDisable(GL_DEPTH_TEST);//关闭深度检测
    sj= (sj = sj-0.01f)<0.75f ?0.75f :sj;
    MatrixState2D::pushMatrix();
    if(levelHelp==Constant::LEVEL_THIRD||levelHelp==Constant::LEVEL_FOUTH)
    {
        MatrixState2D::translate(0.2f, -0.2f, 0);
    }
    else{
        MatrixState2D::translate(-0.2f, -0.2f, 0);
    }
    finger->drawSelf(fingerMoveX,fingerMoveY,sj);
    MatrixState2D::popMatrix();
    
    back->drawSelf(BACK_X,BACK_Y,1.0f);
    next->drawSelf(NEXT_X,NEXT_Y,1.0f);
    reSet->drawSelf(RESET_X,RESET_Y,1.0f);

    switch(levelHelp){
        case Constant::LEVEL_FIRST:
            levelName->setTexId(texId1);
            break;
        case Constant::LEVEL_SEC:
            levelName->setTexId(texId2);
            break;
        case Constant::LEVEL_THIRD:
            levelName->setTexId(texId3);
            break;
        case  Constant::LEVEL_FOUTH:
            levelName->setTexId(texId4);
            break;
    }
    
    levelName->drawSelf(LEVEL_NAME_X, LEVEL_NAME_Y,1.0f);
    glEnable(GL_DEPTH_TEST);
}

void HelpView::drawGameViewNotCubes(float *currData,int length,int currCol)
{
    angleCube+=2;
    int rows1 = length/currCol;
    for(int i=0;i<rows1;i++)
    {
        if(currData[i*currCol]==0)
        {//绘制游戏场景中的地板
            MatrixState::pushMatrix();
            MatrixState::translate(currData[i*currCol+1], currData[i*currCol+2], currData[i*currCol+3]);
            MatrixState::scale(currData[i*currCol+4], currData[i*currCol+5], currData[i*currCol+6]);
            plane->drawSelf(planeTextureId);
            MatrixState::popMatrix();
        }else if(currData[i*currCol]==8)
        {//绘制游戏场景中可升降的木块
            MatrixState::pushMatrix();
            MatrixState::translate(currData[i*currCol+1], currData[i*currCol+2], currData[i*currCol+3]);
            MatrixState::translate(upCubeData[0],upCubeData[1],upCubeData[2]);
            MatrixState::scale(upCubeData[3], upCubeData[4], upCubeData[5]);
            blockCube->drawSelf(blockCubeTextureId);
            MatrixState::popMatrix();
        }else if(currData[i*currCol]==4)
        {//绘制游戏场景中控制木块升降的小薄片
            MatrixState::pushMatrix();
            MatrixState::translate(currData[i*currCol+1], currData[i*currCol+2], currData[i*currCol+3]);
            MatrixState::scale(currData[i*currCol+4], currData[i*currCol+5], currData[i*currCol+6]);
            thinCube->drawSelf(cube1TextureId);
            MatrixState::popMatrix();
        }else if(currData[i*currCol]==3)
        {//绘制游戏场景中旋转的小方块
            MatrixState::pushMatrix();
            MatrixState::translate(currData[i*currCol+1], currData[i*currCol+2], currData[i*currCol+3]);
            MatrixState::rotate(angleCube, 0,1,0);
            MatrixState::rotate(45,1,0,1);
            smallCube->drawSelf(greenCubeTextureId);
            MatrixState::popMatrix();
        }
    }
}

//绘制场景中可移动的木块
void HelpView::drawSenceMoveCubes(LoadedObjectVertexNormalTexture* tempOBJ)
{
    MatrixState::pushMatrix();//保护现场
    MatrixState::translate(tempOBJ->mCubeTransX, tempOBJ->mCubeTransY, tempOBJ->mCubeTransZ);//推远坐标系
    MatrixState::translate(tempOBJ->tempCenterX, tempOBJ->tempCenterY, tempOBJ->tempCenterZ);//平移
    MatrixState::rotate(tempOBJ->angleX, 1, 0, 0);//X轴旋转扰动
    MatrixState::rotate(tempOBJ->angleY, 0, 1, 0);//Y轴旋转扰动
    MatrixState::rotate(tempOBJ->angleZ, 0, 0, 1);//Z轴旋转扰动
    if(tempOBJ->flag==Constant::BLUE_CUBEID){//蓝色木块
        tempOBJ->drawSelf(cube1TextureId);
    }else if(tempOBJ->flag==Constant::GREEN_CUBEID){//绿色木块
        tempOBJ->drawSelf(greenCubeTextureId);
    }else if(tempOBJ->flag==Constant::RED_CUBEID){//蓝红色木块
       tempOBJ->drawSelf(redCubeTextureId);
    }
    MatrixState::popMatrix();
}

void HelpView::moveFinger()
{//移动手指
    if(levelHelp==Constant::LEVEL_FIRST||levelHelp==Constant::LEVEL_SEC)
    {//右移动手指
        fingerMoveX = ((fingerMoveX+=0.01f)<0.45f) ? fingerMoveX : 0.45f;
        if(fingerMoveX == 0.45f&&fingerCount==0)
        {
            fingerMoveX = 0.2f;
            fingerCount ++;
        }
    }
    else{//左移动手指
        fingerMoveX = ((fingerMoveX-=0.01f)<(-0.35f)) ? (-0.35f) :fingerMoveX ;
        if(fingerMoveX == (-0.35f)&&fingerCount==0)
        {
            fingerMoveX = -0.2f;
            fingerCount ++;
        }
    }
    if(fingerCount==1)
    {//进行第二次动画
        sj= 1;
        anmiFlag = true;
        fingerCount = -1;
    }
    
}

void HelpView::doTast()
{
    if(!anmiFlag){//无须移动木块
        return;
    }
    for(int i=0;i<mapLOVN[levelHelp].size();i++){
        mapLOVN[levelHelp][i]->isTouch = true;
        if(mapLOVN[levelHelp][i]->flag==Constant::RED_CUBEID)
        {
            mapLOVN[levelHelp][i]->isTwoSteps = true;
        }
        int state=-1;
        if(levelHelp==Constant::LEVEL_THIRD||levelHelp==Constant::LEVEL_FOUTH){
            if(mapLOVN[levelHelp][i]->flag==Constant::GREEN_CUBEID)
            {
                state = Constant::RIGHT_KEY;
            }else{
                state = Constant::LEFT_KEY;
            }
        }else{
            state = Constant::RIGHT_KEY;
        }
        mapLOVN[levelHelp][i]->keyChange(state, mapLOVN[levelHelp][i]->isTouch);
    }
    countDrawTastHelp++;
    if(countDrawTastHelp%20==0){
        for(int i=0;i<mapLOVN[levelHelp].size();i++){//旋转结束恢复扰动变量
            
            if(levelHelp == Constant::LEVEL_THIRD)
            {
                upCubeData[1] = -5;
                upCubeData[4] = 0.05f;
            }
            
            mapLOVN[levelHelp][i]->mCubeTransX=mapObject[levelHelp][2*i+0];
            mapLOVN[levelHelp][i]->mCubeTransZ=mapObject[levelHelp][2*i+1];
            mapLOVN[levelHelp][i]->reCoverHelpView();
            if(levelHelp==3&&mapLOVN[levelHelp][i]->flag==Constant::BLUE_CUBEID)
            {//蓝色变绿色
                mapLOVN[levelHelp][i]->isChangeColor = true;
                mapLOVN[levelHelp][i]->changeType(Constant::GREEN_CUBEID);
            }
        }
        MyData::playSound("buttonclick","mp3");//移动木块，播放声音
        anmiFlag=false;
        countDrawTastHelp=0;
    }
}
//帮助界面数据
void HelpView::initHelpViewDatas()
{
    //加载obj
    plane=LoadUtil::loadFromFile("plane",Constant::CUBEID);
    blockCube=LoadUtil::loadFromFile("cube3",Constant::BLOCK_CUBEID);
    thinCube=LoadUtil::loadFromFile("cube4",Constant::THIN_CUBEID);
    smallCube=LoadUtil::loadFromFile("cube5",Constant::SMALL_CUBEID);
    //初始化纹理id
    planeTextureId = LoadResourceUtil::initTexture("whiteplane", false);
    cube1TextureId=LoadResourceUtil::initTexture("blue",false);//可移动木块纹理id
    redCubeTextureId=LoadResourceUtil::initTexture("red",false);
    greenCubeTextureId=LoadResourceUtil::initTexture("green",false);
    blockCubeTextureId = LoadResourceUtil::initTexture("cubeblue",true);
    fingerTexureId = LoadResourceUtil::initTexture("finger",false);
    texId1 = LoadResourceUtil::initTexture("onelevelblack",false);
    texId2 = LoadResourceUtil::initTexture("twolevelblack",false);
    texId3 = LoadResourceUtil::initTexture("threelevelblack",false);
    texId4 = LoadResourceUtil::initTexture("fourlevelblack",false);
    finger = new BN2DObject(false,0,fingerTexureId,FINGER_WIDTH,FINGER_HEIGHT);//手指绘制者
    back = new BN2DObject(false,0,LoadResourceUtil::initTexture("back1",false),BACK_WIDTH,BACK_HEIGHT);//返回菜单绘制者
    next = new BN2DObject(false,0,LoadResourceUtil::initTexture("next",false),NEXT_WIDTH,NEXT_HEIGHT);//下一关绘制者
    levelName = new BN2DObject(false,0,texId1,LEVEL_NAME_WIDTH,LEVEL_NAME_HEIGHT);//关卡名称绘制者
    reSet = new BN2DObject(false,0,LoadResourceUtil::initTexture("reset",false),RESET_WIDTH,RESET_HEIGHT);//重放
    
    initHelpViewFirst();//第1关的绘制数据
    initHelpViewSecond();//第2关的绘制数据
    initHelpViewThird();//第3关的绘制数据
    initHelpViewFourth();//第4关的绘制数据
}

void HelpView::initHelpViewFirst()
{//初始化第1关的绘制数据
    LoadedObjectVertexNormalTexture* blueCube=LoadUtil::loadFromFile("cube1",Constant::BLUE_CUBEID);
    //第1关  蓝色方块
    float* tempCubeFirst=new float[5];
    tempCubeFirst[0] = -10;
    tempCubeFirst[1] = -11;
    tempCubeFirst[2] = -30;
    tempCubeFirst[3] = -1;
    tempCubeFirst[4] = -1;
    blueCube->initCubes(tempCubeFirst);
    vector<LoadedObjectVertexNormalTexture*> firstOBJS ;
    firstOBJS.push_back(blueCube);
    mapLOVN[0] = firstOBJS;
    
    float* tempRecoverFirst = new float[2];
    tempRecoverFirst[0] = 0;
    tempRecoverFirst[1] = -30;
    mapObject[0] = tempRecoverFirst;
    
    float* tempInitFirst = new float[2];
    tempInitFirst[0] = -10;
    tempInitFirst[1] = -30;
    mapOrign[0] = tempInitFirst;

}
void HelpView::initHelpViewSecond()
{//初始化第2关的绘制数据
    LoadedObjectVertexNormalTexture* redCube=LoadUtil::loadFromFile("cube1",Constant::RED_CUBEID);
    LoadedObjectVertexNormalTexture* blueCubeSec=LoadUtil::loadFromFile("cube1",Constant::BLUE_CUBEID);
    //第2关  蓝色和红色方块
    //方块位置
    float* tempBlueCubeSec=new float[5];
    tempBlueCubeSec[0] = -15;
    tempBlueCubeSec[1] = -3;
    tempBlueCubeSec[2] = -30;
    tempBlueCubeSec[3] = -1;
    tempBlueCubeSec[4] = -1;
    float* tempRedCubeSec=new float[5];
    tempRedCubeSec[0] = -15;
    tempRedCubeSec[1] = -19;
    tempRedCubeSec[2] = -30;
    tempRedCubeSec[3] = -1;
    tempRedCubeSec[4] = -1;
    blueCubeSec->initCubes(tempBlueCubeSec);
    redCube->initCubes(tempRedCubeSec);
    vector<LoadedObjectVertexNormalTexture*> secOBJS ;
    secOBJS.push_back(blueCubeSec);
    secOBJS.push_back(redCube);
    mapLOVN[1] = secOBJS;
    float* tempRecoverSec = new float[4];
    tempRecoverSec[0] = -5;
    tempRecoverSec[1] = -30;
    tempRecoverSec[2] = 5;
    tempRecoverSec[3] = -30;
    mapObject[1] = tempRecoverSec;
    float* tempInitSec = new float[4];
    tempInitSec[0] = -15;
    tempInitSec[1] = -30;
    tempInitSec[2] = -15;
    tempInitSec[3] = -30;
    mapOrign[1] = tempInitSec;
}
void HelpView::initHelpViewThird()
{//初始化第3关的绘制数据
    LoadedObjectVertexNormalTexture* blueCubeThi=LoadUtil::loadFromFile("cube1",Constant::BLUE_CUBEID);
    //第3关  蓝色方块
    float* tempBlueCubeThi=new float[5];
    tempBlueCubeThi[0] = -5;
    tempBlueCubeThi[1] = -11;
    tempBlueCubeThi[2] = -30;
    tempBlueCubeThi[3] = -1;
    tempBlueCubeThi[4] = -1;
    blueCubeThi->initCubes(tempBlueCubeThi);
    vector<LoadedObjectVertexNormalTexture*> thirdOBJS ;
    thirdOBJS.push_back(blueCubeThi);
    mapLOVN[2] = thirdOBJS;
    float* tempRecoverThi = new float[2];
    tempRecoverThi[0] = -15;
    tempRecoverThi[1] = -30;
    mapObject[2] = tempRecoverThi;
    
    float* tempInitThi = new float[2];
    tempInitThi[0] = -5;
    tempInitThi[1] = -30;
    mapOrign[2] = tempInitThi;
}
void HelpView::initHelpViewFourth()
{//初始化第4关的绘制数据
    LoadedObjectVertexNormalTexture* greenCube=LoadUtil::loadFromFile("cube1",Constant::GREEN_CUBEID);
    LoadedObjectVertexNormalTexture* blueCubeFour=LoadUtil::loadFromFile("cube1",Constant::BLUE_CUBEID);
    //第4关  蓝色和绿色方块
    float* tempBlueCubeFour=new float[5];
    tempBlueCubeFour[0] = 5;
    tempBlueCubeFour[1] = -19;
    tempBlueCubeFour[2] = -30;
    tempBlueCubeFour[3] = -1;
    tempBlueCubeFour[4] = -1;
    float* tempGreenCubeFour=new float[5];
    tempGreenCubeFour[0] = -5;
    tempGreenCubeFour[1] = -3;
    tempGreenCubeFour[2] = -30;
    tempGreenCubeFour[3] = -1;
    tempGreenCubeFour[4] = -1;
    blueCubeFour->initCubes(tempBlueCubeFour);
    greenCube->initCubes(tempGreenCubeFour);
    vector<LoadedObjectVertexNormalTexture*> fourOBJS ;
    fourOBJS.push_back(blueCubeFour);
    fourOBJS.push_back(greenCube);
    mapLOVN[3] = fourOBJS;
    float* tempRecoverFou = new float[4];
    tempRecoverFou[0] = -5;
    tempRecoverFou[1] = -30;
    tempRecoverFou[2] = 5;
    tempRecoverFou[3] = -30;
    mapObject[3] = tempRecoverFou;
    
    float* tempInitFou = new float[4];
    tempInitFou[0] = 5;
    tempInitFou[1] = -30;
    tempInitFou[2] = -5;
    tempInitFou[3] = -30;
    mapOrign[3] = tempInitFou;
}

void HelpView::initVariables()
{
    isInitDatas = false;
    anmiFlag = false;
    fingerMoveX = 0;
    fingerMoveY = 0;
    countDrawTastHelp=0;//任务执行方法的计数器
    angleCube = 0;
    fingerCount = 0;
    angleCube = 0;//小方块的旋转角度
    sj=1;
    
    upCubeData[1] = 0;
    upCubeData[4] = 1;

    for(int i=0;i<mapLOVN[levelHelp].size();i++){//旋转结束恢复扰动变量
        mapLOVN[levelHelp][i]->mCubeTransX=mapOrign[levelHelp][2*i+0];
        mapLOVN[levelHelp][i]->mCubeTransZ=mapOrign[levelHelp][2*i+1];
        mapLOVN[levelHelp][i]->reCoverHelpView();
        if(mapLOVN[levelHelp][i]->isChangeColor )
        {
            mapLOVN[levelHelp][i]->changeType(Constant::BLUE_CUBEID);
            mapLOVN[levelHelp][i]->isChangeColor = false;
        }
    }

}
HelpView::~HelpView(){}
