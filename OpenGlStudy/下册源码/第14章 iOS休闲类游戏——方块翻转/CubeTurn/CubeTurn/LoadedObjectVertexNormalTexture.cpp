#include "LoadedObjectVertexNormalTexture.h"
#include "ShaderUtil.h"
#include "MatrixState.hpp"
#include "Constant.hpp"

int LoadedObjectVertexNormalTexture::colAndRow[4]={3,3,5,3};
int LoadedObjectVertexNormalTexture::X_OFFSET_CHANGE[4]={1,-1,0,0};
int LoadedObjectVertexNormalTexture::Z_OFFSET_CHANGE[4]={0,0,-1,1};
int LoadedObjectVertexNormalTexture::ROTATE_ANMI_ID[4]={0,1,2,3};
float LoadedObjectVertexNormalTexture::location[4]={-15,-30,5,-30};

LoadedObjectVertexNormalTexture::LoadedObjectVertexNormalTexture(float* vertices, float* normals, float* texCoors,int vCount,int flag)
{
    initVertexData(vertices,normals,texCoors,vCount);
    initShader();
    this->flag=flag;
    if(flag==Constant::BLUE_CUBEID)
    {
        alStr[0]="BLUE_CUBEID";
    }else if(flag==Constant::GREEN_CUBEID)
    {
        alStr[0]="GREEN_CUBEID";
    }
    alBool[0]=false;
}


void LoadedObjectVertexNormalTexture::initVertexData(float* vertices, float* normals, float* texCoors,int count)
{
    vCount=count/3;

    mVertexBuffer=vertices;
    mTexCoorBuffer=texCoors;
    mNormalBuffer=normals;
    
    preBox = new AABB3(vertices,count);
}

void LoadedObjectVertexNormalTexture::initShader()
{
    mProgram = ShaderUtil::createProgram("vertex", "frag");
    
    //获取程序中顶点位置属性引用id
    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    //获取程序中顶点纹理属性引用id
    maTexCoorHandle=glGetAttribLocation(mProgram, "aTexCoor");
    //获取程序中顶点法向量属性引用id
    maNormalHandle= glGetAttribLocation(mProgram, "aNormal");
    //获取程序中总变换矩阵引用id
    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    //获取程序中摄像机位置引用id
    maCameraHandle=glGetUniformLocation(mProgram, "uCamera");
    //获取程序中光源位置引用id
    maLightLocationHandle=glGetUniformLocation(mProgram, "uLightLocationSun");
    //获取位置、旋转变换矩阵引用id
    muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
}

void LoadedObjectVertexNormalTexture::drawSelf(int texId)
{
    copyM();
    glUseProgram(mProgram);
    glUniformMatrix4fv(muMVPMatrixHandle, 1, 0, MatrixState::getFinalMatrix());
    //将位置、旋转变换矩阵传入着色器程序
    glUniformMatrix4fv(muMMatrixHandle, 1, 0, MatrixState::getMMatrix());
    //将摄像机位置传入着色器程序
    glUniform3fv(maCameraHandle, 1, MatrixState::cameraFB);
    //将光源位置传入着色器程序
    glUniform3fv(maLightLocationHandle, 1, MatrixState::lightPositionFBSun);
    
    glVertexAttribPointer(maPositionHandle,3, GL_FLOAT, GL_FALSE,3*4, mVertexBuffer);
    glVertexAttribPointer(maTexCoorHandle,2,GL_FLOAT,GL_FALSE,2*4,mTexCoorBuffer);
    glVertexAttribPointer(maNormalHandle,3,GL_FLOAT,GL_FALSE,3*4,mNormalBuffer);
    
    
    glEnableVertexAttribArray(maPositionHandle);
    glEnableVertexAttribArray(maTexCoorHandle);
    glEnableVertexAttribArray(maNormalHandle);
    
    //绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glDrawArrays(GL_TRIANGLES, 0, vCount);
    
    glDisableVertexAttribArray(maPositionHandle);
    glDisableVertexAttribArray(maTexCoorHandle);
    glDisableVertexAttribArray(maNormalHandle);
}

AABB3* LoadedObjectVertexNormalTexture::getCurrBox()
{
    return preBox->setToTransformedBox(m);
}

void LoadedObjectVertexNormalTexture::copyM()
{
    for(int i=0;i<16;i++){
        m[i]=MatrixState::getMMatrix()[i];
    }
}

void LoadedObjectVertexNormalTexture::changeType(int flagTemp)
{
    this->flag=flagTemp;
}

void LoadedObjectVertexNormalTexture::setInitCubeLocation(MyData* md,float* data,int col,int row,float* map_data,int* map)
{
    this->mCubeTransX=data[0];
    this->mCubeTransY=data[1];
    this->mCubeTransZ=data[2];
    this->MAP_COl_CUR=col;
    this->MAP_ROW_CUR=row;
    this->MAP_DATA_CUR=map_data;
    this->MAP_CUR=map;
    this->afterXOffset=mCubeTransX;
    this->afterZOffset=mCubeTransZ;
    this->currCol=(int) data[3];
    this->currRow=(int) data[4];
    float * temps=new float[3];
    this->md=md;
    temps[0]=mCubeTransX;
    temps[1]=mCubeTransY;
    temps[2]=mCubeTransZ;
    alPoints[0]=temps;
}

void LoadedObjectVertexNormalTexture::keyChange(int state, bool isCube)//计算方块移动位移的 方法
{
    if(!isCube||state == -1)
    {
        return;
    }
    static int sign0[] = {-1, 1, 1, 1, 1, 1, -1, 1};
    static int sign1[] = {-1, -1, 1, -1, 1, -1, -1, -1};
    static int hal[] = {1, -1, -1, -1, -1, -1, 1, -1};
    static int ang[] = {1, -1, 1, -1};
    if(!isTwoSteps)//如果只移动一步
    {
        tempCenterY=(float)(sign0[state*2+1] * sin(Constant::toRadians(angleDraw+45))*Constant::RADIUS)+hal[state*2+1] * Constant::HalfCubeSize;
        if(state / 2 == 0){
            tempCenterX=(float)(sign0[state*2] * cos(Constant::toRadians(angleDraw+45))*Constant::RADIUS)+hal[state*2] * Constant::HalfCubeSize;
            angleZ = angleDraw * ang[state];
        }else{
            tempCenterZ=(float)(sign0[state*2] * cos(Constant::toRadians(angleDraw+45))*Constant::RADIUS)+hal[state*2] * Constant::HalfCubeSize;
            angleX = angleDraw * ang[state];
        }
        if(angleDraw<90)
        {
            angleDraw+=Constant::ONE_STEP_ANGLE_PER_FRAME;//每次移动的度数加5
        }
    }
    else{//如果需要移动两步
        if(!isFirstFinish)//首先走第一步
        {
            tempCenterY=(float)(sign0[state*2+1] * sin(Constant::toRadians(angleDraw+45))*Constant::RADIUS)+hal[state*2+1] * Constant::HalfCubeSize;
            if(state / 2 == 0){
                tempCenterX=(float)(sign0[state*2] * cos(Constant::toRadians(angleDraw+45))*Constant::RADIUS)+hal[state*2] * Constant::HalfCubeSize;
                angleZ = angleDraw * ang[state];
            }else{
                tempCenterZ=(float)(sign0[state*2] * cos(Constant::toRadians(angleDraw+45))*Constant::RADIUS)+hal[state*2] * Constant::HalfCubeSize;
                angleX = angleDraw * ang[state];
            }
            if(angleDraw<90)
            {
                isFirstFinish = false;//正在走第一步
                angleDraw+=Constant::TWO_STEP_ANGLE_PER_FRAME;//每次移动步数加10
            }else
            {
                isFirstFinish = true;//第一步已经走完
                tempX = tempCenterX;//纪录当前位移值
                tempY = tempCenterY;
                tempZ = tempCenterZ;
            }
        }else//其次走第二步
        {
            tempCenterY=tempY+((float)(sign1[state*2+1] * cos(Constant::toRadians(angleDraw+45))*Constant::RADIUS)+hal[state*2+1] * Constant::HalfCubeSize);
            if(state / 2 == 0){
                tempCenterX=tempX+((float)(sign1[state*2+0] * sin(Constant::toRadians(angleDraw+45))*Constant::RADIUS)+hal[state*2+0] * Constant::HalfCubeSize);
                angleZ = angleDraw * ang[state];
            }else{
                tempCenterZ=tempZ+((float)(sign1[state*2+0] * sin(Constant::toRadians(angleDraw+45))*Constant::RADIUS)+hal[state*2+0] * Constant::HalfCubeSize);
                angleX = angleDraw * ang[state];
            }
            if(angleDraw<180)
            {
                angleDraw+=Constant::TWO_STEP_ANGLE_PER_FRAME;//每次移动步数加10
            }else
            {
                isFirstFinish = false;//第二步已经走完，恢复标志位
            }
        }
    }
}


//根据按键状态计算下一步的位置
void LoadedObjectVertexNormalTexture::keyPress(int keyNumber,int tCol,int tRow,bool isBack)
{
    
    this->isTouch=true;//当前物体被触摸
    int tempCol=-1;
    int tempRow=-1;
    //根据当前姿态号，按键号确定结果XZ位置，每次移动两步
    float tempAfterXOffset=this->mCubeTransX+X_OFFSET_CHANGE[keyNumber]*Constant::CubeSize*2;
    float tempAfterZOffset=this->mCubeTransZ+Z_OFFSET_CHANGE[keyNumber]*Constant::CubeSize*2;
    //根据当前姿态号，按键号确定结果XZ位置，每次移动一步后的坐标
    float tempAfterXOffset2=this->mCubeTransX+X_OFFSET_CHANGE[keyNumber]*Constant::CubeSize;
    float tempAfterZOffset2=this->mCubeTransZ+Z_OFFSET_CHANGE[keyNumber]*Constant::CubeSize;
    
    if(this->flag==Constant::RED_CUBEID)
    {//红色木块
        int* result1 = ifPointsExists(tempAfterXOffset2,tempAfterZOffset2,tCol,tRow,true);
        if(result1[0]&&result1[1]&&result1[4])//下一步可通过并且不与任何物体相撞
        {
            int* result2 = ifPointsExists(tempAfterXOffset,tempAfterZOffset,tCol,tRow,true);
            if(result2[0]&&result2[1]&&result2[4])//下两步可通过并且不与任何物体相撞
            {//下一步可通过的情况下，判断第两步是否可以通过
                tempCol=result2[3];
                tempRow=result2[2];
                isTwoSteps=true;
            }
        }
    }
    
    //移动两步不成功的情况下，移动一步或者就只移动一步
    if((tempCol==-1&&tempRow==-1))
    {
        int* result3 = ifPointsExists(tempAfterXOffset2, tempAfterZOffset2, tCol, tRow,false);
        //如果上面进行的是每次移动两步，则执行每次移动一步的动作，否则不执行此处的任务
        if(result3[0]&&result3[1])
        {
            tempCol=result3[3];
            tempRow=result3[2];
            isTwoSteps=false;
            if(this->md->level==3&&!isBack){//缩小
                if(currCol==colAndRow[0]&&currRow==colAndRow[1]){//green
                    this->md->isGreenScale=false;
                    this->md->scaleGreen=2.0f;
                }else if(currCol==colAndRow[2]&&currRow==colAndRow[3]){//blue
                    this->md->isBlueScale=false;
                    this->md->scaleBlue=2.0f;
                }
            }
        }else if(result3[0]&&!result3[1])
        {
            this->md->isStop=true;
            if(this->mCubeTransX==this->md->controlCubeData[this->md->level*4+2]
                &&this->mCubeTransZ==this->md->controlCubeData[this->md->level*4+3]){
                cubeOnControl = true;
            }
        }
    }
    
    if(tempRow!=-1&&tempCol!=-1&&MAP_CUR[tempRow*MAP_COl_CUR+tempCol]==1)
    {//表示可通过
        currCol=tempCol;
        currRow=tempRow;
        count++;
        isMove=true;
        if(this->md->level==3&&!isBack)
        {//放大
            if(currCol==colAndRow[0]&&currRow==colAndRow[1])
            {//green
                this->md->isGreenScale=true;
            }else if(currCol==colAndRow[2]&&currRow==colAndRow[3])
            {//blue
                this->md->isBlueScale=true;
            }
        }
        
        afterXOffset = isTwoSteps? tempAfterXOffset : tempAfterXOffset2;
        afterZOffset = isTwoSteps? tempAfterZOffset : tempAfterZOffset2;
        float* tempD = new float[3];
        tempD[0] = afterXOffset;
        tempD[1] = mCubeTransY;
        tempD[2] = afterZOffset;
        alPoints[count] = tempD;
        if(flag==Constant::BLUE_CUBEID)
        {
            alStr[count]="BLUE_CUBEID";
        }else if(flag==Constant::GREEN_CUBEID)
        {
            alStr[count]="GREEN_CUBEID";
        }
        alBool[count]=false;
        //根据当前姿态号，按键号确定动画号
        keyState=ROTATE_ANMI_ID[keyNumber];
    }else{
        count++;
        keyState=-1;
        isMove=false;
        float* tempD1 = new float[3];
        tempD1[0] = this->mCubeTransX;
        tempD1[1] = this->mCubeTransY;
        tempD1[2] = this->mCubeTransZ;
        alPoints[count] = tempD1;
    }
    setCubeUpState();//设置可升降方块的升降状态
}

int* LoadedObjectVertexNormalTexture::ifPointsExists(float tempX, float tempZ, int tCol, int tRow,bool go)
{
    int* result = new int[5];
    result[0] = 0;
    result[1] = 0;
    result[2] = -1;
    result[3] = -1;
    result[4] = 0;
    for(int i=0;i<MAP_ROW_CUR;i++)
    {
        for(int j=0;j<MAP_COl_CUR;j++)
        {
            if(MAP_DATA_CUR[2*(i*MAP_COl_CUR+j)+0]==tempX
               &&MAP_DATA_CUR[2*(i*MAP_COl_CUR+j)+1]==tempZ&&isCubeOcclusion(j,i))
            {
                result[0] = 1;
                
                if(go)
                {
                    if(MAP_CUR[i*MAP_COl_CUR+j]==1)
                    {
                        result[4] = 1;
                    }else
                    {
                        result[4] = 0;
                    }
                }
                if(!(j==tCol&&i==tRow))
                {
                    result[1] = 1;
                    result[2] = i;
                    result[3] = j;
                    break;
                }else
                {
                    result[1] = 0;
                }
                
            }
        }
    }
    return result;
}


void LoadedObjectVertexNormalTexture::setCubeUpState()//设置可升降方块的升降状态
{
    if(this->md->level==Constant::LEVEL_THIRD)//判断第三关方块可升降状态
    {
        if(this->flag==Constant::BLUE_CUBEID)//判断蓝色当前在什么位置
        {
            setThirdLevelCubeUpState(0);
            setThirdLevelCubeUpState(2);
            //如果红色可以先走一步或者两步，并且当前蓝色在机关位置，则机关标志设为升起状态
            if(cubeOnControl)
            {
                this->md->isCubeUp[this->md->level*2+1]  = true;
                cubeOnControl = false;
            }
        }
    }else if(this->md->level==Constant::LEVEL_FOUTH)//判断第四关方块可升降状态
    {
        if(this->flag==Constant::BLUE_CUBEID){//判断蓝色当前在什么位置
            setForthLevelCubeUpState(0);
        }
        if(this->flag==Constant::GREEN_CUBEID){//判断绿色当前在什么位置
            setForthLevelCubeUpState(2);
        }
    }
}

void LoadedObjectVertexNormalTexture::setThirdLevelCubeUpState(int index)//设置第三关的方块可升降状态
{
    if(afterXOffset==this->md->controlCubeData[this->md->level*4+index]&&afterZOffset==this->md->controlCubeData[this->md->level*4+index+1])
    {
        this->md->isCubeUp[this->md->level*2+index/2]=false;
    }else
    {
        this->md->isCubeUp[this->md->level*2+index/2]=true;
    }
}
void LoadedObjectVertexNormalTexture::setForthLevelCubeUpState(int index)//设置第四关的方块可升降状态
{
    if(afterXOffset==this->md->controlCubeData[this->md->level*4+index]&&afterZOffset==this->md->controlCubeData[this->md->level*4+index+1])
    {
        this->md->everCubeUp[index/2] = true;//只要有一个方块在机关位置，则方块下降
        this->md->isCubeUp[this->md->level*2+index/2]=false;
    }else
    {
        if(!this->md->everCubeUp[index/2])
        {
            this->md->isCubeUp[this->md->level*2+index/2]=true;
        }
    }
}

bool LoadedObjectVertexNormalTexture::isCubeOcclusion(int j,int i)
{
    bool isCube1=false;
    bool isCube2=false;
    if(this->md->isCubeUp[this->md->level*2+0])//方块为上升状态
    {
        //需要判断下一步是否会碰到升起的方块
        if(!(j==this->md->upCubeRowAndColData[this->md->level*4+1]&&i==this->md->upCubeRowAndColData[this->md->level*4+0]))
        {
            isCube1 = true;
        }
    }else
    {
        isCube1 = true;
    }
    if(this->md->isCubeUp[this->md->level*2+1])
    {
        if(!(j==this->md->upCubeRowAndColData[this->md->level*4+3]&&i==this->md->upCubeRowAndColData[this->md->level*4+2]))
        {
            isCube2 = true;
        }
    }else
    {
        isCube2 = true;
    }
    return isCube1&&isCube2;
}

//根据摄像机位置更新按键的状态
void LoadedObjectVertexNormalTexture::updateKeyStatus(int camearStatus,float distanceX,float distanceY,int tCol,int tRow,bool isBack)
{
    bool flagTemp=abs(distanceX)>=abs(distanceY)?true:false;
    if( camearStatus==0)
    {//摄像机正对场景
        if(flagTemp&&distanceX<0)
        {//左
            if(flag==Constant::GREEN_CUBEID)
            {
                keyPress(Constant::RIGHT_KEY, tCol,tRow,isBack);
            }else{
                keyPress(Constant::LEFT_KEY,tCol, tRow,isBack);
            }
            
        }else if(flagTemp&&distanceX>0)
        {//右
             if(flag==Constant::GREEN_CUBEID){
                 keyPress(Constant::LEFT_KEY,tCol, tRow,isBack);
             }else{
                 keyPress(Constant::RIGHT_KEY,tCol, tRow,isBack);
             }
        }else if(!flagTemp&&distanceY<0)
        {//上
             if(flag==Constant::GREEN_CUBEID){
                 keyPress(Constant::DOWN_KEY,tCol, tRow,isBack);
             }else{
                 keyPress(Constant::UP_KEY,tCol, tRow,isBack);
             }
        }else if(!flagTemp&&distanceY>0)
        {//下
            if(flag==Constant::GREEN_CUBEID){
                keyPress(Constant::UP_KEY,tCol, tRow,isBack);
            }else{
                keyPress(Constant::DOWN_KEY,tCol, tRow,isBack);
            }
        }
    }else if( camearStatus==1)
    {//摄像机在场景的左侧
        if(flagTemp&&distanceX<0)
        {
            if(flag==Constant::GREEN_CUBEID){
                keyPress(Constant::DOWN_KEY,tCol, tRow,isBack);
            }else{
                keyPress(Constant::UP_KEY,tCol, tRow,isBack);
            }
        }else if(flagTemp&&distanceX>0)
        {
            if(flag==Constant::GREEN_CUBEID){
                keyPress(Constant::UP_KEY,tCol, tRow,isBack);
            }else{
                keyPress(Constant::DOWN_KEY,tCol, tRow,isBack);
            }
        }else if(!flagTemp&&distanceY<0)
        {
            if(flag==Constant::GREEN_CUBEID){
                keyPress(Constant::LEFT_KEY,tCol, tRow,isBack);
            }else{
                keyPress(Constant::RIGHT_KEY,tCol, tRow,isBack);
            }
        }else if(!flagTemp&&distanceY>0)
        {
            if(flag==Constant::GREEN_CUBEID){
                keyPress(Constant::RIGHT_KEY,tCol, tRow,isBack);
            }else{
                keyPress(Constant::LEFT_KEY,tCol, tRow,isBack);
            }
        }
    }else if(camearStatus==2)
    {//摄像机在场景的右侧
        if(flagTemp&&distanceX<0)
        {
            if(flag==Constant::GREEN_CUBEID){
                keyPress(Constant::UP_KEY,tCol, tRow,isBack);
            }else{
                keyPress(Constant::DOWN_KEY,tCol, tRow,isBack);
            }
        }else if(flagTemp&&distanceX>0)
        {
            if(flag==Constant::GREEN_CUBEID){
                keyPress(Constant::DOWN_KEY,tCol, tRow,isBack);
            }else{
                keyPress(Constant::UP_KEY,tCol, tRow,isBack);
            }
        }else if(!flagTemp&&distanceY<0)
        {
            if(flag==Constant::GREEN_CUBEID){
                keyPress(Constant::RIGHT_KEY,tCol, tRow,isBack);
            }else{
                keyPress(Constant::LEFT_KEY,tCol, tRow,isBack);
            }
        }else if(!flagTemp&&distanceY>0)
        {
            if(flag==Constant::GREEN_CUBEID){
                keyPress(Constant::LEFT_KEY,tCol, tRow,isBack);
            }else{
                keyPress(Constant::RIGHT_KEY,tCol, tRow,isBack);
            }
        }
    }else if( camearStatus==3)
    {//摄像机在场景的后侧
        if(flagTemp&&distanceX<0)
        {
            if(flag==Constant::GREEN_CUBEID){
                keyPress(Constant::LEFT_KEY,tCol, tRow,isBack);
            }else{
                keyPress(Constant::RIGHT_KEY,tCol, tRow,isBack);
            }
            
        }else if(flagTemp&&distanceX>0)
        {
            if(flag==Constant::GREEN_CUBEID){
                keyPress(Constant::RIGHT_KEY,tCol, tRow,isBack);
            }else{
                keyPress(Constant::LEFT_KEY,tCol, tRow,isBack);
            }
   	        
        }else if(!flagTemp&&distanceY<0)
        {
            if(flag==Constant::GREEN_CUBEID){
                keyPress(Constant::UP_KEY,tCol, tRow,isBack);
            }else{
                keyPress(Constant::DOWN_KEY,tCol, tRow,isBack);
            }
            
        }else if(!flagTemp&&distanceY>0)
        {
            if(flag==Constant::GREEN_CUBEID){
                keyPress(Constant::DOWN_KEY,tCol, tRow,isBack);
            }else{
                keyPress(Constant::UP_KEY,tCol, tRow,isBack);
            }
        }
    }
}

//后退一步的方法
void LoadedObjectVertexNormalTexture::back(int level)
{
    if((count-1)<0){return;}
    //第3或4关卡，若后退之前是机关方块，则可升降的方块上升
    if(level==Constant::LEVEL_FOUTH||level==Constant::LEVEL_THIRD){
        cubeUpAndDown(alPoints[count][0],alPoints[count][2],level,true);
    }
    alPoints.erase(count);//删除上一个点
    count--;
    mCubeTransX=afterXOffset=alPoints[count][0];//获取x坐标
    mCubeTransZ=afterZOffset=alPoints[count][2];//获取z坐标

    if(level==Constant::LEVEL_FOUTH)
    {//最后一个关卡,缩小
        if(alStr[count]=="GREEN_CUBEID"&&alBool[count+1]){//若上一步为绿色，则变回绿色
            changeType(Constant::GREEN_CUBEID);
        }
        if(alStr[count]=="BLUE_CUBEID"&&alBool[count+1]){//若上一步为蓝色，则变回蓝色
            changeType(Constant::BLUE_CUBEID);
        }
        if(mCubeTransX==location[0]&&mCubeTransZ==location[1]){//变成绿色
            changeType(Constant::GREEN_CUBEID);
            alStr[count]="GREEN_CUBEID";
            alBool[count]=true;
        }
        if(mCubeTransX==location[2]&&mCubeTransZ==location[3]){//变成蓝色
            changeType(Constant::BLUE_CUBEID);
            alStr[count]="BLUE_CUBEID";
            alBool[count]=true;
        }
        alBool.erase(count+1);
    }
    //第3或4关卡时，若后退一步遇到机关方块，则可升降的方块下降
    if(level==Constant::LEVEL_FOUTH||level==Constant::LEVEL_THIRD){
        cubeUpAndDown(mCubeTransX,mCubeTransZ,level,false);
    }
}
void LoadedObjectVertexNormalTexture::cubeUpAndDown(float x,float z,int level,bool isUpOrDown)//后退时设置可升降方块的标志位
{
    if(x==MyData::controlCubeData[level*4+0]&&z==MyData::controlCubeData[level*4+1])
    {//判断当前位置是否为机关方块
        MyData::isCubeUp[level*2+0] = isUpOrDown;
    }
    if(x==MyData::controlCubeData[level*4+2]&&z==MyData::controlCubeData[level*4+3])
    {//判断当前位置是否为机关方块
        MyData::isCubeUp[level*2+1] = isUpOrDown;
    }
}


//旋转动画完成后恢复扰动
void LoadedObjectVertexNormalTexture::reCoverVariables(int viewId,int level,bool isBack)
{
    if(level==Constant::LEVEL_FOUTH&&viewId==Constant::GAME_VIEW_ID)
    {//最后一个关卡,缩小
        if(currCol==colAndRow[0]&&currRow==colAndRow[1])
        {//green
            changeType(Constant::GREEN_CUBEID);
            alStr[count]="GREEN_CUBEID";
            alBool[count]=true;
            this->md->isGreenScale=false;
            if(!isBack){this->md->scaleGreen=2.0f;}
        }else if(currCol==colAndRow[2]&&currRow==colAndRow[3])
        {//blue
            changeType(Constant::BLUE_CUBEID);
            alStr[count]="BLUE_CUBEID";
            alBool[count]=true;
            this->md->isBlueScale=false;
            if(!isBack){this->md->scaleBlue=2.0f;}
        }
    }
    tempCenterX=0;
    tempCenterY=0;
    tempCenterZ=0;
    angleX=0;
    angleY=0;
    angleZ=0;
    angleDraw=0;
    
    mCubeTransX=afterXOffset;
    mCubeTransZ=afterZOffset;
    isMove=false;
    if(isTouch){//结束触摸
        isTouch=!isTouch;
    }
}

//重置变量
void LoadedObjectVertexNormalTexture::initVariables(float* data)
{
    initCubes(data);
    keyState=-1;
    count=0;
    afterXOffset=mCubeTransX;
    afterZOffset=mCubeTransZ;
    this->md->isStop=false;
    isTwoSteps=false;
    isChangeColor=false;
    isMove=false;
}
void LoadedObjectVertexNormalTexture::initCubes(float * data)
{
    mCubeTransX=data[0];
    mCubeTransY=data[1];
    mCubeTransZ=data[2];
    
    currCol=(int) data[3];
    currRow=(int) data[4];
    reCoverHelpView();
}
void LoadedObjectVertexNormalTexture::reCoverHelpView()//帮助界面初始化变量
{
    tempCenterX=0;
    tempCenterY=0;
    tempCenterZ=0;
    
    angleZ=0;
    angleY=0;
    angleX=0;
    angleDraw=0;

    isTouch=false;
    isFirstFinish=false;
}

LoadedObjectVertexNormalTexture::~LoadedObjectVertexNormalTexture()
{
    
}
