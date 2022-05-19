//
//  IntersectantUtil.h
//  Sample14_4
//
//  Created by wuyafeng on 15/9/13.
//  Copyright (c) 2015年 wuyafeng. All rights reserved.
//

#ifndef __Sample14_4__IntersectantUtil__
#define __Sample14_4__IntersectantUtil__

#include <stdio.h>

class IntersectantUtil{
    
    public:
    static float* calculateABPosition(float x,float y,float w,float h,
                               float left,float top,float near,float far);
    
};

#endif /* defined(__Sample14_4__IntersectantUtil__) */
