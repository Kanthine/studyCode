#ifndef _SetView_H_
#define _SetView_H_
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "MyBullet/TextureRect.h"
#include "jni.h"
class SetView{
public:
	SetView();
	~SetView();
public:
	//被创建
	void onViewCreated(JNIEnv * env,jobject obj);
	//被改变
	void onViewchanged(int w, int h);
	//画自己
	void drawSelf();//传入需要绘制的菜单Id，根据id绘制对应的菜单
	//初始化纹理矩形
	void initTextureRect();
	//初始化纹理Id
	void initTextureId(JNIEnv * env,jobject obj);
	//触摸开始被回调
	void onTouchBegan(float touchX,float touchY);
	//触摸移动被回调
	void onTouchMoved(float touchX,float touchY);
	//触摸移动被回调
	void onTouchEnded(float touchX,float touchY);
	int judgeMenuTouched(float x,float y);
public:
	//背景纹理Id
	GLuint bgTexId;
	//背景纹理矩形
	TextureRect* bgTextureRect;
	//背景音乐纹理Id
	GLuint BJYY_TexId;
	//音效音乐纹理Id
	GLuint YXYY_TexId;
	//背景音乐、音效音乐纹理矩形
	TextureRect* infoTextureRect;
	//背景纹理Id
	GLuint menu_QD_TexId;
	//背景纹理Id
	GLuint menu_FH_TexId;
	//确定、取消纹理矩形
	TextureRect* menuTextureRect;

	//选中框纹理Id
	GLuint menu_KUANG_TexId;
	TextureRect* kuangTextureRect;

	//对勾纹理Id
	GLuint KUANG_DUIGOU_TexId;

	float ratio;
};

#endif
