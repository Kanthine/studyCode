//
//  Mountion.hpp
//  Mist
//
//  Created by 苏莫离 on 2019/5/15.
//

#ifndef Mountion_hpp
#define Mountion_hpp

#include <iostream>
#import <OpenGLES/ES3/gl.h>
#include <vector>

using namespace std;

class Mountion {
    GLuint mProgram; //自定义渲染管线的id
    GLuint muMVPMatrixHandle; //总变化矩阵引用的id
    GLuint muMMatrixHandle; //基本变换阵引用的id
    GLuint muCamaraLocationHandle; //摄像机位置引用的id
    GLuint slabYHandle; //体积雾产生者平面高度引用的id
    GLuint startAngleHandle; //体积雾高度扰动起始角引用的id
    GLuint maPositionHandle; //顶点位置属性引用id
    GLuint maTexCoorHandle; //顶点纹理坐标属性引用id

    GLuint landStartYYHandle; //起始x值
    GLuint landYSpanHandle; //长度
    
    const GLvoid *mVertexBuffer;//顶点坐标数据缓冲
    const GLvoid *mTexCoorBuffer;//纹理坐标数据缓冲
    
    float unitSize = 3.0f;   //单位长度
    float TJ_GOG_SLAB_Y = 8.0f; //体积雾产生者平面的高度
    float startAngle = 0; //扰动起始角
    int vCount = 0; //顶点数量
    
    float toRadians(float angle);
    void initVertexData(vector<vector<float>> yArray);
    void initShader();
public:
    Mountion(vector<vector<float>> yArray);
    void drawSelf(void);
};

#endif 
