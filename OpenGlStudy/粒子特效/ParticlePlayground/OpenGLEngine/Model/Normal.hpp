//
//  Normal.hpp
//  ParticlePlayground
//
//  Created by 苏莫离 on 2019/5/15.
//

#ifndef Normal_hpp
#define Normal_hpp

#include <iostream>
#import <OpenGLES/ES3/gl.h>
#include "ShaderTool.h"

using namespace std;
class Normal {
    float nx;
    float ny;
    float nz;
public:
    static float DIFF;
    Normal(float nx, float ny, float nz) {
        this -> nx = nx;
        this -> ny = ny;
        this -> nz = nz;
    }
    
    bool equals(Normal *other) {
        if (typeid(other) != typeid(Normal)) return false;
        return (abs(other -> nx - this -> nx) < DIFF &&
                abs(other -> ny - this -> ny) < DIFF &&
                abs(other -> nz - this -> nz) < DIFF);
    }

    static vector_float3 getAverage(vector_float3 *sn, int count) {
        vector_float3 result = (vector_float3){0, 0, 0};
        for (int i = 0; i < count; i++) {
            result.x += sn[i].x;
            result.y += sn[i].y;
            result.z += sn[i].z;
        }
        return result;
    }
    
};

float Normal::DIFF = 0.0000001f;

#endif
