#include "view/AboutView.h"
#include "util/MatrixState.h"
#include "util/ShaderManager.h"
#include <jni.h>
#include "util/Constant.h"

#include "FireBasketBallcpp/main.h"

AboutView::AboutView() {
}
AboutView::~AboutView() {
}
//被创建
void AboutView::onViewCreated(JNIEnv * env, jobject obj) {
	//设置背景颜色
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	//初始化变换矩阵
	MatrixState::setInitStack();
}

//被改变
void AboutView::onViewchanged(int w, int h) {
	//设置视口
	glViewport(0, 0, w, h);
	//计算宽长比
	ratio = (float) w / h;
	//打开背面剪裁
	glEnable(GL_CULL_FACE);
//    glEnable(GL_DEPTH_TEST);
//    glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

}
//画自己
void AboutView::drawSelf() {
	//清缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//调用此方法计算产生透视投影矩阵
//	MatrixState::setProjectFrustum(-ratio, ratio, -1, 1, 4.0f, 100);
	MatrixState::setProjectOrtho(-ratio, ratio, -1, 1, 4.0f, 100);
	//相机位置(0,3.15,14.9)目标位置是篮框(0,3.15,0)
	MatrixState::setCamera(0, 0, 5, 0, 0, 0, 0, 1, 0);
	//开启混合
	glEnable(GL_BLEND);
	//设置混合因子
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	MatrixState::pushMatrix();
	bgTextureRect->drawSelf(bgTexId);
	MatrixState::popMatrix();

	MatrixState::pushMatrix();
	aboutTextureRect->drawSelf(aboutTexId);
	MatrixState::popMatrix();

	MatrixState::pushMatrix();
	MatrixState::translate(-0.3, -0.7, 0);
	if (Constant::isTouched[Constant::TOUCHED_QD_MENU]) {
		MatrixState::scale(Constant::MENU_SACLE_X, Constant::MENU_SACLE_Y,
				Constant::MENU_SACLE_Z);
	}
	menuTextureRect->drawSelf(QDMenuTextId);
	MatrixState::popMatrix();

	MatrixState::pushMatrix();
	MatrixState::translate(0.3, -0.7, 0);
	if (Constant::isTouched[Constant::TOUCHED_FH_MENU]) {
		MatrixState::scale(Constant::MENU_SACLE_X, Constant::MENU_SACLE_Y,
				Constant::MENU_SACLE_Z);
	}
	menuTextureRect->drawSelf(FHMenuTextId);
	MatrixState::popMatrix();

	//开启混合
	glDisable(GL_BLEND);
}
//初始化纹理ID
void AboutView::initTextureId(JNIEnv * env, jobject obj) {
	jclass cl = env->FindClass("com/bn/bullet/GL2JNIView");

	jmethodID id = env->GetStaticMethodID(cl,"initTextureRepeat","(Landroid/opengl/GLSurfaceView;Ljava/lang/String;)I");
	//背景纹理Id
	jstring name = env->NewStringUTF("pic/background.png");
	bgTexId = env->CallStaticIntMethod(cl, id, main::g_obj, name);

	//关于信息纹理Id
	name = env->NewStringUTF("pic/guanyuwenzitu.png");
	aboutTexId = env->CallStaticIntMethod(cl, id, main::g_obj, name);
	//确定菜单纹理Id
	name = env->NewStringUTF("pic/quedinganniu.png");
	QDMenuTextId = env->CallStaticIntMethod(cl, id, main::g_obj, name);
	//返回菜单纹理Id
	name = env->NewStringUTF("pic/fanhuianniu.png");
	FHMenuTextId = env->CallStaticIntMethod(cl, id, main::g_obj, name);
}
//初始化纹理矩形
void AboutView::initTextureRect() {
	bgTextureRect = new TextureRect(Constant::RADIO,1.0, 0, 1,
			ShaderManager::getCommTextureShaderProgram(), NULL); //创建矩形形对象

	aboutTextureRect = new TextureRect(0.5, 0.5, 0, 1,
			ShaderManager::getCommTextureShaderProgram(), NULL); //创建矩形形对象

}
//初始化菜单
void AboutView::initMenu() {
	menuTextureRect = new TextureRect(Constant::MENU_QD_FH_WIDTH_HALF,
			Constant::MENU_QD_FH_HEIGHT_HALF, 0, 1,
			ShaderManager::getCommTextureShaderProgram(), NULL); //创建矩形形对象
}
void AboutView::onTouchBegan(float touchX, float touchY) {
	//屏幕x坐标到视口x坐标
	float tx = Constant::fromScreenXToNearX(touchX);
	//屏幕y坐标到视口y坐标
	float ty = Constant::fromScreenYToNearY(touchY);
	//获取点击的菜单编号
	int touchedIndex = judgeMenuTouched(tx, ty);
	//记录点击菜单的编号
	Constant::TOUCHED_CURR_MENU = touchedIndex;
	//菜单项被选中
	Constant::isTouched[Constant::TOUCHED_CURR_MENU] = true;
}
void AboutView::onTouchMoved(float touchX, float touchY) {
}
void AboutView::onTouchEnded(float touchX, float touchY) {\
	//屏幕x坐标到视口x坐标
	float tx = Constant::fromScreenXToNearX(touchX);
	//屏幕y坐标到视口y坐标
	float ty = Constant::fromScreenYToNearY(touchY);
	//获取点击的菜单编号
	int touchedIndex = judgeMenuTouched(tx, ty);
	//如果触摸开始和触摸抬起是同一个菜单---则执行该菜单的功能--切换界面
	if (Constant::TOUCHED_CURR_MENU == touchedIndex) {
		switch (Constant::TOUCHED_CURR_MENU) {
		case 9: //确定按钮被按下
		case 10:
			Constant::SWAP_VIEW_FLAG = true;
			Constant::VIEW_CUR_GL_INDEX = Constant::VIEW_MENU_INDEX;
			main::menuView->onViewchanged(Constant::w, Constant::h);
			Constant::SWAP_VIEW_FLAG = false;
			break; //返回按钮被按下
		}
		//重置初始值，表明未点击任何菜单
		Constant::TOUCHED_CURR_MENU = Constant::TOUCHED_NONE_MENU;
	}
	//重置菜单项都未被选中
	for (int i = 0; i < 15; i++) {
		Constant::isTouched[i] = false;
	}
}
int AboutView::judgeMenuTouched(float x, float y) {
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
	//没有选中按钮
	return Constant::TOUCHED_NONE_MENU;
}
