//
//  LoadUtil.h
//  Sample14_4
//
//  Created by wuyafeng on 15/9/11.
//  Copyright (c) 2015年 wuyafeng. All rights reserved.
//

#ifndef __Sample14_4__LoadUtil__
#define __Sample14_4__LoadUtil__

#include <stdio.h>

#include <string>
#include <vector>
#include "LoadedObjectVertexNormalTexture.h"

using namespace std;

class LoadUtil
{
public:
    static LoadedObjectVertexNormalTexture* loadFromFile(const string& fname,int flag);
    static size_t  splitString(const string& strSrc, const string& strDelims, vector<string>& strDest);
    static float parseFloat(const char *token);
    static bool tryParseDouble(const char *s, const char *s_end, double *result);
    static int parseInt(const char *token);
};

#endif /* defined(__Sample14_4__LoadUtil__) */
