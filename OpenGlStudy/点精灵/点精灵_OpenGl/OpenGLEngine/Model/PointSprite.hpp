//
//  PointSprite.hpp
//  PointSprite
//
//  Created by 苏莫离 on 2019/10/17.
//

#ifndef PointSprite_hpp
#define PointSprite_hpp

#include <iostream>
#import <OpenGLES/ES3/gl.h>

class PointSprite {
    GLuint mProgram;// 自定义渲染管线着色器程序id
    GLuint muMVPMatrixHandle;// 总变换矩阵引用
    GLuint maPositionHandle; // 顶点位置属性引用

    const GLvoid* mVertexBuffer;//顶点坐标数据缓冲
    int vCount = 0;

    void initVertexData();
    void initShader();
    
public:
    PointSprite();
    void drawSelf(int texId);
};


#endif
