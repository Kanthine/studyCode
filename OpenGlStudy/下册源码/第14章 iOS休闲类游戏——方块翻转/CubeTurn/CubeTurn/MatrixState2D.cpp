//
//  MatrixState2D.cpp
//  Sample14_4
//
//  Created by wuyafeng on 15/9/17.
//  Copyright (c) 2015年 wuyafeng. All rights reserved.
//

#include "MatrixState2D.hpp"


float MatrixState2D::currMatrix[16];
float MatrixState2D::mProjMatrix[16];
float MatrixState2D::mVMatrix[16];
float MatrixState2D::mMVPMatrix[16];
float MatrixState2D::mStack[10][16];
float MatrixState2D::lightLocationSun[3];//太阳定位光光源位置

GLfloat* MatrixState2D::cameraFB;
GLfloat* MatrixState2D::lightPositionFBSun;
int MatrixState2D::stackTop=-1;
