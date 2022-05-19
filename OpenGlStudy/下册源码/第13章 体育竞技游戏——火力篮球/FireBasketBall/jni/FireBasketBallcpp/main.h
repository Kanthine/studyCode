#ifndef _main_H_
#define _main_H_

#include "view/GameView.h"
#include "view/MenuView.h"
#include "view/AboutView.h"
#include "view/SetView.h"
#include "view/LoadingView.h"
#include "view/RecordView.h"
#include "jni.h"

class main
{
public:
	static GameView* gameView;
	static MenuView* menuView;
	static AboutView* aboutView;
	static SetView* setView;
	static LoadingView* loadingView;
	static RecordView* recordView;
	//退出
	void static NDKCallExit(JNIEnv * env,jobject obj);
	static void playBackmusic(JNIEnv * env,jobject obj,jint flag);
	static void sendViewIndex(JNIEnv * env,jobject obj,jint currViewIn);
	static void loadObjDataWd(
			int objId,
			float* vertices,
			int numsVer,
			int* indices,
			int numsInd,
			float* texs,
			int numsTex,
			float* normal,
			int numNormal);
	static float* copyFloats(float* src,int count);
	static int* copyInts(int* src,int count);
	static JNIEnv * envMain;
	static jobject objMain;

	//全局变量
	static JavaVM *g_jvm;
	static jobject g_obj;

};

#endif
