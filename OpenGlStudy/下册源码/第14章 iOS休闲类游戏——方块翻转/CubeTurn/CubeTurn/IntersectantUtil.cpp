//
//  IntersectantUtil.cpp
//  Sample14_4
//
//  Created by wuyafeng on 15/9/13.
//  Copyright (c) 2015年 wuyafeng. All rights reserved.
//

#include "IntersectantUtil.h"
#include "MatrixState.hpp"
float* IntersectantUtil::calculateABPosition
(
    float x,
    float y,
    float w,
    float h,
    float left,
    float top,
    float near,
    float far
){
    float x0=x-w/2;
    float y0=h/2-y;
    
    float xNear=2*x0*left/w;
    float yNear=2*y0*top/h;
    
    float ratio=far/near;
    float xFar=ratio*xNear;
    float yFar=ratio*yNear;
    
    float ax=xNear;
    float ay=yNear;
    float az=-near;
    
    float bx=xFar;
    float by=yFar;
    float bz=-far;
    
    float* p1 = new float[3];
    p1[0] = ax;
    p1[1] = ay;
    p1[2] = az;
    float* A = MatrixState::fromPtoPreP(p1);
    delete p1;
    float* p2 = new float[3];
    p2[0] = bx;
    p2[1] = by;
    p2[2] = bz;
    float* B = MatrixState::fromPtoPreP(p2);
    delete p2;
    float* result=new float[6];
    result[0]=A[0];
    result[1]=A[1];
    result[2]=A[2];
    result[3]=B[0];
    result[4]=B[1];
    result[5]=B[2];
    delete A;
    delete B;
    return result;
}