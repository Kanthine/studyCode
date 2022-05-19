#ifndef _LoadingView_H_
#define _LoadingView_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "jni.h"
#include <thread>

#include "MyBullet/TextureRect.h"
using namespace std;
class LoadingView {
public:
	LoadingView();
	~LoadingView();
public:
	//被创建
	void onViewCreated(JNIEnv * env, jobject obj);
	//被改变
	void onViewchanged(int w, int h);
	//画自己
	void drawSelf();
	//初始化纹理矩形
	void initTextureRect();
	//初始化纹理Id
	void initTextureId(JNIEnv * env, jobject obj);
	//转圈线程
	void threadTask();
	//加载资源
	void intiAllAssats(JNIEnv * env, jobject obj);
public:
	float ratio;
	//背景纹理Id
	GLuint bgTexId;
	//背景纹理矩形
	TextureRect* bgTextureRect;
	//旋转的纹理Id
	GLuint XZTexId;
	//旋转纹理矩形
	TextureRect* XZTextureRect;
	//旋转的角度
	int angle;

	thread* t;
	bool loadingFlag;
	int loadTime;

	bool isAttach;
	int loadIndex;
};

#endif
