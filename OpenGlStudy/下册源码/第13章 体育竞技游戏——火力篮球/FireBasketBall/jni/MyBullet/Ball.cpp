#include "Ball.h"
#include "util/ShaderUtil.h"
#include "util/MatrixState.h"

#include <math.h>


#include <stdio.h>
#include <stdlib.h>
#include "util/FileUtil.h"
#include "util/Constant.h"

#include <string>
using namespace std;


Ball::Ball(
			int texIdIn,
			float r,//半径
			GLuint programIn
			)
{
	this->r = r;
	initVertexData();
	initShader(programIn);
}

float Ball::toRadians(float angle)
{
    return angle*3.1415926f/180;//试一下
}
//自动切分纹理产生纹理数组的方法
void Ball::generateTexCoor(int bw,int bh,float* tex){
    //float result[bw*bh*6*2];
    float sizew=1.0f/bw;//列数
    float sizeh=1.0f/bh;//行数
    int c=0;
    for(int i=0;i<bh;i++){
        for(int j=0;j<bw;j++){
            //每行列一个矩形，由两个三角形构成，共六个点，12个纹理坐标
            float s=j*sizew;
            float t=i*sizeh;
            tex[c++]=s;
            tex[c++]=t;
            tex[c++]=s;
            tex[c++]=t+sizeh;
            tex[c++]=s+sizew;
            tex[c++]=t;
            tex[c++]=s+sizew;
            tex[c++]=t;
            tex[c++]=s;
            tex[c++]=t+sizeh;
            tex[c++]=s+sizew;
            tex[c++]=t+sizeh;
    }}
}
void Ball::initVertexData()
{
	vCount=11664/3;//顶点的数量为坐标值数量的1/3，因为一个顶点有3个坐标
    //顶点坐标数据的初始化================begin============================
    const float angleSpan=10;//将球进行单位切分的角度
    float *vertices=new float[11664];//36*18*6*3
	int count=0;//顶点计数器

    for(float vAngle=90;vAngle>-90;vAngle=vAngle-angleSpan){//垂直方向angleSpan度一份
        for(float hAngle=360;hAngle>0;hAngle=hAngle-angleSpan){//水平方向angleSpan度一份
            //纵向横向各到一个角度后计算对应的此点在球面上的坐标
            double xozLength=r*cos(toRadians(vAngle));
            float x1=(float)(xozLength*cos(toRadians(hAngle)));
            float z1=(float)(xozLength*sin(toRadians(hAngle)));
            float y1=(float)(r*sin(toRadians(vAngle)));

            xozLength=r*cos(toRadians(vAngle-angleSpan));
            float x2=(float)(xozLength*cos(toRadians(hAngle)));
            float z2=(float)(xozLength*sin(toRadians(hAngle)));
            float y2=(float)(r*sin(toRadians(vAngle-angleSpan)));

            xozLength=r*cos(toRadians(vAngle-angleSpan));
            float x3=(float)(xozLength*cos(toRadians(hAngle-angleSpan)));
            float z3=(float)(xozLength*sin(toRadians(hAngle-angleSpan)));
            float y3=(float)(r*sin(toRadians(vAngle-angleSpan)));

            xozLength=r*cos(toRadians(vAngle));
            float x4=(float)(xozLength*cos(toRadians(hAngle-angleSpan)));
            float z4=(float)(xozLength*sin(toRadians(hAngle-angleSpan)));
            float y4=(float)(r*sin(toRadians(vAngle)));

        	vertices[count++]=x1;
        	vertices[count++]=y1;
        	vertices[count++]=z1;

        	vertices[count++]=x2;
        	vertices[count++]=y2;
        	vertices[count++]=z2;

        	vertices[count++]=x4;
        	vertices[count++]=y4;
        	vertices[count++]=z4;

        	vertices[count++]=x4;
        	vertices[count++]=y4;
        	vertices[count++]=z4;

        	vertices[count++]=x2;
        	vertices[count++]=y2;
        	vertices[count++]=z2;

        	vertices[count++]=x3;
        	vertices[count++]=y3;
        	vertices[count++]=z3;

    }}
	mVertexBuffer = &vertices[0];
	mNormalBuffer = &vertices[0];//给顶点法向量数据缓冲赋值

    float *textures=new float[vCount*2];
    generateTexCoor(36, 18, textures);
    mTextureBuffer = &textures[0];
}

//初始化着色器的intShader方法
void Ball::initShader(GLuint programIn)
{
	this->mProgram=programIn;
	//获取程序中顶点位置属性引用id
	maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
	//获取程序中顶点纹理坐标属性引用id
	maTexCoorHandle= glGetAttribLocation(mProgram, "aTexCoor");
	//获取程序中总变换矩阵引用id
	muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
	//获取位置、旋转变换矩阵引用id
	muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
	//获取程序中顶点法向量属性引用id
	maNormalHandle= glGetAttribLocation(mProgram, "aNormal");
	//获取程序中光源位置引用id
	maLightLocationHandle=glGetUniformLocation(mProgram, "uLightLocation");
	//获取程序中摄像机位置引用id
	maCameraHandle=glGetUniformLocation(mProgram, "uCamera");
	//获取程序中是否绘制阴影属性引用id
	muIsShadow=glGetUniformLocation(mProgram, "uisShadow");
	muIsShadowFrag=glGetUniformLocation(mProgram, "uisShadowFrag");
	//获取是否绘制篮板上阴影属性应用ID
	muIsLanBanShdow=glGetUniformLocation(mProgram, "uisLanbanFrag");
	//获取程序中摄像机矩阵引用id
	muCameraMatrixHandle=glGetUniformLocation(mProgram, "uMCameraMatrix");
	//获取程序中投影矩阵引用id
	muProjMatrixHandle=glGetUniformLocation(mProgram, "uMProjMatrix");
	//获取桌球纹理属性引用id
	muBallTexHandle=glGetUniformLocation(mProgram, "usTextureBall");
	//获取程序中平面法向量引用id;
	muPlaneN=glGetUniformLocation(mProgram, "uplaneN");
	//获取程序中平面上的点的引用的Id
	muPlaneV=glGetUniformLocation(mProgram, "uplaneA");
}
void Ball::drawSelf(int texIdIn,int isShadow,int planeId,int isLanbanYy)
{
	//制定使用某套shader程序
	glUseProgram(mProgram);
    //将最终变换矩阵传入shader程序
    glUniformMatrix4fv(muMVPMatrixHandle, 1, 0, MatrixState::getFinalMatrix());
    //将位置、旋转变换矩阵传入shader程序
    glUniformMatrix4fv(muMMatrixHandle, 1, 0, MatrixState::getMMatrix());
    //将光源位置传入shader程序
    glUniform3fv(maLightLocationHandle, 1,MatrixState::lightPositionFBSun);
    //将摄像机位置传入shader程序
    glUniform3fv(maCameraHandle, 1, MatrixState::cameraFB);
    //将是否绘制阴影属性传入shader程序
    glUniform1i(muIsShadow, isShadow);
    glUniform1i(muIsShadowFrag, isShadow);
    glUniform1i(muIsLanBanShdow, isLanbanYy);
    //将摄像机矩阵传入shader程序
    glUniformMatrix4fv(muCameraMatrixHandle, 1, 0, MatrixState::getCaMatrix());
    //将投影矩阵传入shader程序
    glUniformMatrix4fv(muProjMatrixHandle, 1, 0, MatrixState::getProjMatrix());
    //将平面位置传入程序
    glUniform3fv(muPlaneV, 1, Constant::mianFXL[planeId][0]);
    //将平面法向量传入程序
    glUniform3fv(muPlaneN, 1, Constant::mianFXL[planeId][1]);

    //参数：程序中顶点位置属性引用id,一组有3个数，数的类型，GL_FALSE，一组所占字节数，顶点数据的首地址
    glVertexAttribPointer(maPositionHandle,3, GL_FLOAT, GL_FALSE,3*4, mVertexBuffer);
    //参数：程序中顶点纹理属性引用id,一组有2个数，数的类型，GL_FALSE，一组所占字节数，顶点纹理的首地址
    glVertexAttribPointer(maTexCoorHandle,2,GL_FLOAT,GL_FALSE,2*4,mTextureBuffer);
	//为画笔指定顶点法向量数据
	glVertexAttribPointer(maNormalHandle,3,GL_FLOAT,GL_FALSE,3*4,mNormalBuffer);

    glEnableVertexAttribArray(maPositionHandle);//参数：程序中顶点位置属性引用id
    glEnableVertexAttribArray(maTexCoorHandle); //参数：程序中顶点纹理属性引用id		开启？？
    glEnableVertexAttribArray(maNormalHandle);//handle 处置

    //绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texIdIn);
    glUniform1i(muBallTexHandle, 0);

    glDrawArrays(GL_TRIANGLES, 0, vCount);//绘制采用GL_TRIANGLES方式，0，vCount顶点数		？？

    glDisableVertexAttribArray(maPositionHandle);//参数：程序中顶点位置属性引用id
    glDisableVertexAttribArray(maTexCoorHandle); //参数：程序中顶点纹理属性引用id		关闭？？
    glDisableVertexAttribArray(maNormalHandle);
}
