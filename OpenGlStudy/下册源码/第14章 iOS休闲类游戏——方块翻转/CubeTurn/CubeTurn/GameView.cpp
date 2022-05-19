#include "GameView.hpp"
#include <math.h>
#include "Constant.hpp"
#include "Matrix.hpp"
#include "LoadedObjectVertexNormalTexture.h"
#include "LoadResourceUtil.h"
#include "MatrixState.hpp"
#include "MatrixState2D.hpp"
#include "LoadUtil.h"
#include "IntersectantUtil.h"


float GameView::upCubeMoveData[48] = {
    0,0,0,1,1,1,0,0,0,1,1,1,
    0,0,0,1,1,1,0,0,0,1,1,1,
    0,0,0,1,1,1,0,0,0,1,1,1,
    0,0,0,1,1,1,0,0,0,1,1,1
};

float GameView::saveUpCubeMoveData[48] = {
    0,0,0,1,1,1,0,0,0,1,1,1,
    0,0,0,1,1,1,0,0,0,1,1,1,
    0,0,0,1,1,1,0,0,0,1,1,1,
    0,0,0,1,1,1,0,0,0,1,1,1
};
int GameView::stepCount[2] = {0,0};
GameView::GameView(){
}
void GameView::Initialize(int width, int height)
{//初始化函数
    float ratio = (float) width/height;//计算宽高比
    MatrixState::setInitStack();   //初始化矩阵
    left=right=ratio;
    bottom=top=1;
    near=2;
    far=500;
    //设置投影矩阵
    MatrixState::setProjectFrustum(-left, right, -bottom, top, near, far);
    
    initObjectData();
    initSceneData(MyData::level);
    
    mPauseView = new PauseView();
    mPauseView -> Initialize(width, height);
    
}

void GameView::Render()
{
    doTast();//任务方法
    glClearColor(1.0f, 1.0f, 1.0f, 1);//设置背景颜色
    //清除深度缓冲与颜色缓冲
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //设置摄像机参数
    MatrixState::setCamera(cameraX,cameraY,cameraZ,targetX-5,targetY-11,targetZ-30,upX,upY,upZ);
    //设置灯光参数
    MatrixState::setLightLocationSun(lightX,lightY,lightZ);
    drawSenceMoveCubes();//绘制可移动的木块
    drawNotMoveObjects();//绘制不可移动的物体
    DrawCubeDownOrUp(0);//绘制当前可升降方块的状态
    DrawCubeDownOrUp(6);

    //绘制移动步数
    MatrixState2D::pushMatrix();
    MatrixState2D::translate(0, 0.75f, 0);
    sn->drawSelf(currBestCount,moveStep,Constant::ICON_WIDTH);
    MatrixState2D::popMatrix();
    
    mPauseView->Render(currLevelTexId,sn,currBestCount,moveStep);

}

void GameView::OnFingerDown(float x,float y,float locationx,float locationy,int width,int height){
    if(mPauseView->isDrawInfo)
    {
        mPauseView->OnFingerDown(x, y);
        if(mPauseView->isInitData)
        {
            mPauseView->initDatas(false, false);
            initSceneData(MyData::level);
        }
    }else{
        if(isBanTouch){//若游戏结束，则不能移动木块
            return;
        }
        float* AB=IntersectantUtil::calculateABPosition
        (
         locationx,//当前触控点的x坐标
         locationy, //当前触控点的y坐标
         width, //屏幕的宽度
         height, //屏幕的高度
         left, //视口左值
         top,
         near, //近平面
         far
         );
        Vector3f* start = new Vector3f(AB[0], AB[1], AB[2]);//起点
        Vector3f* end = new Vector3f(AB[3], AB[4], AB[5]);//终点
        Vector3f* dir = end->vMinus(start);
        float minTime=1;
        for(int i=0;i<vectorLOVN.size();i++)
        {
            AABB3* box=vectorLOVN[i]->getCurrBox();
            float t = box->rayIntersect(start, dir);//计算相交时间
            if (t <= minTime){
                minTime = t;//记录最小值
                touchX=locationx;
                touchY=locationy;
                isCubeTouch=true;
            }
        }
        delete AB;
        delete start;
        delete end;
        delete dir;
    }
}

void GameView::OnFingerUp(float locationx,float locationy){
    if(mPauseView->isDrawInfo){return;}
    if(isScaleScene){isScaleScene=false;}
    if(anmiFlag||!isCubeTouch||isFinish||isScaleScene||isBanTouch)
    {//若在动画播放过程中，按键不起作用
        return;
    }
    float disX=locationx-touchX;
    float disY=locationy-touchY;
    if((abs(disX)<Constant::touch_span&&abs(disY)<Constant::touch_span))
    {
        return;
    }
    anmiFlag=true;//设置动画播放中标志
    updateEvery(CAMERA_DIRECTION,disX,disY,isBack);
    isCubeTouch=false;
}

void GameView::OnFingerMove(float previousx,float previousy,float currentx,float currenty)
{
    if(isCubeTouch||isFinish||isScaleScene||mPauseView->isDrawInfo){
        return;
    }
    float dy = (currenty - previousy);//计算触控笔Y位移
    float dx = (currentx - previousx);//计算触控笔X位移
    calCamera(dx,dy);
}

void GameView::OnFingerPinched(float scale)
{
    if(mPauseView->isDrawInfo){return;}
    isScaleScene=true;
    scale = (scale>1.5f) ? 1.5f : scale;
    scale = (scale<0.5f) ? 0.5f : scale;
    currScale = scale;//当前扩大或者缩小值
    calCamera(0, 0);//重新计算摄像机参数
}

void GameView::OnOneFingerTwoTaps()
{
    if(mPauseView->isDrawInfo){return;}
    mPauseView->isDrawInfo=true;
}

//=========修改后=====================
void GameView::OnTwoFingersTwoTaps()
{
    if(isBanTouch){return;}//若游戏结束，则不能后退方块
    if(moveStep>0){
        moveStep--;
        isBack=true;
        this->md->isBlueScale=false;
        this->md->isGreenScale=false;
    }
    for(int i=0;i<vectorLOVN.size();i++)
    {
        vectorLOVN[i]->back(MyData::level);
    }
}
//绘制场景中不可移动的物体
void GameView::drawNotMoveObjects()
{
    angleCube+=2;
    
    if(!isBack){
        this->md->scaleBlue += this->md->isBlueScale? 0.05f : -0.05f;
        this->md->scaleBlue = this->md->scaleBlue<1 ? 1:this->md->scaleBlue;//最大值为1
        this->md->scaleBlue=min(2.0f, this->md->scaleBlue);
        
        this->md->scaleGreen += this->md->isGreenScale? 0.05f : -0.05f;
        this->md->scaleGreen = this->md->scaleGreen<1 ? 1:this->md->scaleGreen;//最大值为1
        this->md->scaleGreen=min(2.0f,this->md->scaleGreen);
    }else{
        this->md->scaleGreen=1;
        this->md->scaleBlue=1;
    }
    for(int i=0;i<currRow;i++)
    {
        float switchId=currData[i*currCol+0];
        switch ((int)switchId) {
            case Constant::CUBEID://绘制平面
                DrawGameViewPlane(i);
                break;
            case Constant::FRAMEID://绘制圆柱
                DrawGameViewFrame(i);
                break;
            case Constant::CORNERID://绘制黑边角
                DrawGameViewCorner(i);
                break;
            case Constant::SMALL_CUBEID://旋转的小立方体
                DrawGameViewSmallCube(i);
                break;
            case Constant::THIN_CUBEID://机关方块
                DrawGameViewThinCube(i);
                break;
            case Constant::BLOCK_CUBEID://可升降方块
                DrawGameViewBlockCube(i);
                break;
        }
    }
}
void GameView::DrawGameViewPlane(int i)//绘制游戏场景中的地板
{
    MatrixState::pushMatrix();
    MatrixState::translate(currData[i*currCol+1], currData[i*currCol+2], currData[i*currCol+3]);
    MatrixState::scale(currData[i*currCol+4], currData[i*currCol+5], currData[i*currCol+6]);
    if(currData[i*currCol+11]==0)
    {
        plane->drawSelf(planeTextureId);
    }else if(currData[i*currCol+11]==3)
    {
        plane->drawSelf(greenCubeTextureId);
    }
    else if(currData[i*currCol+11]==2)
    {
        plane->drawSelf(redCubeTextureId);
    }else if(currData[i*currCol+11]==1)
    {
        plane->drawSelf(cube1TextureId);
    }
    MatrixState::popMatrix();
}
void GameView::DrawGameViewFrame(int i)//绘制游戏场景中的边框
{
    MatrixState::pushMatrix();
    MatrixState::translate(currData[i*currCol+1], currData[i*currCol+2], currData[i*currCol+3]);
    MatrixState::scale(currData[i*currCol+4], currData[i*currCol+5], currData[i*currCol+6]);
    MatrixState::rotate(currData[i*currCol+7], currData[i*currCol+8], currData[i*currCol+9],currData[i*currCol+10]);
    cylinder->drawSelf(cylinderTextureId);
    MatrixState::popMatrix();
}
void GameView::DrawGameViewCorner(int i)//绘制游戏场景中的黑边角
{
    MatrixState::pushMatrix();
    MatrixState::translate(currData[i*currCol+1], currData[i*currCol+2], currData[i*currCol+3]);
    MatrixState::scale(currData[i*currCol+4], currData[i*currCol+5], currData[i*currCol+6]);
    cube2->drawSelf(cube2TextureId);
    MatrixState::popMatrix();
}
void GameView::DrawGameViewSmallCube(int i)//绘制游戏场景中的旋转立方体
{
    MatrixState::pushMatrix();
    if(currData[i*currCol+11]==3)
    {
        MatrixState::translate(currData[i*currCol+1], currData[i*currCol+2]+this->md->scaleGreen, currData[i*currCol+3]);
        MatrixState::scale(this->md->scaleGreen,this->md->scaleGreen,this->md->scaleGreen);
        MatrixState::rotate(angleCube, 0,1,0);
        MatrixState::rotate(currData[i*currCol+7], currData[i*currCol+8], currData[i*currCol+9],currData[i*currCol+10]);
        cube5->drawSelf(greenCubeTextureId);
    }else if(currData[i*currCol+11]==1)
    {
        MatrixState::translate(currData[i*currCol+1], currData[i*currCol+2]+this->md->scaleBlue, currData[i*currCol+3]);
        MatrixState::scale(this->md->scaleBlue,this->md->scaleBlue,this->md->scaleBlue);
        MatrixState::rotate(angleCube, 0,1,0);
        MatrixState::rotate(currData[i*currCol+7], currData[i*currCol+8], currData[i*currCol+9],currData[i*currCol+10]);
        cube5->drawSelf(cube1TextureId);
    }
    MatrixState::popMatrix();

}

void GameView::DrawGameViewThinCube(int i)//绘制游戏场景中的机关方块
{
    MatrixState::pushMatrix();
    MatrixState::translate(currData[i*currCol+1], currData[i*currCol+2], currData[i*currCol+3]);
    MatrixState::scale(currData[i*currCol+4], currData[i*currCol+5], currData[i*currCol+6]);
    if(currData[i*currCol+11]==3)
    {
        cube4->drawSelf(greenCubeTextureId);
    }else if(currData[i*currCol+11]==1)
    {
        cube4->drawSelf(cube1TextureId);
    }
    MatrixState::popMatrix();
}

void GameView::DrawGameViewBlockCube(int i)//绘制游戏场景中的可升降方块
{
    MatrixState::pushMatrix();
    MatrixState::translate(currData[i*currCol+1], currData[i*currCol+2], currData[i*currCol+3]);
    if(i==35)
    {
        MatrixState::translate(upCubeMoveData[MyData::level*12+0],
                               upCubeMoveData[MyData::level*12+1], upCubeMoveData[MyData::level*12+2]);
        MatrixState::scale(upCubeMoveData[MyData::level*12+3],
                           upCubeMoveData[MyData::level*12+4],upCubeMoveData[MyData::level*12+5]);
    }else if(i==36)
    {
        MatrixState::translate(upCubeMoveData[MyData::level*12+6],
                               upCubeMoveData[MyData::level*12+7],upCubeMoveData[MyData::level*12+8]);
        MatrixState::scale(upCubeMoveData[MyData::level*12+9],
                           upCubeMoveData[MyData::level*12+10], upCubeMoveData[MyData::level*12+11]);
    }
    if(currData[i*currCol+11]==3)
    {
        cube3->drawSelf(greenBlockTextureId);
    }else if(currData[i*currCol+11]==1)
    {
        cube3->drawSelf(blueBlockTextureId);
    }
    MatrixState::popMatrix();
}

//绘制场景中可移动的木块
void GameView::drawSenceMoveCubes()
{
    for(int i=0;i<vectorLOVN.size();i++)
    {
        MatrixState::pushMatrix();//保护现场
        MatrixState::translate(vectorLOVN[i]->mCubeTransX, vectorLOVN[i]->mCubeTransY, vectorLOVN[i]->mCubeTransZ);//推远坐标系
        MatrixState::translate(vectorLOVN[i]->tempCenterX, vectorLOVN[i]->tempCenterY, vectorLOVN[i]->tempCenterZ);//平移
        MatrixState::rotate(vectorLOVN[i]->angleX, 1, 0, 0);//X轴旋转扰动
        MatrixState::rotate(vectorLOVN[i]->angleY, 0, 1, 0);//Y轴旋转扰动
        MatrixState::rotate(vectorLOVN[i]->angleZ, 0, 0, 1);//Z轴旋转扰动
        if(vectorLOVN[i]->flag==Constant::BLUE_CUBEID){
            vectorLOVN[i]->drawSelf(cube1TextureId);
        }else if(vectorLOVN[i]->flag==Constant::GREEN_CUBEID){
            vectorLOVN[i]->drawSelf(greenCubeTextureId);
        }else if(vectorLOVN[i]->flag==Constant::RED_CUBEID){
            vectorLOVN[i]->drawSelf(redCubeTextureId);
        }
        
        vectorLOVN[i]->drawSelf(cube1TextureId);//绘制可移动木块
        MatrixState::popMatrix(); //恢复现场
    }
}


void GameView::DrawCubeDownOrUp(int index)//绘制当前可升降方块的状态
{
    if(!this->md->isCubeUp[MyData::level*2+index/6]){//如果需要降下来
        if(stepCount[index/6]<10){//缩小并且下降
            upCubeMoveData[MyData::level*12+index+1]+=Constant::upCubeTransStep;
            upCubeMoveData[MyData::level*12+index+4]+=Constant::upCubeScaleStep;
            stepCount[index/6]++;
        }
    }else{//如果需要升上去
        if(stepCount[index/6]>0){//扩大并且上升
            upCubeMoveData[MyData::level*12+index+1]-=Constant::upCubeTransStep;
            upCubeMoveData[MyData::level*12+index+4]-=Constant::upCubeScaleStep;
            stepCount[index/6]--;
        }
    }
}


void GameView::calCamera(float dx,float dy)
{
    direction += dx * Constant::TOUCH_SCALE_FACTOR;
    angleCamera+=dy * Constant::TOUCH_SCALE_FACTOR;
    direction = (int)(direction+360)%360;//注意这里是整型 可能有问题
    
    if(direction>=135&&direction<=225){
        CAMERA_DIRECTION=0;
    }else if(direction>225&&direction<=315){
        CAMERA_DIRECTION=1;
    }else if((direction>=315&&direction<=360)||(direction>=0&&direction<=45)){
        CAMERA_DIRECTION=3;
    }else if(direction>45&&direction<135){
        CAMERA_DIRECTION=2;
    }
    
    if(angleCamera<=15){
        angleCamera=15;
    }else if(angleCamera>=70){
        angleCamera=70;
    }
    
    double angleHD=Constant::toRadians(direction);
    
    float* cup= new float[4];
    cup[0] = -(float)sin(angleHD);
    cup[1] = 0;
    cup[2] = (float)cos(angleHD);
    cup[3] = 1;
    
    float* cLocation= new float[4];
    cLocation[0] = 0;
    cLocation[1] = distance/currScale;
    cLocation[2] = 0;
    cLocation[3] = 1;
    
    float* zhou = new float[3];
    zhou[0] = -cup[2];
    zhou[1] = 0;
    zhou[2] = cup[0];
    
    float* helpM=new float[16];
    Matrix::setIdentityM(helpM, 0);
    
    Matrix::rotateM(helpM, 0, angleCamera, zhou[0],zhou[1],zhou[2]);
    helpM[1]=-helpM[1];
    helpM[4]=-helpM[4];
    helpM[6]=-helpM[6];
    helpM[9]=-helpM[9];
    float* cupr=new float[4];
    float* cLocationr=new float[4];
    Matrix::multiplyMV(cupr, 0, helpM, 0, cup, 0);
    Matrix::multiplyMV(cLocationr, 0, helpM, 0, cLocation, 0);
    
    cameraX=cLocationr[0]-5;
    cameraY=cLocationr[1]-11;
    cameraZ=cLocationr[2]-30;
    
    upX=cupr[0];upY=cupr[1];upZ=cupr[2];
    
    
    lightX=cameraX;
    lightY=cameraY+100;
    lightZ=cameraZ;
    
    delete cup;
    delete cLocation;
    delete zhou;
    delete helpM;
    delete cupr;
    delete cLocationr;
}

void GameView::doTast()
{
    if(anmiFlag){
        for(int i=0;i<vectorLOVN.size();i++){
            vectorLOVN[i]->keyChange(vectorLOVN[i]->keyState, vectorLOVN[i]->isTouch);
        }
        countDrawTast++;
        if(countDrawTast%20==0){
            for(int i=0;i<vectorLOVN.size();i++){//旋转结束恢复扰动变量
                if(vectorLOVN[i]->isMove){
                    MyData::playSound("buttonclick","mp3");//移动木块，播放声音
                }
                vectorLOVN[i]->reCoverVariables(Constant::GAME_VIEW_ID,MyData::level,isBack);
            }
            anmiFlag=false;
            countDrawTast=0;
        }
    }
    
    if(isFinish){
        waitCount++;
        if(waitCount%80==0)
        {
            mPauseView->isDrawInfo=true;
            waitCount = 0;
            isFinish = false;
            isBanTouch = true;
        }
    
    }
}

void GameView::updateEvery(int status,float disX,float disY,bool isBack)
{
    if(vectorLOVN.size()==1){//一个目标点
        vectorLOVN[0]->updateKeyStatus(status,disX,disY,-1,-1,isBack);
        if(vectorLOVN[0]->flag==Constant::BLUE_CUBEID){
            isFinish=judgeFinish(vectorLOVN[0]->currCol,vectorLOVN[0]->currRow,1);
        }else if(vectorLOVN[0]->flag==Constant::RED_CUBEID){
            isFinish=judgeFinish(vectorLOVN[0]->currCol,vectorLOVN[0]->currRow,2);
        }
        if(vectorLOVN[0]->isMove){
            moveStep = vectorLOVN[0]->count;
        }
    }else{//两个目标点
        vectorLOVN[0]->updateKeyStatus(status,disX,disY,vectorLOVN[1]->currCol,vectorLOVN[1]->currRow,isBack);
        if(this->md->isStop){
            
            vectorLOVN[1]->updateKeyStatus(status,disX,disY,vectorLOVN[0]->currCol,vectorLOVN[0]->currRow,isBack);
            vectorLOVN[0]->updateKeyStatus(status,disX,disY,vectorLOVN[1]->currCol,vectorLOVN[1]->currRow,isBack);
        }else{
            
            vectorLOVN[1]->updateKeyStatus(status,disX,disY,vectorLOVN[0]->currCol,vectorLOVN[0]->currRow,isBack);
        }
        if(vectorLOVN[0]->isMove||vectorLOVN[1]->isMove)
        {
            moveStep++;
        }
        this->md->everCubeUp[0] = false;
        this->md->everCubeUp[1] = false;
        bool blueFlag=judgeFinish(vectorLOVN[0]->currCol,vectorLOVN[0]->currRow,1);
        bool greenFlag=judgeFinish(vectorLOVN[1]->currCol,vectorLOVN[1]->currRow,2);
        isFinish=blueFlag&&greenFlag;
    }
    isBack=false;
}

//判断木块是否移动到指定位置
bool  GameView::judgeFinish(int tempCol,int tempRow,int object_value)
{
    bool flag=false;
    //判断当前行列的格子是否是预先指定好的格子
    if(this->currMapObjectPane[tempRow*this->currMapPaneCol+tempCol]==object_value)
    {
        flag=true;
    }
    return flag;
}
void GameView::initObjectData()
{
    cube1TextureId=LoadResourceUtil::initTexture("blue",false);//可移动木块纹理id
    cube2TextureId=LoadResourceUtil::initTexture("black",false);//不可移动木块纹理id
    cylinderTextureId=LoadResourceUtil::initTexture("white",false);//圆柱纹理id
    planeTextureId=LoadResourceUtil::initTexture("whiteplane",false);//地板纹理id
    numberBlueTexId=LoadResourceUtil::initTexture("bluenumber",false);
    numberRedTexId=LoadResourceUtil::initTexture("rednumber",false);
    redCubeTextureId=LoadResourceUtil::initTexture("red",false);
    greenCubeTextureId=LoadResourceUtil::initTexture("green",false);
    blueBlockTextureId=LoadResourceUtil::initTexture("cubeblue",true);
    greenBlockTextureId=LoadResourceUtil::initTexture("cubegreen",true);
    
    cube2=LoadUtil::loadFromFile("cube2",Constant::CORNERID);
    cylinder=LoadUtil::loadFromFile("cylinder",Constant::FRAMEID);
    plane=LoadUtil::loadFromFile("plane",Constant::CUBEID);
    cube3=LoadUtil::loadFromFile("cube3",Constant::BLOCK_CUBEID);
    cube4=LoadUtil::loadFromFile("cube4",Constant::THIN_CUBEID);
    cube5=LoadUtil::loadFromFile("cube5",Constant::SMALL_CUBEID);
    
    this->md = new MyData();
    this->md->initAllLevelsDatas();
    sn=new StepNumber(numberBlueTexId,numberRedTexId,Constant::ICON_WIDTH,Constant::ICON_HEIGHT);
    
}

void GameView::initSceneData(int level)
{
    this->currLevelTexId=this->md->mapTexIds[level];
    this->currMAP_PANE=this->md->mapPaneData[level];
    this->currMapObjectPane=this->md->mapObjectPaneData[level];//获取地图中目标格子的数据
    this->currMAP_DATA=this->md->mapDataXZ[level];//获取地图中每个格子的坐标数据
    this->currMapPaneRow=this->md->mapPaneRow[level];//获取地图格子的行数
    this->currMapPaneCol=this->md->mapPaneCol[level];//获取地图格子的列数
    this->cubesData=this->md->mapCubeLocation[level];//初始方块的位置和行列值
    this->currBestCount=this->md->mapBestCount[level];//最短步数
    this->currRow = this->md->dataMapRow[level];//获取当前关卡数据的行数
    this->currCol = this->md->dataMapCol[level];//获取当前关卡数据的列数
    this->currData = this->md->dataMapDraw[level];//获取当前关卡的绘制数据
    this->vectorLOVN = this->md->mapLOVN[level];
    
    for(int i=0;i<this->vectorLOVN.size();i++){
        this->vectorLOVN[i]->setInitCubeLocation(this->md,this->cubesData[i],this->currMapPaneCol,
                                                 this->currMapPaneRow,this->currMAP_DATA, this->currMAP_PANE);
        this->vectorLOVN[i]->initVariables(cubesData[i]);
    }
    
    SaveCamera=md->mapCamera[level];
    cameraX = SaveCamera[0];
    cameraY = SaveCamera[1];
    cameraZ = SaveCamera[2];
    targetX = SaveCamera[3];
    targetY = SaveCamera[4];
    targetZ = SaveCamera[5];
    upX = SaveCamera[6];
    upY = SaveCamera[7];
    upZ = SaveCamera[8];
    
    direction = 180;//摄像机方向角
    angleCamera = 63;//摄像机仰角
    distance = 158;//摄像机与目标点距离
    
    SaveLight=md->mapLight[level];
    lightX=SaveLight[0];
    lightY=SaveLight[1];
    lightZ=SaveLight[2];
    
    anmiFlag=false;
    isCubeTouch=false;
    isScaleScene=false;
    isBack=false;
    isFinish=false;
    isBanTouch = false;
    countDrawTast=0;
    touchX=0;
    touchY=0;
    moveStep=0;
    CAMERA_DIRECTION=0;
    angleCube=0;
    waitCount = 0;
    this->md->isBlueScale=false;
    this->md->isGreenScale=false;
    this->md->scaleBlue=1;
    this->md->scaleGreen=1;
    for(int i=0;i<48;i++){
        upCubeMoveData[i]=saveUpCubeMoveData[i];
    }
    for(int i=0;i<8;i++){
        MyData::isCubeUp[i] = MyData::saveIsCubeUp[i];
    }
    for(int i=0;i<2;i++){
        stepCount[i]=0;
    }
}
GameView::~GameView(){
}



