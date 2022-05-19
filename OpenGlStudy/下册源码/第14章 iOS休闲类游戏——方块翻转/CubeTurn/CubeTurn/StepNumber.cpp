#include "StepNumber.h"
#include "MatrixState2D.hpp"
#include <string>
#include <sstream>
using namespace std;

StepNumber::StepNumber(int numberBlueTexId,int numberRedTexId,float width,float height)
{   
    for(int i=0;i<10;i++)
    {
    
        numberBlues[i] = new BN2DObject
        (
         true,
         i,
         numberBlueTexId,
         width,
         height
         );
        numberReds[i] = new BN2DObject
        (
         true,
         i,
         numberRedTexId,
         width,
         height
         );
    }
}

StepNumber::~StepNumber()
{
    
}

void StepNumber::drawSelf(int BEST_COUNT,int countDrawTrans,float width)
{
    string scoreStr=intToString(countDrawTrans);
    const char *scoreChar=scoreStr.c_str();
    if(countDrawTrans<=BEST_COUNT)
    {
        drawNumber(scoreChar,scoreChar,numberBlues,width);
    }else
    {
        drawNumber(scoreChar,scoreChar,numberReds,width);
    }
}
void StepNumber::drawShortStep(int best_count,float width)
{
    string scoreStr=intToString(best_count);
    const char *scoreChar=scoreStr.c_str();
    drawNumber(scoreChar,scoreChar,numberBlues,width);
}

void StepNumber::drawNumber(const char* tp,const string& str,map<int, BN2DObject*> temp,float width)
{
    glDisable(GL_DEPTH_TEST);//关闭深度检测
    for(int i=0;i<str.length();i++)
    {
        char c=tp[i];
        MatrixState2D::pushMatrix();
        float translateSpan=i*width*1.0f;
        if(str.length()>1){
            translateSpan=i*width*0.5f-0.03f;
        }
        MatrixState2D::translate(translateSpan, 0, 0.0f);
        temp[(c-'0')]->drawSelf(0,0,1.0f);
        MatrixState2D::popMatrix();
    }
    glEnable(GL_DEPTH_TEST);//打开深度检测
}

string StepNumber::intToString(const int &int_num)
{
    stringstream sstream;
    sstream<<int_num;
    return sstream.str();
}