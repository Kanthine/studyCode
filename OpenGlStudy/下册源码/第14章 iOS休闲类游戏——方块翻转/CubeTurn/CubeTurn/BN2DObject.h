#ifndef __Sample14_4__BN2DObject__
#define __Sample14_4__BN2DObject__

#include <stdio.h>
#include <iostream>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
class BN2DObject
{
public:
    GLuint mProgram;//自定义渲染管线程序id
    GLuint muMVPMatrixHandle;//总变换矩阵引用id
    GLuint maPositionHandle; //顶点位置属性引用id
    GLuint maTexCoorHandle; //顶点纹理坐标属性引用id
    GLuint muSJ;
    
    const GLvoid* mVertexBuffer;//顶点坐标数据缓冲
    const GLvoid* mTexCoorBuffer;//顶点纹理坐标数据缓冲
    
    int texId;
    int vCount;
    int num;
    bool drawNum;
    
    BN2DObject(bool drawNum,int num,int texId,float width,float height);
    void initVertexData(float width,float height);
    void initShader();
    void drawSelf(float x,float y,float sj);
    void setTexId(int textureId);
    ~BN2DObject();
};

#endif /* defined(__Sample14_4__BN2DObject__) */
