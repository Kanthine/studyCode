#ifndef ChooseLevelView_hpp
#define ChooseLevelView_hpp

#include <stdio.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#include "BN2DObject.h"

class ChooseLevelView{
public:
    ChooseLevelView();
    void Initialize(int width, int height);
    void Render() ;
    void OnFingerDown(float locationx,float locationy);
    ~ChooseLevelView();
private:
    BN2DObject* chooseLevel;//选关绘制者
    BN2DObject* first;
    BN2DObject* sec;
    BN2DObject* third;
    BN2DObject* four;
    BN2DObject* back;
    
    float CHOOSE_WIDTH=0.8f;//选择关卡大纹理的宽度
    float CHOOSE_HEIGHT=0.25f;//选择关卡大纹理的高度
    
    float FIRST_WIDTH=0.8f;//关卡按钮的宽度
    float FIRST_HEIGHT=0.2f;
    
    float span = 0.3f;
    float CHOOSE_Y = 0.7f;//选择关卡的y绘制坐标
    float FIRST_Y = CHOOSE_Y-span;//第一关卡的y绘制坐标
    float SEC_Y = FIRST_Y-span;//第二关卡的y绘制坐标
    float THI_Y = SEC_Y-span;//第三关卡的y绘制坐标
    float FOU_Y = THI_Y-span;//第四关卡的y绘制坐标
    float BACK_Y = FOU_Y-span;//返回的y绘制坐标
    
    float BACK_MIN_X=-0.25f;//返回按钮最小x坐标
    
    void drawChoose();
};
#endif /* ChooseLevelView_hpp */
