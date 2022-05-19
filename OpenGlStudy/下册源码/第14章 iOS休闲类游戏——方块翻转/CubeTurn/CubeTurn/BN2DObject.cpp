#include "BN2DObject.h"
#include "ShaderUtil.h"
#include "MatrixState2D.hpp"
BN2DObject::BN2DObject(bool drawNum,int num,int texId,float width,float height)
{
    this->drawNum = drawNum;
    this->num=num;
    this->texId=texId;
    
    initVertexData(width,height);
    initShader();
}


void BN2DObject::initVertexData(float width, float height)
{
    vCount=4;
    float* vertices=new float[12];
    vertices[0] = -width/2;
    vertices[1] = height/2;
    vertices[2] = 0;
    vertices[3] = -width/2;
    vertices[4] = -height/2;
    vertices[5] = 0;
    vertices[6] = width/2;
    vertices[7] = height/2;
    vertices[8] = 0;
    vertices[9] = width/2;
    vertices[10] = -height/2;
    vertices[11] = 0;
    mVertexBuffer=vertices;
    
    float *texCoor = new float[12];
    if(this->drawNum)
    {
        //一行一10列
        float rateS=0.1f*num;
        texCoor[0]=rateS;
        texCoor[1]=0;
        
        texCoor[2]=rateS;
        texCoor[3]=1;
        
        texCoor[4]=0.1f+rateS;
        texCoor[5]=0;
        
        texCoor[6]=0.1f+rateS;
        texCoor[7]=1;
        
        texCoor[8]=0.1f+rateS;
        texCoor[9]=0;
        
        texCoor[10]=rateS;
        texCoor[11]=1;
        
        //两行五列
//        float rateS=0.2f*(num%5);
//        float rateT=0.5f*(num/5);
//        texCoor[0]=0+rateS;
//        texCoor[1]=0+rateT;
//        
//        texCoor[2]=0+rateS;
//        texCoor[3]=0.5f+rateT;
//        
//        texCoor[4]=1*0.2f+rateS;
//        texCoor[5]=0+rateT;
//        
//        texCoor[6]=1*0.2f+rateS;
//        texCoor[7]=0.5f+rateT;
//        
//        texCoor[8]=1*0.2f+rateS;
//        texCoor[9]=0+rateT;
//        
//        texCoor[10]=0+rateS;
//        texCoor[11]=0.5f+rateT;
    }else
    {
        texCoor[0]=0;texCoor[1]=0;
        texCoor[2]=0;texCoor[3]=1;
        texCoor[4]=1;texCoor[5]=0;
        texCoor[6]=1;texCoor[7]=1;
        texCoor[8]=1;texCoor[9]=0;
        texCoor[10]=0;texCoor[11]=1;
    }
    mTexCoorBuffer=texCoor;
}

void BN2DObject::initShader(){
    mProgram = ShaderUtil::createProgram("vertex_rect", "frag_rect");
    //获取程序中顶点位置属性引用id
    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    //获取程序中顶点纹理属性引用id
    maTexCoorHandle=glGetAttribLocation(mProgram, "aTexCoor");
    //获取程序中总变换矩阵引用id
    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    
    muSJ = glGetUniformLocation(mProgram, "uSJ");
    
}

void BN2DObject::drawSelf(float x,float y,float sj)
{
    MatrixState2D::pushMatrix();
    MatrixState2D::translate(x, y, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(mProgram);
    glUniform1f(muSJ, sj);
    glUniformMatrix4fv(muMVPMatrixHandle, 1, 0, MatrixState2D::getFinalMatrix());
    glVertexAttribPointer(maPositionHandle,3, GL_FLOAT, GL_FALSE,3*4, mVertexBuffer);
    glVertexAttribPointer(maTexCoorHandle,2,GL_FLOAT,GL_FALSE,2*4,mTexCoorBuffer);
    
    glEnableVertexAttribArray(maPositionHandle);
    glEnableVertexAttribArray(maTexCoorHandle);
    
    //绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vCount);
    glDisableVertexAttribArray(maPositionHandle);
    glDisableVertexAttribArray(maTexCoorHandle);
    glDisable(GL_BLEND);
    MatrixState2D::popMatrix();
}
void BN2DObject::setTexId(int textureId)
{
    this->texId = textureId;
}
BN2DObject::~BN2DObject()
{
    
}