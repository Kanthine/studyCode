#ifndef __Sample14_4__StepNumber__
#define __Sample14_4__StepNumber__
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#include <stdio.h>
#include "BN2DObject.h"
#include <map>
#include <vector>

using namespace std;

class StepNumber
{
private:
    
public :    
    map<int, BN2DObject*>  numberBlues;
    map<int, BN2DObject*>  numberReds;
    
    
    int numberBlueTexId;
    int numberRedTexId;
    constexpr const static float ICON_WIDTH=0.18f;
    constexpr const static float ICON_HEIGHT=0.18f;
    
    StepNumber(int numberBlueTexId,int numberRedTexId,float width,float height);
    void drawSelf(int best_count,int count,float width);
    void drawShortStep(int best_count,float width);
    void drawNumber(const char* tp,const string& str,map<int, BN2DObject*> temp,float width);
    string intToString(const int& int_num);
    void init();
    ~StepNumber();
    
};

#endif /* defined(__Sample14_4__StepNumber__) */
