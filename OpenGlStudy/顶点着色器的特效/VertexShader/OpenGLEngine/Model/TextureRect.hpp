//
//  TextureRect.hpp
//  VertexShader
//
//  Created by 苏莫离 on 2019/10/17.
//

#ifndef TextureRect_hpp
#define TextureRect_hpp

#include <iostream>
#import <OpenGLES/ES3/gl.h>

using namespace std;
class TextureRect {
    GLuint mProgram[3];        /// 自定义渲染管线 id
    GLuint muMVPMatrixHandle[3]; /// 总变换矩阵引用
    GLuint maPositionHandle[3];  /// 顶点位置属性引用
    GLuint maTexCoorHandle[3];
    GLuint maStartAngleHandle[3];
    GLuint muWidthSpanHandle[3];
    
    int currIndex = 0;
    GLuint vCount = 0;
    float WIDTH_SPAN = 3.3f;// 3.3f;
    
    const GLvoid *mVertexBuffer;//顶点坐标数据缓冲
    const GLvoid *mTexCoorBuffer;//纹理坐标数据缓冲
    
    void initVertexData();
    void initTextureData();
    void initShader(int index, const string &vartexName);
    
public:
    TextureRect();
    void drawSelf(int texId, float currStartAngle);
};


#endif
