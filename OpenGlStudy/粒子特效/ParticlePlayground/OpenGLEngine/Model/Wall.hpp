//
//  Wall.hpp
//  ParticlePlayground
//
//  Created by 苏莫离 on 2019/5/15.
//

#ifndef Wall_hpp
#define Wall_hpp

#include <iostream>
#import <OpenGLES/ES3/gl.h>

class Wall {
    GLuint mProgram;          /// 自定义渲染管线 id
    GLuint muMVPMatrixHandle; /// 总变换矩阵引用
    GLuint maPositionHandle;  /// 顶点位置属性引用
    GLuint maTexCoorHandle;   /// 顶点纹理坐标
    GLuint muMMatrixHandle;   /// 位置、旋转变换矩阵
    GLuint maLightLocationHandle; /// 光源位置
    GLuint maCameraHandle;    /// 摄像机位置
    GLuint maNormalHandle;    /// 顶点法向量
    
    const GLvoid *mVertexBuffer;//顶点坐标数据缓冲
    const GLvoid *mTexCoorBuffer;//纹理坐标数据缓冲
    const GLvoid *mNormalBuffer;//纹理坐标数据缓冲
    
    int vCount = 0;
    float wallsLength; //立方体屋子以1为单位长度，进行的缩放比例
    
    void initVertexData(float wallsLength);
    void initShader();
public:
    Wall(float wallsLength);
    void drawSelf(GLuint texId);
};


class WallsForwDraw {
    Wall *wall;
public:
    WallsForwDraw();
    void drawSelf();
};


#endif
