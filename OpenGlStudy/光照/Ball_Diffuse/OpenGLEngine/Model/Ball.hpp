//
//  Ball.hpp
//  Ball
//
//  Created by 苏莫离 on 2019/10/17.
//

#ifndef Ball_hpp
#define Ball_hpp

#include <iostream>
#import <OpenGLES/ES3/gl.h>

class Ball {
    GLuint mProgram;          /// 自定义渲染管线 id
    GLuint muMVPMatrixHandle; /// 总变换矩阵引用
    GLuint maPositionHandle;  /// 顶点位置属性引用
    GLuint muRHandle;         /// 球的半径参数引用
    
    GLuint muMMatrixHandle;
    GLuint maNormalHandle;    /// 顶点法向量属性引用
    GLuint maLightLocationHandle; // 光源位置属性引用

    GLuint vCount = 0;
    GLclampf xAngle = 0;  /// 绕 X 轴旋转角度
    GLclampf yAngle = 0;  /// 绕 Y 轴旋转角度
    GLclampf zAngle = 0;  /// 绕 Z 轴旋转角度
    GLclampf r = 1.0f;
    const GLvoid* mVertexBuffer;//顶点坐标数据缓冲
    const GLvoid* mNormalBuffer;//顶点法向量数据缓冲引用

    float toRadians(float angle);
    void initVertexData();
    void initShader();
    
public:
    Ball();
    void drawSelf();
};
#endif /* Ball_hpp */
