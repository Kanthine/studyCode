#include <stdio.h>
#include <math.h>

#include <string.h>
#include <jni.h>

#include <vector>
#include "Bullet/LinearMath/btAlignedObjectArray.h"
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/BulletSoftBody/btSoftBody.h"
#include "Bullet/BulletSoftBody/btSoftBodyHelpers.h"
#include "Bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "Bullet/BulletSoftBody/btSoftBodySolvers.h"
#include "Bullet/BulletSoftBody/btDefaultSoftBodySolver.h"
#include "Bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"

#include "util/Matrix.h"
#include "util/MatrixState.h"
#include "util/FileUtil.h"
#include "util/Constant.h"
#include "FireBasketBallcpp/main.h"
#include "MyBullet/TexTriangleMesh.h"
#include "MyBullet/SoftObj.h"
#include "SQLiteUtil/SQLiteUtil.h"
#include "util/FileUtil.h"
using namespace std;

GameView* main::gameView;
MenuView* main::menuView;
AboutView* main::aboutView;
SetView* main::setView;
LoadingView* main::loadingView;
RecordView* main::recordView;

JNIEnv * main::envMain;
jobject main::objMain;

//全局变量
JavaVM *main::g_jvm = NULL;
jobject main::g_obj = NULL;
void main::NDKCallExit(JNIEnv * env,jobject obj)
{
    //Attach主线程
    if((*main::g_jvm).AttachCurrentThread(&env, NULL) != JNI_OK)
    {
        return ;
    }
//	//获取此类文件，若没有获取到，则表明没有此类
	jclass cls = env->FindClass("com/bn/bullet/JNIPort");
	if(cls != NULL)
	{
		//获取此类文件的NDKCallExit方法，若没有获取到，则表明没有此方法
//			GetStaticMethodID方法相关参数1.java类，2.方法名，3.参数,返回类型
		jmethodID id = env->GetStaticMethodID(cls,"NDKCallExit","()V");
		if(id != NULL)
		{
			env->CallStaticVoidMethod(cls, id);
		}
	}
	//Detach主线程
	if((*main::g_jvm).DetachCurrentThread() != JNI_OK)
	{
		return ;
	}
}
void main::playBackmusic(JNIEnv * env,jobject obj,jint flag)
{
    //Attach主线程
    if((*main::g_jvm).AttachCurrentThread(&env, NULL) != JNI_OK)
    {
        return ;
    }
	//获取此类文件，若没有获取到，则表明没有此类
	jclass cls = env->FindClass("com/bn/bullet/JNIPort");
	if(cls != NULL)
	{
		//获取此类文件的NDKCallExit方法，若没有获取到，则表明没有此方法
		//GetStaticMethodID方法相关参数1.java类，2.方法名，3.参数,返回类型
		jmethodID id = env->GetStaticMethodID(cls,"playBackMusic","(I)V");
		if(id != NULL)
		{
			env->CallStaticVoidMethod(cls, id,flag);
		}
	}
}
void main::sendViewIndex(JNIEnv* env,jobject obj,jint currViewIn)
{
    //Attach主线程
    if((*main::g_jvm).AttachCurrentThread(&env, NULL) != JNI_OK)
    {
        return ;
    }
	//获取此类文件，若没有获取到，则表明没有此类
	jclass cls = env->FindClass("com/bn/bullet/JNIPort");

	if(cls != NULL)
	{
		//获取此类文件的NDKCallExit方法，若没有获取到，则表明没有此方法
		//	GetStaticMethodID方法相关参数1.java类，2.方法名，3.参数,返回类型
		jmethodID id = env->GetStaticMethodID(cls,"recordCurrViewIndex","(I)V");
		if(id != NULL)
		{
			jvalue jvaluetemp[1];
			jvaluetemp[0].i = currViewIn;
			env->CallStaticVoidMethodA(cls, id,jvaluetemp) ; // CallStaticVoidMethod(cls, id,currViewIn);
		}
	}
}

#define RIGID_KUANG_OBJ 1//篮筐OBJ导入
#define SOFT_NET_OBJ 2//篮网OBJ导入
void main::loadObjDataWd(
		int objId,
		float* vertices,
		int numsVer,
		int* indices,
		int numsInd,
		float* texs,
		int numsTex,
		float* normal,
		int numNormal
)
{
	if(objId == RIGID_KUANG_OBJ)
	{
		TexTriangleMesh::vertices_yuanhuan = (float*)vertices;	//顶点数据
		TexTriangleMesh::numsVer_yuanhuan = numsVer;			//顶点个数--------------定点数*3*3
		TexTriangleMesh::indices_yuanhuan = (int*)indices;		//顶点索引
		TexTriangleMesh::numsInd_yuanhuan = numsInd;			//索引个数
		TexTriangleMesh::texs_yuanhuan = (float*)texs;			//纹理坐标
		TexTriangleMesh::numsTex_yuanhuan = numsTex;			//纹理坐标个数

		TexTriangleMesh::normal_yuanhuan = (float*)normal;
		TexTriangleMesh::numNormal_yuanhuan = numNormal;
	}
	else if(objId == SOFT_NET_OBJ)
	{
		SoftObj::vertices_soft = (float*)vertices;
		SoftObj::numsVer_soft = numsVer;
		SoftObj::indices_soft = (int*)indices;
		SoftObj::numsInd_soft = numsInd;
		SoftObj::texs_soft = (float*)texs;
		SoftObj::numsTex_soft = numsTex;
		SoftObj::normal_soft = (float*)normal;
		SoftObj::numNormal_soft = numNormal;
	}
}
float* main::copyFloats(float* src,int count)
{
	float* dst=new float[count];
	for(int i=0;i<count;i++)
	{
		dst[i]=src[i];
	}
	return dst;
}
int* main::copyInts(int* src,int count)
{
	int* dst=new int[count];
	for(int i=0;i<count;i++)
	{
		dst[i]=src[i];
	}
	return dst;
}
//地形  光线投射
extern "C" {
	bool onSurfaceChanged(int w, int h) {
		Constant::w = w;
		Constant::h = h;
		if(!Constant::SWAP_VIEW_FLAG)
		{
			//判断当前的游戏界面
			if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_GAME_INDEX)		//游戏主界面
			{
				main::gameView->onViewchanged(w,h);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_MENU_INDEX)	//菜单界面
			{
				main::menuView->onViewchanged(w,h);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_ABOUT_INDEX)	//关于界面
			{
				main::aboutView->onViewchanged(w,h);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_SET_INDEX)	//设置界面
			{
				main::setView->onViewchanged(w,h);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_LOADING_INDEX)
			{
				main::loadingView->onViewchanged(w,h);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_RECORD_INDEX)
			{
				main::recordView->onViewchanged(w,h);
			}
		}
		return true;
	}
	void initDatabase()
	{
		//建表语句
		std::string sSQL1 = "create table if not exists paihangbang(grade int(4),time char(20))";
		//建表
	    SQLiteUtil::doSqlite3_exec(sSQL1);
	}
	bool onSurfaceCreated(JNIEnv * env,jobject obj) {

		main::envMain = env;
		main::objMain = env->NewGlobalRef(obj);

		env->GetJavaVM(&main::g_jvm);
		main::g_obj = env->NewGlobalRef(obj);

		//初始化数据库
		initDatabase();

		//加载资源界面
		main::loadingView = new LoadingView();
		main::loadingView->onViewCreated(env,obj);

		return true;
	}

	void renderFrame() {
		if(!Constant::SWAP_VIEW_FLAG)
		{
			//判断当前的游戏界面
			if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_GAME_INDEX)
			{
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				main::gameView->drawSelf();
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_MENU_INDEX)
			{
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				main::menuView->drawSelf(Constant::MENU_CURR_MENUINDEX);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_ABOUT_INDEX)
			{
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				main::aboutView->drawSelf();
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_SET_INDEX)	//设置界面
			{
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				main::setView->drawSelf();
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_LOADING_INDEX)
			{
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				main::loadingView->drawSelf();
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_RECORD_INDEX)
			{
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				main::recordView->drawSelf();
			}
		}
	}
	char*   Jstring2CStr(JNIEnv*   env,   jstring   jstr)
	{
	     char*   rtn   =   NULL;
	     jclass   clsstring   =   (*env).FindClass("java/lang/String");
	     jstring   strencode   =   (*env).NewStringUTF("GB2312");
	     jmethodID   mid   =   (*env).GetMethodID(clsstring,"getBytes",   "(Ljava/lang/String;)[B");
	     jbyteArray   barr=   (jbyteArray)(*env).CallObjectMethod(jstr,mid,strencode); // String .getByte("GB2312");
	     jsize   alen   =   (*env).GetArrayLength(barr);
	     jbyte*   ba   =   (*env).GetByteArrayElements(barr,JNI_FALSE);
	     if(alen   >   0)
	     {
	      rtn   =   (char*)malloc(alen+1);         //new   char[alen+1]; "\0"
	      memcpy(rtn,ba,alen);
	      rtn[alen]=0;
	     }
	     (*env).ReleaseByteArrayElements(barr,ba,0);  //释放内存

	     return rtn;
	}
	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_sendSDCardPath
	  (JNIEnv *env, jclass cls, jstring SDCardPathin)
	{
		char *SDCardPathTemp = Jstring2CStr(env, SDCardPathin);
		std::string path((const char*)SDCardPathTemp);//获得结果字符串
		FileUtil::setSDCardPath(path);
	}
	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_nativeSetAssetManager
	  (JNIEnv *env, jclass cls, jobject assetManager)
	{
		AAssetManager* aamIn = AAssetManager_fromJava( env, assetManager );
	    FileUtil::setAAssetManager(aamIn);
	}
	//触摸回调方法
	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_onTouchBegan(JNIEnv *env, jclass jc, jfloat touchX,jfloat touchY)
	{
		if(!Constant::SWAP_VIEW_FLAG)
		{
			//判断当前的游戏界面
			if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_GAME_INDEX)//游戏界面
			{
				main::gameView->onTouchBegan(touchX,touchY);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_MENU_INDEX)//游戏菜单界面
			{
				main::menuView->onTouchBegan(touchX,touchY);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_ABOUT_INDEX)//游戏关于界面
			{
				main::aboutView->onTouchBegan(touchX,touchY);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_SET_INDEX)	//设置界面
			{
				main::setView->onTouchBegan(touchX,touchY);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_RECORD_INDEX)
			{
				main::recordView->onTouchBegan(touchX,touchY);
			}
		}
	}
	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_onTouchMoved(JNIEnv *env, jclass jc, jfloat touchX,jfloat touchY)
	{
		if(!Constant::SWAP_VIEW_FLAG)
		{
			//判断当前的游戏界面
			if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_GAME_INDEX)//游戏界面
			{
				main::gameView->onTouchMoved(touchX,touchY);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_MENU_INDEX)//游戏菜单界面
			{
				main::menuView->onTouchMoved(touchX,touchY);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_ABOUT_INDEX)//游戏关于界面
			{
				main::aboutView->onTouchMoved(touchX,touchY);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_SET_INDEX)	//设置界面
			{
				main::setView->onTouchMoved(touchX,touchY);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_RECORD_INDEX)
			{
				main::recordView->onTouchMoved(touchX,touchY);
			}
		}
	}
	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_onTouchEnded(JNIEnv *env, jclass jc, jfloat touchX,jfloat touchY)
	{
		if(!Constant::SWAP_VIEW_FLAG)
		{
			//判断当前的游戏界面
			if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_GAME_INDEX)//游戏界面
			{
				main::gameView->onTouchEnded(touchX,touchY);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_MENU_INDEX)//游戏菜单界面
			{
				main::menuView->onTouchEnded(touchX,touchY);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_ABOUT_INDEX)//游戏关于界面
			{
				main::aboutView->onTouchEnded(touchX,touchY);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_SET_INDEX)	//设置界面
			{
				main::setView->onTouchEnded(touchX,touchY);
			}else if(Constant::VIEW_CUR_GL_INDEX == Constant::VIEW_RECORD_INDEX)
			{
				main::recordView->onTouchEnded(touchX,touchY);
			}
		}
	}
	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_sendWidthAndHeight(JNIEnv *env, jclass jc, jfloat width,jfloat height)
	{
		Constant::SCREEN_HEIGHT = height;
		Constant::SCREEN_WIDTH = width;

        float zoomx=Constant::SCREEN_WIDTH/1080;
		float zoomy=Constant::SCREEN_HEIGHT/1920;
		if(zoomx>zoomy){
			Constant::SCREEN_WIDTH_SCALE=Constant::SCREEN_HEIGHT_SCALE=zoomy;
		}else
		{
			Constant::SCREEN_WIDTH_SCALE=Constant::SCREEN_HEIGHT_SCALE=zoomx;
		}
//		Constant::sXtart=(Constant::SCREEN_WIDTH-Constant::SCREEN_WIDTH*Constant::SCREEN_WIDTH_SCALE)/2;
//		Constant::sYtart=(Constant::SCREEN_HEIGHT-Constant::SCREEN_HEIGHT*Constant::SCREEN_HEIGHT_SCALE)/2;

		Constant::RADIO = (float)(1080) / (float)(1920);
//		Constant::RADIO = width / height;

	}
	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_toMenuView(JNIEnv *env, jclass jc)
	{
		Constant::SWAP_VIEW_FLAG = true;
		Constant::VIEW_CUR_GL_INDEX = Constant::VIEW_MENU_INDEX;
		Constant::MENU_CURR_MENUINDEX = Constant::MENU_STARTVIEW_START;
		main::menuView->onViewchanged(Constant::w, Constant::h);
		Constant::SWAP_VIEW_FLAG = false;
	}
//	//由java调用来建立JNI环境
//	JNIEXPORT void Java_com_bn_bullet_JNIPort_setJNIEnv( JNIEnv* env, jobject obj)
//	 {
//	     //保存全局JVM以便在子线程中使用
//	     (*env).GetJavaVM(&main::g_jvm);
//	     //不能直接赋值(g_obj = obj)
//	     main::g_obj = (*env).NewGlobalRef(obj);
//	 }

}


