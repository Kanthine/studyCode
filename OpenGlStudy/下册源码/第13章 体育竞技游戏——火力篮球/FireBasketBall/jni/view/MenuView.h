#ifndef _MenuView_H_
#define _MenuView_H_
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <jni.h>

#include "MyBullet/TextureRect.h"

class MenuView
{
public:

	MenuView();
	~MenuView();
	void initGameOverMenu();
	void initTextureId(JNIEnv * env,jobject obj);
public:
	//被创建
	void onViewCreated(JNIEnv * env,jobject obj);
	//被改变
	void onViewchanged(int w, int h);
	//画自己
	void drawSelf(int MenuId);//传入需要绘制的菜单Id，根据id绘制对应的菜单
	//绘制游戏结束菜单
	void drawGameOverMenu();
	//绘制游戏开始菜单
	void drawGameStartMenu();
	//判断哪个菜单被点中，若返回0，则无菜单项被点中
	int judgeMenuTouched(float x,float y);
	//触摸开始被回调
	void onTouchBegan(float touchX,float touchY);
	//触摸移动被回调
	void onTouchMoved(float touchX,float touchY);
	//触摸移动被回调
	void onTouchEnded(float touchX,float touchY);
public:
	//背景
	TextureRect* gameOverBgTextureRect;
	//次菜单
	TextureRect* menuTextureRect;

	float ratio;
	//背景纹理Id
	GLuint bgTexId;
	//再来一次纹理Id
	GLuint ZLYCTexId;
	//退出纹理Id
	GLuint TCTextId;
	//返回菜单纹理Id
	GLuint FHCDTextId;
	//开始游戏菜单纹理id
	GLuint KSYXTexId;
	//设置菜单纹理id
	GLuint SZTexId;
	//关于菜单纹理id
	GLuint GYTexId;
	//帮助菜单纹理id
	GLuint BZTexId;
	//记录菜单纹理id
	GLuint JLTexId;
};

#endif
