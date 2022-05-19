//
//  GLResourceManager.h
//  Ball
//
//  Created by 苏莫离 on 2019/10/17.
//

#ifndef GLResourceManager_hpp
#define GLResourceManager_hpp

#import <OpenGLES/ES3/gl.h>
#import <string>

using namespace std;

class GLResourceManager {
public:
    static GLuint initTexture(const string& name);
    static string loadShaderScript(const string& name);
    static string loadObjScript(const string& name);
};

#endif
