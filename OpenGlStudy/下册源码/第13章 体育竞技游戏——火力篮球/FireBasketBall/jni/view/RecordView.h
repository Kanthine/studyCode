#ifndef _RecordView_H_
#define _RecordView_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "MyBullet/TextureRect.h"
#include "jni.h"

using namespace std;
#include <string>
#include <vector>

class RecordView
{
public:
	RecordView();
	~RecordView(){};
public:
	//被创建
	void onViewCreated(JNIEnv * env,jobject obj);
	//被改变
	void onViewchanged(int w, int h);
	//画自己
	void drawSelf();
	//初始化纹理矩形
	void initTextureRect();
	//初始化纹理Id
	void initTextureId(JNIEnv * env,jobject obj);
	//触摸被回调方法
	void onTouchBegan(float touchX,float touchY);
	void onTouchMoved(float touchX,float touchY);
	void onTouchEnded(float touchX,float touchY);
	//查询表
	void calTableResult();
	//把向量分组
	void vectorToStringArray(std::vector<std::string>* result);
	//解析数组
	std::vector<GLuint>* analyzeStringToPNGVector(std::string strArray);
	//判断是否被触摸
	int judgeMenuTouched(float x, float y);
public:
	float ratio;

	TextureRect* bgTextureRect ;
	GLuint bgTexId;

	TextureRect* tgBgTextureRect ;
	GLuint tgBgTexId;

	TextureRect* digitTextureRect ;
	GLuint digitTexId[13];

	//菜单纹理矩形
	TextureRect* menuTextureRect;
	//确定菜单纹理Id
	GLuint QDMenuTextId;
	//阀门、你还菜单纹理Id
	GLuint FHMenuTextId;

	//储存得分图片名称的字符串数组
	std::vector<std::vector<GLuint>>* gradePNGVector ;
	//储存时间图片名称的字符串数组
	std::vector<std::vector<GLuint>>* timePNGVector ;

};

#endif
