//
//  GLResourceManager.h
//  Mist
//
//  Created by 苏莫离 on 2019/10/17.
//

#ifndef GLResourceManager_hpp
#define GLResourceManager_hpp

#import <OpenGLES/ES3/gl.h>
#import <string>
#include <vector>

using namespace std;

class GLResourceManager {
private:
    constexpr const static float LAND_HIGH_ADJUST = 2.0f; //陆地的高度调整值
    constexpr const static float LAND_HIGHEST = 60.0f; //陆地最大高差
public:
    static GLuint initTexture(const string& name);
    static string loadShaderScript(const string& name);
    static string loadObjScript(const string& name);
    static vector<vector<float>> loadLandforms(const string& name);
};

#endif
