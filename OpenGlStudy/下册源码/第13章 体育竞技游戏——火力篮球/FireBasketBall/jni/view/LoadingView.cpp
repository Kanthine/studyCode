#include "view/LoadingView.h"
#include "util/MatrixState.h"
#include "util/ShaderManager.h"

#include "FireBasketBallcpp/main.h"
#include "jni.h"
#include "util/FileUtil.h"

LoadingView::LoadingView() {
	//旋转的角度
	angle = 0;
	loadingFlag = true;
	loadTime = 0;

	isAttach = false;

	loadIndex = -1;
}
LoadingView::~LoadingView() {

}
//被创建
void LoadingView::onViewCreated(JNIEnv * env, jobject obj) {
	//设置背景颜色
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	//初始化变换矩阵
	MatrixState::setInitStack();
	//这里编译3D场景中欢迎欢迎界面中的shader
	ShaderManager::compileShader();
	//初始化纹理ID
	initTextureId(env, obj);
	//初始化纹理矩形
	initTextureRect();
}
//被改变
void LoadingView::onViewchanged(int w, int h) {
	//设置视口
	glViewport(0, 0, w, h);
	//计算宽长比
	ratio = (float) w / h;
	//打开背面剪裁
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}
//画自己
void LoadingView::drawSelf() //传入需要绘制的菜单Id，根据id绘制对应的菜单
{
	//清缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//调用此方法计算产生透视投影矩阵
	MatrixState::setProjectOrtho(-ratio, ratio, -1, 1, 4.0f, 100);
	//相机位置(0,3.15,14.9)目标位置是篮框(0,3.15,0)
	MatrixState::setCamera(0, 0, 5, 0, 0, 0, 0, 1, 0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	MatrixState::pushMatrix();
	bgTextureRect->drawSelf(bgTexId);
	MatrixState::popMatrix();

	MatrixState::pushMatrix();
	angle = angle + 20;
	MatrixState::rotate(angle, 0, 0, 1);
	XZTextureRect->drawSelf(XZTexId);
	MatrixState::popMatrix();

	if(loadingFlag == true)
	{
		intiAllAssats(main::envMain,main::objMain);
	}

	glDisable(GL_BLEND);
}
void LoadingView::threadTask() {
	while (true) {
		loadTime = loadTime + 10;
		if (loadTime >= 200) {
			loadingFlag = false;
			Constant::SWAP_VIEW_FLAG = true;
			Constant::VIEW_CUR_GL_INDEX = Constant::VIEW_MENU_INDEX;
			Constant::MENU_CURR_MENUINDEX = Constant::MENU_STARTVIEW_START;
			main::menuView->onViewchanged(Constant::w, Constant::h);
			Constant::SWAP_VIEW_FLAG = false;
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
#define RIGID_KUANG_OBJ 1//篮筐OBJ导入
#define SOFT_NET_OBJ 2//篮网OBJ导入
void LoadingView::intiAllAssats(JNIEnv* env, jobject obj) {
	loadingFlag = true;
	switch(loadIndex)
	{
	case 0:
		FileUtil::loadFromSDCardFile("/profitData/lankuang.obj");
		main::loadObjDataWd(
				(int)RIGID_KUANG_OBJ,
				main::copyFloats((float*)FileUtil::m_vertices,(int)FileUtil::numVertices),
				(int)FileUtil::numVertices,
				main::copyInts((int*)FileUtil::m_verticesIndices,(int)FileUtil::numVerticesIndex),
				(int)FileUtil::numVerticesIndex,
				main::copyFloats((float*)FileUtil::m_tex,(int)FileUtil::numTex),
				(int)FileUtil::numTex,
				main::copyFloats((float*)FileUtil::m_normal,(int)FileUtil::numNormal),
				(int)FileUtil::numNormal
			);
		break;
	case 1:
		main::menuView = new MenuView();
		break;
	case 2:
		main::menuView->onViewCreated(env, obj);
		break;
	case 3:
		main::menuView->initTextureId(env,obj);
		break;
	case 4:
		main::menuView->initGameOverMenu();
		break;
	case 5:
		main::setView = new SetView();
		break;
	case 6:
		main::setView->onViewCreated(env, obj);
		break;
	case 7:
		main::setView->initTextureId(env,obj);
		break;
	case 8:
		main::setView->initTextureRect();
		break;
	case 9:
		FileUtil::loadFromSDCardFile("/profitData/wang.obj");
		main::loadObjDataWd(
				(int)SOFT_NET_OBJ,
				main::copyFloats((float*)FileUtil::m_vertices,(int)FileUtil::numVertices),
				(int)FileUtil::numVertices,
				main::copyInts((int*)FileUtil::m_verticesIndices,(int)FileUtil::numVerticesIndex),
				(int)FileUtil::numVerticesIndex,
				main::copyFloats((float*)FileUtil::m_tex,(int)FileUtil::numTex),
				(int)FileUtil::numTex,
				main::copyFloats((float*)FileUtil::m_normal,(int)FileUtil::numNormal),
				(int)FileUtil::numNormal
				);
		break;
	case 10:
		main::recordView = new RecordView();
		break;
	case 11:
		main::recordView->onViewCreated(env, obj);
		break;
	case 12:
		main::recordView->initTextureId(env, obj);
		break;
	case 13:
		main::recordView->initTextureRect();
		break;
	case 14:
		main::gameView = new GameView();
		break;
	case 15:
		main::gameView->onViewCreated(env, obj);
		break;
	case 16:
	    //初始化纹理矩形
		main::gameView->initTextureRect();
	    break;
	case 17:
	    //初始化纹理Id
		main::gameView->initTextureId(env,obj);
	    break;
	case 18:
	    //初始化物理世界
		main::gameView->initPhysicalWorld();
	    break;
	case 19:
	    //初始化物理世界的物体
		main::gameView->initPhysicalWorldBody();
		break;
	case 20:
		main::aboutView = new AboutView();
		break;
	case 21:
		main::aboutView->onViewCreated(env, obj);
		break;
	case 22://初始化纹理ID
		main::aboutView->initTextureId(env, obj);
		break;
	case 23://初始化纹理矩形
		main::aboutView->initTextureRect();
		break;
	case 24:	//初始化菜单
		main::aboutView->initMenu();
		break;
	case 25:
		loadingFlag = false;
		Constant::SWAP_VIEW_FLAG = true;
		Constant::VIEW_CUR_GL_INDEX = Constant::VIEW_MENU_INDEX;
		Constant::MENU_CURR_MENUINDEX = Constant::MENU_STARTVIEW_START;
		main::menuView->onViewchanged(Constant::w, Constant::h);
		Constant::SWAP_VIEW_FLAG = false;
		break;
	}
	//加载数目增加
	loadIndex ++ ;
}
void LoadingView::initTextureId(JNIEnv * env, jobject obj) {
	jclass cl = env->FindClass("com/bn/bullet/GL2JNIView");
	jmethodID id = env->GetStaticMethodID(cl, "initTextureRepeat",
			"(Landroid/opengl/GLSurfaceView;Ljava/lang/String;)I");
	//背景纹理Id
	jstring name = env->NewStringUTF("pic/background.png");
	bgTexId = env->CallStaticIntMethod(cl, id, obj, name);
	//旋转纹理Id
	name = env->NewStringUTF("pic/dott.png");
	XZTexId = env->CallStaticIntMethod(cl, id, obj, name);
	env->DeleteLocalRef(name);
}
//初始化纹理矩形
void LoadingView::initTextureRect() {
	//背景纹理矩形
	bgTextureRect = new TextureRect(Constant::RADIO, 1.0, 0, 1,
			ShaderManager::getCommTextureShaderProgram(), NULL); //创建矩形形对象
	//旋转纹理矩形
	XZTextureRect = new TextureRect(0.1, 0.1, 0, 1,
			ShaderManager::getCommTextureShaderProgram(), NULL);	//创建矩形形对象
}
