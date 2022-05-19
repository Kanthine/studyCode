//
//  GLMatrixState.cpp
//  Ball
//
//  Created by 苏莫离 on 2019/10/17.
//

#include "GLMatrixState.hpp"


float GLMatrixState::currMatrix[16];
float GLMatrixState::mProjMatrix[16];
float GLMatrixState::mVMatrix[16];
float GLMatrixState::mMVPMatrix[16];
float GLMatrixState::mStack[10][16];
float GLMatrixState::lightLocation[3];//太阳定位光光源位置

GLfloat* GLMatrixState::cameraFB = NULL;
GLfloat* GLMatrixState::lightPositionFB = NULL;
int GLMatrixState::stackTop = -1;
