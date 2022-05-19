#include "view/SetView.h"

#include "util/Constant.h"
#include "util/ShaderManager.h"

#include "util/MatrixState.h"

#include "FireBasketBallcpp/main.h"

SetView::SetView()
{

}
SetView::~SetView()
{

}
//被创建
void SetView::onViewCreated(JNIEnv * env,jobject obj)
{
	//设置背景颜色
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	//初始化变换矩阵
    MatrixState::setInitStack();
	//这里编译3D场景中欢迎欢迎界面中的shader
//	ShaderManager::compileShader();
    //初始化纹理ID
//    initTextureId(env,obj);
    //初始化纹理矩形
//    initTextureRect();
}
//被改变
void SetView::onViewchanged(int w, int h)
{
	//设置视口
    glViewport(0, 0, w, h);
    //计算宽长比
    ratio = (float) w/h;
    //打开背面剪裁
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}
//画自己---传入需要绘制的菜单Id，根据id绘制对应的菜单
void SetView::drawSelf()
{
    //打开背面剪裁
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
	//清缓存
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //调用此方法计算产生透视投影矩阵
//    MatrixState::setProjectFrustum(-ratio, ratio, -1, 1, 4.0f, 100);
    MatrixState::setProjectOrtho(-ratio, ratio, -1, 1, 4.0f, 100);
    //相机位置(0,3.15,14.9)目标位置是篮框(0,3.15,0)
    MatrixState::setCamera(0, 0,5, 0, 0,0, 0, 1, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    MatrixState::pushMatrix();
    bgTextureRect->drawSelf(bgTexId);
    MatrixState::popMatrix();

    MatrixState::pushMatrix();
    MatrixState::translate(-Constant::MENU_HEIGHT_HALF*2,-Constant::MENU_HEIGHT_HALF*2,0);
    infoTextureRect->drawSelf(BJYY_TexId);
    MatrixState::popMatrix();

    MatrixState::pushMatrix();
    MatrixState::translate(-Constant::MENU_HEIGHT_HALF*2,Constant::MENU_HEIGHT_HALF*2,0);
    infoTextureRect->drawSelf(YXYY_TexId);
    MatrixState::popMatrix();

    MatrixState::pushMatrix();
    MatrixState::translate(-0.3,-0.7,0);
    if(Constant::isTouched[Constant::TOUCHED_QD_MENU])
    {
    	MatrixState::scale(Constant::MENU_SACLE_X,Constant::MENU_SACLE_Y,Constant::MENU_SACLE_Z);
    }
    menuTextureRect->drawSelf(menu_QD_TexId);
    MatrixState::popMatrix();

    MatrixState::pushMatrix();
    MatrixState::translate(0.3,-0.7,0);
    if(Constant::isTouched[Constant::TOUCHED_FH_MENU])
    {
    	MatrixState::scale(Constant::MENU_SACLE_X,Constant::MENU_SACLE_Y,Constant::MENU_SACLE_Z);
    }
    menuTextureRect->drawSelf(menu_FH_TexId);
    MatrixState::popMatrix();

    if(Constant::isOpen_BJYY)
    {
        MatrixState::pushMatrix();
        MatrixState::translate(Constant::MENU_HEIGHT_HALF*5,-Constant::MENU_HEIGHT_HALF*2,0);
        kuangTextureRect->drawSelf(KUANG_DUIGOU_TexId);
        MatrixState::popMatrix();
    }

    MatrixState::pushMatrix();
    MatrixState::translate(Constant::MENU_HEIGHT_HALF*5,-Constant::MENU_HEIGHT_HALF*2,0);
    kuangTextureRect->drawSelf(menu_KUANG_TexId);
    MatrixState::popMatrix();

    if(Constant::isOpen_CJYY)
    {
        MatrixState::pushMatrix();
        MatrixState::translate(Constant::MENU_HEIGHT_HALF*5,Constant::MENU_HEIGHT_HALF*2,0);
        kuangTextureRect->drawSelf(KUANG_DUIGOU_TexId);
        MatrixState::popMatrix();
    }

    MatrixState::pushMatrix();
    MatrixState::translate(Constant::MENU_HEIGHT_HALF*5,Constant::MENU_HEIGHT_HALF*2,0);
    kuangTextureRect->drawSelf(menu_KUANG_TexId);
    MatrixState::popMatrix();

    //关闭混合
    glDisable(GL_BLEND);

}
void SetView::initTextureId(JNIEnv * env,jobject obj)
{
	jclass cl = env->FindClass("com/bn/bullet/GL2JNIView");
	jmethodID id = env->GetStaticMethodID(cl,"initTextureRepeat","(Landroid/opengl/GLSurfaceView;Ljava/lang/String;)I");
	//背景纹理Id
	jstring name = env->NewStringUTF("pic/background.png");
	bgTexId = env->CallStaticIntMethod(cl,id,obj,name);
	//背景音乐纹理Id
	name = env->NewStringUTF("pic/beijingyinyu.png");
	BJYY_TexId = env->CallStaticIntMethod(cl,id,obj,name);
	//音效音乐纹理Id
	name = env->NewStringUTF("pic/changjinyinyu.png");
	YXYY_TexId = env->CallStaticIntMethod(cl,id,obj,name);
	//确定纹理Id
	name = env->NewStringUTF("pic/quedinganniu.png");
	menu_QD_TexId = env->CallStaticIntMethod(cl,id,obj,name);
	//返回纹理Id
	name = env->NewStringUTF("pic/fanhuianniu.png");
	menu_FH_TexId = env->CallStaticIntMethod(cl,id,obj,name);

	//返回纹理Id
	name = env->NewStringUTF("pic/baisefangge.png");
	menu_KUANG_TexId = env->CallStaticIntMethod(cl,id,obj,name);

	//对勾纹理Id
	name = env->NewStringUTF("pic/honsegou.png");
	KUANG_DUIGOU_TexId = env->CallStaticIntMethod(cl,id,obj,name);

}
//初始化纹理矩形
void SetView::initTextureRect()
{
	//背景纹理矩形
	bgTextureRect = new TextureRect(Constant::RADIO,1.0,0,1,ShaderManager::getCommTextureShaderProgram(),NULL);//创建矩形形对象
	//背景音乐、音效音乐纹理矩形
	infoTextureRect = new TextureRect(Constant::MENU_WIDTH_HALF,Constant::MENU_HEIGHT_HALF,0,1,ShaderManager::getCommTextureShaderProgram(),NULL);//创建矩形形对象
	//确定、取消纹理矩形
	menuTextureRect = new TextureRect(Constant::MENU_QD_FH_WIDTH_HALF,Constant::MENU_QD_FH_HEIGHT_HALF,0,1,ShaderManager::getCommTextureShaderProgram(),NULL);//创建矩形形对象

	kuangTextureRect = new TextureRect(Constant::MENU_KUANG_WIDTH_HALF,Constant::MENU_KUANG_HEIGHT_HALF,0,1,ShaderManager::getCommTextureShaderProgram(),NULL);//创建矩形形对象
}
//触摸开始被回调
void SetView::onTouchBegan(float touchX,float touchY)
{
	//屏幕x坐标到视口x坐标
	float tx = Constant::fromScreenXToNearX(touchX);
	//屏幕y坐标到视口y坐标
	float ty = Constant::fromScreenYToNearY(touchY);
	//获取点击的菜单编号
	int touchedIndex = judgeMenuTouched(tx,ty);
	//记录点击菜单的编号
	Constant::TOUCHED_CURR_MENU = touchedIndex;
	//菜单项被选中
	Constant::isTouched[Constant::TOUCHED_CURR_MENU] = true;
}
//触摸移动被回调
void SetView::onTouchMoved(float touchX,float touchY)
{

}
//触摸移动被回调
void SetView::onTouchEnded(float touchX,float touchY)
{
	//屏幕x坐标到视口x坐标
	float tx = Constant::fromScreenXToNearX(touchX);
	//屏幕y坐标到视口y坐标
	float ty = Constant::fromScreenYToNearY(touchY);
	//获取点击的菜单编号
	int touchedIndex = judgeMenuTouched(tx,ty);
	if(Constant::TOUCHED_CURR_MENU == touchedIndex)
	{
		switch(Constant::TOUCHED_CURR_MENU)
		{
		case 9: //点击确定菜单执行的代码
		case 10: //点击返回菜单执行的代码
			Constant::SWAP_VIEW_FLAG = true;
			Constant::VIEW_CUR_GL_INDEX = Constant::VIEW_MENU_INDEX;
			main::menuView->onViewchanged(Constant::w,Constant::h);
			Constant::SWAP_VIEW_FLAG = false;

			break;
		case 13: Constant::isOpen_CJYY = !Constant::isOpen_CJYY;//点击场景音乐框菜单执行的代码
			break;
		case 14:
			Constant::isOpen_BJYY = !Constant::isOpen_BJYY;//点击背景音乐框菜单执行的代码

			break;
		}
	}
	//重置菜单项都未被选中
	for(int i=0;i<15;i++)
	{
		Constant::isTouched[i] = false;
	}
}

int SetView::judgeMenuTouched(float x,float y)
{
	if(y<(Constant::MENU_QD_FH_HEIGHT_HALF-0.7)
			&&y>(-Constant::MENU_QD_FH_HEIGHT_HALF-0.7))
	{
		for(int i=-1;i<2;i++)
		{
			if(i==0)
			{
				continue;
			}
			if(x<(Constant::MENU_QD_FH_WIDTH_HALF-0.3*(-i))
					&&x>(-Constant::MENU_QD_FH_WIDTH_HALF-0.3*(-i)))
			{
				switch(i)
				{
				case -1:
					return Constant::TOUCHED_QD_MENU;
				case 1:
					return Constant::TOUCHED_FH_MENU;
				}
			}
		}
	}
	if(x>(Constant::MENU_HEIGHT_HALF*5-Constant::MENU_KUANG_WIDTH_HALF)
			&&x<(Constant::MENU_HEIGHT_HALF*5+Constant::MENU_KUANG_WIDTH_HALF))
	{
		for(int i=-1;i<2;i++)
		{
			if(i == 0)
			{
				continue;
			}
			if(y>(Constant::MENU_HEIGHT_HALF*2*i-Constant::MENU_KUANG_HEIGHT_HALF)&&
					y<(Constant::MENU_HEIGHT_HALF*2*i+Constant::MENU_KUANG_HEIGHT_HALF)
			)
			{
				switch(i)
				{
				case -1:
					return Constant::TOUCH_BJYY_KUANG_MENU;
				case 1:
					return Constant::TOUCH_CJYY_KUANG_MENU;
				}
			}
		}
	}
	//没有选中按钮
	return Constant::TOUCHED_NONE_MENU;
}
