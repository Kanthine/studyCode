#include "AboutView.hpp"
#include "LoadResourceUtil.h"
#include "MatrixState2D.hpp"
AboutView::AboutView(){}

void AboutView::Initialize(int width, int height)
{//初始化函数    
    back = new BN2DObject(false,0,LoadResourceUtil::initTexture("back2",false),BACK_WIDTH,BACK_HEIGHT);
    about = new BN2DObject(false,0,LoadResourceUtil::initTexture("about",false),ABOUT_WIDTH,ABOUT_HEIGHT);
    ground = new BN2DObject(false,0,LoadResourceUtil::initTexture("ground",false),BLACK_GAME_WIDTH,BLACK_GAME_HEIGHT);
}
void AboutView::Render() {
    MatrixState2D::pushMatrix();
    MatrixState2D::translate(0, 0, 0.5f);
    ground->drawSelf(0, -0.02f,1.0f);
    MatrixState2D::popMatrix();
    
    MatrixState2D::pushMatrix();
    MatrixState2D::translate(0, 0,1.0f);
    back->drawSelf(BACK_X, BACK_Y, 1.0F);
    MatrixState2D::popMatrix();
    
    
    MatrixState2D::pushMatrix();
    MatrixState2D::translate(0, 0, 1.0f);
    about->drawSelf(0, 0, 1.0F);
    MatrixState2D::popMatrix();
}
void AboutView::OnFingerDown(float x,float y)
{
    if(x>(BACK_X-BACK_WIDTH/2)&&x<(BACK_X+BACK_WIDTH/2)&&y>(BACK_Y-BACK_HEIGHT/2)&&y<(BACK_Y+BACK_HEIGHT/2))
    {//返回
        isDrawAbout=false;
    }
}
AboutView::~AboutView(){}

