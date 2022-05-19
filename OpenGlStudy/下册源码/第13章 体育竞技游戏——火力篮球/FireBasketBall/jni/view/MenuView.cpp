#include "view/MenuView.h"
#include "util/Constant.h"
#include "util/ShaderManager.h"
#include "util/MatrixState.h"

#include "view/GameView.h"
#include "FireBasketBallcpp/main.h"

MenuView::MenuView()
{
}
MenuView::~MenuView()
{

}

//被创建
void MenuView::onViewCreated(JNIEnv * env,jobject obj)
{
	//设置背景颜色
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	//初始化变换矩阵
    MatrixState::setInitStack();
}

//被改变
void MenuView::onViewchanged(int w, int h)
{
	//设置视口
    glViewport(0, 0, w, h);
    //计算宽长比
    ratio = (float) w/h;
    //打开背面剪裁
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}
//画自己
void MenuView::drawSelf(int MenuId)
{
    //打开背面剪裁
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
	//清缓存
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //调用此方法计算产生透视投影矩阵
    MatrixState::setProjectOrtho(-ratio, ratio, -1, 1, 4.0f, 100);
    //相机位置(0,3.15,14.9)目标位置是篮框(0,3.15,0)
    MatrixState::setCamera(0, 0,5, 0, 0,0, 0, 1, 0);

	MatrixState::pushMatrix();
	gameOverBgTextureRect->drawSelf(bgTexId);
	MatrixState::popMatrix();

	switch(MenuId)
	{
	case 0://Constant::MENU_STARTVIEW_START:
		drawGameStartMenu();
	break;

	case 1://Constant::MENU_GAMEVIEW_END:
		drawGameOverMenu();
	break;
	}
}
void MenuView::initTextureId(JNIEnv * env,jobject obj)
{
	jclass cl = env->FindClass("com/bn/bullet/GL2JNIView");
	jmethodID id = env->GetStaticMethodID(cl,"initTextureRepeat","(Landroid/opengl/GLSurfaceView;Ljava/lang/String;)I");
	//背景纹理Id
	jstring name = env->NewStringUTF("pic/background.png");
	bgTexId = env->CallStaticIntMethod(cl,id,obj,name);
	//再来一局纹理Id
	name = env->NewStringUTF("pic/retry.png");
	ZLYCTexId = env->CallStaticIntMethod(cl,id,obj,name);
	//退出纹理Id
	name = env->NewStringUTF("pic/shut.png");
	TCTextId = env->CallStaticIntMethod(cl,id,obj,name);
	//背景纹理Id
	name = env->NewStringUTF("pic/fanhuicaidan.png");
	FHCDTextId = env->CallStaticIntMethod(cl,id,obj,name);

	//开始游戏菜单纹理id
	name = env->NewStringUTF("pic/begin.png");
	KSYXTexId = env->CallStaticIntMethod(cl,id,obj,name);
	//设置菜单纹理id
	name = env->NewStringUTF("pic/shezhi.png");
	SZTexId = env->CallStaticIntMethod(cl,id,obj,name);
	//关于菜单纹理id
	name = env->NewStringUTF("pic/about1.png");
	GYTexId = env->CallStaticIntMethod(cl,id,obj,name);
	//帮助菜单纹理id
	name = env->NewStringUTF("pic/help1.png");
	BZTexId = env->CallStaticIntMethod(cl,id,obj,name);
	//记录菜单纹理id
	name = env->NewStringUTF("pic/jilu.png");
	JLTexId = env->CallStaticIntMethod(cl,id,obj,name);
	env->DeleteLocalRef(name);
}
void MenuView::initGameOverMenu()
{
	gameOverBgTextureRect = new TextureRect(Constant::RADIO,1.0,0,1,ShaderManager::getCommTextureShaderProgram(),NULL);//创建矩形形对象

	menuTextureRect = new TextureRect(Constant::MENU_WIDTH_HALF,Constant::MENU_HEIGHT_HALF,0,1,ShaderManager::getCommTextureShaderProgram(),NULL);//创建矩形形对象
}
//绘制游戏结束菜单
void MenuView::drawGameOverMenu()
{
    //开启混合
    glEnable(GL_BLEND);
    //设置混合因子
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	MatrixState::pushMatrix();
	MatrixState::translate(0,Constant::MENU_SPAN,0);
    if(Constant::isTouched[Constant::TOUCHED_ZLYJ_MENU])
    {
    	MatrixState::scale(Constant::MENU_SACLE_X,Constant::MENU_SACLE_Y,Constant::MENU_SACLE_Z);
    }
	menuTextureRect->drawSelf(ZLYCTexId);
	MatrixState::popMatrix();

	MatrixState::pushMatrix();
    if(Constant::isTouched[Constant::TOUCHED_FHCD_MENU])
    {
    	MatrixState::scale(Constant::MENU_SACLE_X,Constant::MENU_SACLE_Y,Constant::MENU_SACLE_Z);
    }
	menuTextureRect->drawSelf(FHCDTextId);
	MatrixState::popMatrix();

	MatrixState::pushMatrix();
	MatrixState::translate(0,-Constant::MENU_SPAN,0);
    if(Constant::isTouched[Constant::TOUCHED_TC_MENU])
    {
    	MatrixState::scale(Constant::MENU_SACLE_X,Constant::MENU_SACLE_Y,Constant::MENU_SACLE_Z);
    }
	menuTextureRect->drawSelf(TCTextId);
	MatrixState::popMatrix();
    //关闭混合
    glDisable(GL_BLEND);
}
//绘制游戏开始菜单
void MenuView::drawGameStartMenu()
{
    //开启混合
    glEnable(GL_BLEND);
    //设置混合因子
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//开始游戏菜单纹理id
    MatrixState::pushMatrix();
    MatrixState::translate(0,Constant::MENU_SPAN/2*2,0);
    if(Constant::isTouched[Constant::TOUCHED_KSYX_MENU])
    {
    	MatrixState::scale(Constant::MENU_SACLE_X,Constant::MENU_SACLE_Y,Constant::MENU_SACLE_Z);
    }
    menuTextureRect->drawSelf(KSYXTexId);
    MatrixState::popMatrix();

	//设置菜单纹理id
    MatrixState::pushMatrix();
    MatrixState::translate(0,Constant::MENU_SPAN/2*1,0);
    if(Constant::isTouched[Constant::TOUCHED_SZ_MENU])
    {
    	MatrixState::scale(Constant::MENU_SACLE_X,Constant::MENU_SACLE_Y,Constant::MENU_SACLE_Z);
    }

    menuTextureRect->drawSelf(SZTexId);
    MatrixState::popMatrix();

	//关于菜单纹理id
    MatrixState::pushMatrix();
    MatrixState::translate(0,Constant::MENU_SPAN/2*0,0);
    if(Constant::isTouched[Constant::TOUCHED_GY_MENU])
    {
    	MatrixState::scale(Constant::MENU_SACLE_X,Constant::MENU_SACLE_Y,Constant::MENU_SACLE_Z);
    }
    menuTextureRect->drawSelf(GYTexId);
    MatrixState::popMatrix();

	//帮助菜单纹理id
	MatrixState::pushMatrix();
	MatrixState::translate(0,-Constant::MENU_SPAN/2,0);
    if(Constant::isTouched[Constant::TOUCHED_BZ_MENU])
    {
    	MatrixState::scale(Constant::MENU_SACLE_X,Constant::MENU_SACLE_Y,Constant::MENU_SACLE_Z);
    }
	menuTextureRect->drawSelf(BZTexId);
	MatrixState::popMatrix();

	//记录菜单纹理id
    MatrixState::pushMatrix();
    MatrixState::translate(0,-Constant::MENU_SPAN/2*2,0);
    if(Constant::isTouched[Constant::TOUCHED_JL_MENU])
    {
    	MatrixState::scale(Constant::MENU_SACLE_X,Constant::MENU_SACLE_Y,Constant::MENU_SACLE_Z);
    }
    menuTextureRect->drawSelf(JLTexId);
    MatrixState::popMatrix();


    MatrixState::pushMatrix();
    MatrixState::translate(0,-Constant::MENU_SPAN/2*3,0);
    if(Constant::isTouched[Constant::TOUCHED_TC_MENU])
    {
    	MatrixState::scale(Constant::MENU_SACLE_X,Constant::MENU_SACLE_Y,Constant::MENU_SACLE_Z);
    }
    menuTextureRect->drawSelf(TCTextId);
    MatrixState::popMatrix();

    //关闭混合
    glDisable(GL_BLEND);
}

void MenuView::onTouchBegan(float touchX,float touchY)
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
void MenuView::onTouchMoved(float touchX,float touchY){}
void MenuView::onTouchEnded(float touchX,float touchY)
{
	//屏幕x坐标到视口x坐标
	float tx = Constant::fromScreenXToNearX(touchX);
	//屏幕y坐标到视口y坐标
	float ty = Constant::fromScreenYToNearY(touchY);
	int touchedIndex = judgeMenuTouched(tx,ty);
	//如果触摸开始和触摸抬起是同一个菜单---则执行该菜单的功能--切换界面
	if(Constant::TOUCHED_CURR_MENU == touchedIndex)
	{
		switch(Constant::TOUCHED_CURR_MENU)
		{
		case 1://点击开始游戏执行的代码
			Constant::SWAP_VIEW_FLAG = true;
			if(Constant::isOpen_BJYY)
			{
				main::playBackmusic(main::envMain,main::objMain,(jint)1);
			}else
			{
				main::playBackmusic(main::envMain,main::objMain,(jint)0);
			}

			Constant::VIEW_CUR_GL_INDEX = Constant::VIEW_GAME_INDEX;
			main::gameView->onViewchanged(Constant::w,Constant::h);
			main::gameView->initGameViewData();
			Constant::SWAP_VIEW_FLAG = false;
			break;
		case 2://点击设置执行的代码
			Constant::SWAP_VIEW_FLAG = true;
			Constant::VIEW_CUR_GL_INDEX = Constant::VIEW_SET_INDEX;
			main::setView->onViewchanged(Constant::w,Constant::h);
			Constant::SWAP_VIEW_FLAG = false;
			break;
		case 3://点击关于执行的代码
			Constant::SWAP_VIEW_FLAG = true;
			Constant::VIEW_CUR_GL_INDEX = Constant::VIEW_ABOUT_INDEX;
			main::aboutView->onViewchanged(Constant::w,Constant::h);
			Constant::SWAP_VIEW_FLAG = false;
			break;
		case 4://点击帮助执行的代码
			Constant::SWAP_VIEW_FLAG = true;
			Constant::VIEW_CUR_GL_INDEX = Constant::VIEW_GAME_INDEX;
			main::gameView->onViewchanged(Constant::w,Constant::h);
			main::gameView->initGameViewData();
			Constant::isnoHelpView = true;
			Constant::SWAP_VIEW_FLAG = false;
			break;
		case 5://点击记录执行的代码
			Constant::SWAP_VIEW_FLAG = true;
			Constant::VIEW_CUR_GL_INDEX = Constant::VIEW_RECORD_INDEX;
			main::recordView->onViewchanged(Constant::w,Constant::h);
			main::recordView->calTableResult();
			Constant::SWAP_VIEW_FLAG = false;
			break;
		case 6://点击退出执行的代码
			Constant::SWAP_VIEW_FLAG = true;
			main::NDKCallExit(main::envMain,main::objMain);
			Constant::SWAP_VIEW_FLAG = false;
			break;
		case 7://点击再来一局执行的代码
			//设置菜单界面
			Constant::SWAP_VIEW_FLAG = true;
			Constant::VIEW_CUR_GL_INDEX = Constant::VIEW_GAME_INDEX;
			main::gameView->onViewchanged(Constant::w,Constant::h);
			main::gameView->initGameViewData();
			Constant::SWAP_VIEW_FLAG = false;
			break;
		case 8://点击返回菜单执行的代码
			Constant::SWAP_VIEW_FLAG = true;
			//设置菜单界面
			Constant::VIEW_CUR_GL_INDEX = Constant::VIEW_MENU_INDEX;
			//主菜单界面
			Constant::MENU_CURR_MENUINDEX = Constant::MENU_STARTVIEW_START;
			main::menuView->onViewchanged(Constant::w,Constant::h);
			Constant::SWAP_VIEW_FLAG = false;
			break;
		}
		//重置初始值，表明未点击任何菜单
		Constant::TOUCHED_CURR_MENU = Constant::TOUCHED_NONE_MENU;
		main::sendViewIndex(main::envMain,main::objMain,Constant::VIEW_CUR_GL_INDEX);
	}

	//重置菜单项都未被选中
	for(int i=0;i<15;i++)
	{
		Constant::isTouched[i] = false;
	}
}
int MenuView::judgeMenuTouched(float x,float y)
{
	if(Constant::MENU_CURR_MENUINDEX == Constant::MENU_GAMEVIEW_END)//如果当前需要显示游戏结束菜单
	{
		if(x > -Constant::MENU_WIDTH_HALF&&x<Constant::MENU_WIDTH_HALF)
		{
			for(int i=-1;i<2;i++)
			{
				//摸中再来一次菜单
				if(y>(Constant::MENU_SPAN*(-i)-Constant::MENU_HEIGHT_HALF)
						&&y<(Constant::MENU_HEIGHT_HALF+(Constant::MENU_SPAN*(-i))))
				{
					//返回再来一局菜单被动点中
					switch(i)
					{
					case 1:return Constant::TOUCHED_TC_MENU;
					case 0:return Constant::TOUCHED_FHCD_MENU;
					case -1:return Constant::TOUCHED_ZLYJ_MENU;
					}
				}
			}
		}
	}else if(Constant::MENU_CURR_MENUINDEX == Constant::MENU_STARTVIEW_START)//如果当前需要显示游戏开始菜单
	{
		if(x > -Constant::MENU_WIDTH_HALF&&x<Constant::MENU_WIDTH_HALF)
		{
			for(int i=-2;i<4;i++)
			{
				//摸中开始游戏菜单
				if(y>(Constant::MENU_SPAN/2*(-i)-Constant::MENU_HEIGHT_HALF)
						&&y<(Constant::MENU_HEIGHT_HALF+(Constant::MENU_SPAN/2*(-i))))
				{
					//返回再来一局菜单被动点中
					switch(i)
					{
					case -2:return Constant::TOUCHED_KSYX_MENU;
					case -1:return Constant::TOUCHED_SZ_MENU;
					case 0:return Constant::TOUCHED_GY_MENU;
					case 1:return Constant::TOUCHED_BZ_MENU;
					case 2:return Constant::TOUCHED_JL_MENU;
					case 3:return Constant::TOUCHED_TC_MENU;
					}
				}
			}
		}
	}
	//未点中
	return Constant::TOUCHED_NONE_MENU;
}

