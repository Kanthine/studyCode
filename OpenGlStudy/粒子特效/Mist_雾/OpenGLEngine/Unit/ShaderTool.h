//
//  ShaderTool.h
//  Mist
//
//  Created by 苏莫离 on 2019/10/17.
//

#ifndef ShaderTool_h
#define ShaderTool_h

#import <OpenGLES/ES3/gl.h>
#import "GLResourceManager.h"

using namespace std;

class ShaderTool {
    
public:
    static GLuint createProgram(const string& vname,const string& fname) {
        string vertexShaderSource = GLResourceManager::loadShaderScript(vname);
        string fragmentShaderSource = GLResourceManager::loadShaderScript(fname);
        
        GLuint vertexShader = loadShader(vertexShaderSource.c_str(), GL_VERTEX_SHADER);//加载顶点着色器
        GLuint fragmentShader = loadShader(fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);//加载片元着色器
        GLuint programHandle = glCreateProgram();//创建程序
        glAttachShader(programHandle, vertexShader);//向程序中加入顶点着色器
        glAttachShader(programHandle, fragmentShader);//向程序中加入片元着色器
        glLinkProgram(programHandle);//链接程序
        GLint linkSuccess;//存放链接成功program数量的变量
        glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);//获取program的链接情况
        //若链接失败则报错并删除程序
        if (linkSuccess == GL_FALSE) {
            GLchar messages[256];
            glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
            std::cout<<"MSG>>" << messages;
            //exit(1);
        }
        return programHandle;
    }
    
    static GLuint loadShader(const char* source, GLenum shaderType) {
         //创建一个新shader
        GLuint shaderHandle = glCreateShader(shaderType);
        //加载shader的源代码
        glShaderSource(shaderHandle, 1, &source, 0);
        //编译shader
        glCompileShader(shaderHandle);
        //把error信息输出到屏幕
        GLint compileSuccess;
        glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
        if (compileSuccess == GL_FALSE) {
            GLchar messages[256];
            glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
            std::cout << messages;
            exit(1);//退出
        }
        return shaderHandle;
    }
};


#endif
