#ifndef _AboutView_H_
#define _AboutView_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <jni.h>

#include "MyBullet/TextureRect.h"

class AboutView
{
public:
	AboutView();
	~AboutView();
public:
	//被创建
	void onViewCreated(JNIEnv * env,jobject obj);
	//被改变
	void onViewchanged(int w, int h);
	//画自己
	void drawSelf();//传入需要绘制的菜单Id，根据id绘制对应的菜单
	//初始化纹理Id
	void initTextureId(JNIEnv * env,jobject obj);
    //初始化纹理矩形
	void initTextureRect();
	//初始化菜单
	void initMenu();
	void onTouchBegan(float touchX,float touchY);
	void onTouchMoved(float touchX,float touchY);
	void onTouchEnded(float touchX,float touchY);
	int judgeMenuTouched(float x, float y);
public:
	//背景纹理Id
	GLuint bgTexId;
	//关于信息纹理Id
	GLuint aboutTexId;
	//确定菜单纹理Id
	GLuint QDMenuTextId;
	//阀门、你还菜单纹理Id
	GLuint FHMenuTextId;

	//背景矩形
	TextureRect* bgTextureRect;
	//关于信息矩形
	TextureRect* aboutTextureRect;
	//菜单纹理矩形
	TextureRect* menuTextureRect;
	//比例
	float ratio;
};

#endif
