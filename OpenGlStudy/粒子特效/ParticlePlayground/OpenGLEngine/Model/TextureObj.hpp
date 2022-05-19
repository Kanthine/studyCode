//
//  TextureObj.hpp
//  ParticlePlayground
//
//  Created by 苏莫离 on 2019/5/15.
//

#ifndef TextureObj_hpp
#define TextureObj_hpp

#include <iostream>
#import <OpenGLES/ES3/gl.h>

using namespace std;

class TextureObj {
    GLuint mProgram;          /// 自定义渲染管线 id
    GLuint muMVPMatrixHandle; /// 总变换矩阵引用
    GLuint maPositionHandle;  /// 顶点位置属性引用
    GLuint maTexCoorHandle;
    GLuint muMMatrixHandle;
    GLuint maLightLocationHandle;
    GLuint maCameraHandle;
        
    const GLvoid *mVertexBuffer;//顶点坐标数据缓冲
    const GLvoid *mTexCoorBuffer;//纹理坐标数据缓冲
    
    int vCount = 0;
    
    void initShader();
public:
    TextureObj(int vCount, float *vertices,float *normals,float *texCoors);
    void drawSelf(GLuint texId);
};



class TextureLoader {
private:
    static size_t splitString(const string& strSrc, const string& strDelims, vector<string>& strDest);
    static float parseFloat(const char *token);
    static bool tryParseDouble(const char *s, const char *s_end, double *result);
    static int parseInt(const char *token);
public:
    static TextureObj *loadTextureFromObjFile(const std::string &fname);
};

#endif
