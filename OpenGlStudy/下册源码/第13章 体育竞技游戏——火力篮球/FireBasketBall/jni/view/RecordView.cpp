#include "view/RecordView.h"
#include "util/MatrixState.h"
#include "util/ShaderManager.h"
#include "SQLiteUtil/SQLiteUtil.h"
#include "util/Constant.h"

#include "FireBasketBallcpp/main.h"

#include <stdlib.h>
RecordView::RecordView() {

}
//被创建
void RecordView::onViewCreated(JNIEnv * env, jobject obj) {
	//设置背景颜色
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	//初始化变换矩阵
	MatrixState::setInitStack();
	//这里编译3D场景中欢迎欢迎界面中的shader
	ShaderManager::compileShader();
	initTextureRect();
	initTextureId(env, obj);
	calTableResult();

}
//被改变
void RecordView::onViewchanged(int w, int h) {
	//设置视口
	glViewport(0, 0, w, h);
	//计算宽长比
	ratio = (float) w / h;
	//打开背面剪裁
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}
//画自己
void RecordView::drawSelf() {
	//清颜色缓冲和深度缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	MatrixState::pushMatrix();
	//设置透视投影
//	MatrixState::setProjectFrustum(-ratio, ratio, -1, 1, 4.0f, 100);
	MatrixState::setProjectOrtho(-ratio, ratio, -1, 1, 4.0f, 100);
	//设置摄像机九参数
	MatrixState::setCamera(0, 0, 5, 0, 0, 0, 0, 1, 0);

	//开启混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	MatrixState::pushMatrix();
	bgTextureRect->drawSelf(bgTexId);
	MatrixState::popMatrix();

	MatrixState::pushMatrix();
	tgBgTextureRect->drawSelf(tgBgTexId);
	MatrixState::popMatrix();

	std::vector<std::vector<GLuint>>::iterator iter = gradePNGVector->begin();
	float offsetX = 0.3;
	float offsetY = 0.25;
	for (; iter != gradePNGVector->end(); iter++) {
		std::vector<GLuint>::iterator iterTwo = (*iter).begin();
		for (; iterTwo != (*iter).end(); iterTwo++) {
			MatrixState::pushMatrix();
			MatrixState::translate(offsetX, offsetY, 0);
			digitTextureRect->drawSelf(*iterTwo);
			MatrixState::popMatrix();
			offsetX += 0.05;
		}
		offsetX = 0.3;
		offsetY -= 0.09;
	}

	iter = timePNGVector->begin();
	offsetX = -0.35;
	offsetY = 0.25;
	for (; iter != timePNGVector->end(); iter++) {
		std::vector<GLuint>::iterator iterTwo = (*iter).begin();
		for (; iterTwo != (*iter).end(); iterTwo++) {
			MatrixState::pushMatrix();
			MatrixState::translate(offsetX, offsetY, 0);
			digitTextureRect->drawSelf(*iterTwo);
			MatrixState::popMatrix();
			offsetX += 0.04;
		}
		offsetX = -0.35;
		offsetY -= 0.09;
	}

	MatrixState::pushMatrix();
	MatrixState::translate(-0.3, -0.8, 0);
	if (Constant::isTouched[Constant::TOUCHED_QD_MENU]) {
		MatrixState::scale(Constant::MENU_SACLE_X, Constant::MENU_SACLE_Y,
				Constant::MENU_SACLE_Z);
	}
	menuTextureRect->drawSelf(QDMenuTextId);
	MatrixState::popMatrix();

	MatrixState::pushMatrix();
	MatrixState::translate(0.3, -0.8, 0);
	if (Constant::isTouched[Constant::TOUCHED_FH_MENU]) {
		MatrixState::scale(Constant::MENU_SACLE_X, Constant::MENU_SACLE_Y,
				Constant::MENU_SACLE_Z);
	}
	menuTextureRect->drawSelf(FHMenuTextId);
	MatrixState::popMatrix();

	//关闭混合
	glDisable(GL_BLEND);
	MatrixState::popMatrix();
}
//初始化纹理矩形
void RecordView::initTextureRect() {
	//背景纹理矩形
	bgTextureRect = new TextureRect(Constant::RADIO, 1.0, 0, 1,
			ShaderManager::getCommTextureShaderProgram(), NULL);

	tgBgTextureRect = new TextureRect(Constant::RADIO-0.15, 0.6, 0, 1,
			ShaderManager::getCommTextureShaderProgram(), NULL);

	digitTextureRect = new TextureRect(0.02, 0.03, 0, 1,
			ShaderManager::getCommTextureShaderProgram(), NULL);

	menuTextureRect = new TextureRect(Constant::MENU_QD_FH_WIDTH_HALF,
			Constant::MENU_QD_FH_HEIGHT_HALF, 0, 1,
			ShaderManager::getCommTextureShaderProgram(), NULL); //创建矩形形对象
}
//初始化纹理Id
void RecordView::initTextureId(JNIEnv * env, jobject obj) {
	jclass cl = env->FindClass("com/bn/bullet/GL2JNIView");
	jmethodID id = env->GetStaticMethodID(cl, "initTextureRepeat",
			"(Landroid/opengl/GLSurfaceView;Ljava/lang/String;)I");
	//背景纹理Id
	jstring name = env->NewStringUTF("pic/background.png");
	bgTexId = env->CallStaticIntMethod(cl, id, obj, name);

	name = env->NewStringUTF("pic/timegradebeijing.png");
	tgBgTexId = env->CallStaticIntMethod(cl, id, obj, name);

	for (int i = 0; i < 10; i++) {
		char pngPath[10];
		//整形转字符串
		sprintf(pngPath, "pic/d%d.png", i);
		name = env->NewStringUTF(pngPath);
		digitTexId[i] = env->CallStaticIntMethod(cl, id, obj, name);
	}
	name = env->NewStringUTF("pic/maohao.png");
	digitTexId[10] = env->CallStaticIntMethod(cl, id, obj, name);

	name = env->NewStringUTF("pic/hengxian.png");
	digitTexId[11] = env->CallStaticIntMethod(cl, id, obj, name);

	name = env->NewStringUTF("pic/d10.png");
	digitTexId[12] = env->CallStaticIntMethod(cl, id, obj, name);

	//确定菜单纹理Id
	name = env->NewStringUTF("pic/quedinganniu.png");
	QDMenuTextId = env->CallStaticIntMethod(cl, id, obj, name);
	//返回菜单纹理Id
	name = env->NewStringUTF("pic/fanhuianniu.png");
	FHMenuTextId = env->CallStaticIntMethod(cl, id, obj, name);
}
void RecordView::calTableResult() {
	//查询结果储存的向量
	std::vector<std::string>* result = new std::vector<std::string>();
	result->clear();
	std::string sSQL =
			"select grade,time from paihangbang  order by grade desc limit 0,30";

	result = SQLiteUtil::query(sSQL);
	vectorToStringArray(result);
}
void RecordView::vectorToStringArray(std::vector<std::string>* result) {
	//储存得分的字符串向量
	std::vector<std::string>* gradeVector = new std::vector<std::string>();
	//储存时间的字符串数组
	std::vector<std::string>* timeVector = new std::vector<std::string>();
	//计数器，count % 2 == 0是得分，count % 2 == 1是时间
	int count = 0;
	//遍历原向量组-进行得分和时间的分离
	std::vector<std::string>::iterator iter = result->begin();
	for (; iter != result->end(); iter++) {
		//避免数组越界--只显示排行前Constant::RECORD_COUNT的记录
		if (count >= Constant::RECORD_COUNT * 2) {
			break;
		}
		if (count % 2 == 0) {
			gradeVector->push_back(*iter);
		} else {
			timeVector->push_back(*iter);
		}
		count++;
	}
	//储存得分图片名称的字符串数组
	gradePNGVector = new std::vector<std::vector<GLuint>>();
	//储存时间图片名称的字符串数组
	timePNGVector = new std::vector<std::vector<GLuint>>();

	//解析得分
	std::vector<std::string>::iterator iterGrade = gradeVector->begin();
	for (; iterGrade != gradeVector->end(); iterGrade++) {
		std::vector<GLuint>* gotGradePNGVector = analyzeStringToPNGVector(
				(*iterGrade));
		gradePNGVector->push_back(*gotGradePNGVector);
	}
	//解析时间
	for (iterGrade = timeVector->begin(); iterGrade != timeVector->end();
			iterGrade++) {
		std::vector<GLuint>* gotTimePNGVector = analyzeStringToPNGVector(
				(*iterGrade));
		timePNGVector->push_back(*gotTimePNGVector);
	}
}
std::vector<GLuint>* RecordView::analyzeStringToPNGVector(
		std::string strArray) {
	//结果向量
	std::vector<GLuint>* resultVector = new std::vector<GLuint>();
	//结果字符串
	GLuint res = 0;
	for (int j = 0; j < strArray.size(); j++) {
		//解析单个字符
		char cTemp = strArray.c_str()[j];
		//判断字符对应的PNG
		switch (cTemp) {
		case '0':
			res = digitTexId[0];
			break;
		case '1':
			res = digitTexId[1];
			break;
		case '2':
			res = digitTexId[2];
			break;
		case '3':
			res = digitTexId[3];
			break;
		case '4':
			res = digitTexId[4];
			break;
		case '5':
			res = digitTexId[5];
			break;
		case '6':
			res = digitTexId[6];
			break;
		case '7':
			res = digitTexId[7];
			break;
		case '8':
			res = digitTexId[8];
			break;
		case '9':
			res = digitTexId[9];
			break;
		case ':':
			res = digitTexId[10];
			break;
		case '-':
			res = digitTexId[11];
			break;
		case ' ':
			res = digitTexId[12];
			break;
		}
		resultVector->push_back(res);
	}
	return resultVector;
}

void RecordView::onTouchBegan(float touchX, float touchY) {
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
void RecordView::onTouchMoved(float touchX, float touchY) {
}
void RecordView::onTouchEnded(float touchX, float touchY) {
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
int RecordView::judgeMenuTouched(float x, float y) {
	if(y<(Constant::MENU_QD_FH_HEIGHT_HALF-0.8)
				&&y>(-Constant::MENU_QD_FH_HEIGHT_HALF-0.8))
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
