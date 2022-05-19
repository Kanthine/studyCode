#include "Constant.h"
#include "math.h"

std::string SDCardPath = "";

float Constant::RADIO = 0;

//onViewChange的w,h
int Constant::w = 0;
int Constant::h = 0;
//界面切换的标志位
bool Constant::SWAP_VIEW_FLAG = false;
//π值
const float Constant::PI = 3.1415926;
//屋子的长、宽、高

const float Constant::CHANGJING_WIDTH=4.0f;
const float Constant::CHANGJING_HEIGHT=7.2f;
const float Constant::CHANGJING_LENGTH=4.0f;
//仪表版
const float Constant::YBB_WIDTH = 2;
const float Constant::YBB_HEIGHT = 0.3f;

//摄像机目标位置
float Constant::CAMERA_X=0.0f;
float Constant::CAMERA_Y=CHANGJING_HEIGHT/2-0.35f;
float Constant::CAMERA_Z=(CHANGJING_LENGTH+2.4f+8.5f);

const float Constant::DISTANCE=CAMERA_Z;//LENGTH;


int Constant::VIEW_CUR_GL_INDEX = 5;
//游戏界面
const int Constant::VIEW_GAME_INDEX = 1;
//游戏结束界面
const int Constant::VIEW_MENU_INDEX = 2;
//游戏关于界面
const int Constant::VIEW_ABOUT_INDEX = 3;
//设置界面
const int Constant::VIEW_SET_INDEX = 4;
//加载界面
const int Constant::VIEW_LOADING_INDEX = 5;
//记录界面
const int Constant::VIEW_RECORD_INDEX = 6;
//当前需要显示的菜单编号
int Constant::MENU_CURR_MENUINDEX = 0;
//开始界面菜单
const int Constant::MENU_STARTVIEW_START = 0;
//游戏结束菜单
const int Constant::MENU_GAMEVIEW_END = 1;

//判断是否是帮助界面的标志位
bool Constant::isnoHelpView = false;

int Constant::shipingJs = 0;//视频线程计时器
const float Constant::STARTBALL_V[3][3]={
		{0.90f,10.8f,-3.0f},
		{0,10.8f,-3.0f},
		{-0.90f,10.8f,-3.0f}
};

//帮助手的坐标
float Constant::shouX = 0;
float Constant::shouY = -1;

float Constant::SCREEN_WIDTH = 480;
float Constant::SCREEN_HEIGHT = 854;

float Constant::SCREEN_WIDTH_SCALE = 1;
float Constant::SCREEN_HEIGHT_SCALE = 1;

float Constant::BASKETBALL_R = 0.35;

//float Constant::BASKETBALL_ORIGIN_X[3] = {-1,0,1};
//float Constant::BASKETBALL_ORIGIN_Y[3] = {0.5,0.5,0.5};
//float Constant::BASKETBALL_ORIGIN_Z[3] = {1.6,1.6,1.6};

const float Constant::LANBAN_X=0;//篮板的位置x坐标
const float Constant::LANBAN_Y=5;//篮板的位置y坐标
const float Constant::LANBAN_Z=-1.9f;//篮板的位置z坐标

const float Constant::LANKUANG_X=0;//篮板的位置x坐标
const float Constant::LANKUANG_Y=4.5;//篮板的位置y坐标
const float Constant::LANKUANG_Z=-1.3f;//篮板的位置z坐标

const float Constant::LANKUANG_R = 0.65;//篮筐半径

const float Constant::LANWANG_H = 4.5;//篮网的高度

float Constant::gFactor=1.6f;							//重力加速度缩放比例
float Constant::vFactor=(float)sqrt(Constant::gFactor);	//y方向速度缩放比例

float Constant::sXtart = 0;
float Constant::sYtart = 0;

//仪表板中单个数字的大小
const float Constant::SHUZI_KUANDU= 0.1f;
const float Constant::SHUZI_GAODU= 0.12f;

//三个篮球对应位置数组
const float Constant::STARTBALL_1[3] = {-1.0f,0.4f,1.6f};
const float Constant::STARTBALL_2[3] = {0,0.4f,1.6f};
const float Constant::STARTBALL_3[3] = {1.0f,0.4f,1.6f};

const float Constant::STARTBALL[3][3] = {
										{Constant::STARTBALL_1[0],Constant::STARTBALL_1[1],Constant::STARTBALL_1[2]},
										{Constant::STARTBALL_2[0],Constant::STARTBALL_2[1],Constant::STARTBALL_2[2]},
										{Constant::STARTBALL_3[0],Constant::STARTBALL_3[1],Constant::STARTBALL_3[2]}
									 };


//得分
int Constant::DIGITAL_SCORE = 13;
//总时间
float Constant::DIGITAL_TOTAL_TIME = 560;
//时间
float Constant::DIGITAL_TIME = 0;

//游戏时间结束标志位
bool Constant::GAMEOVER = false;

//每个菜单的间距
const float Constant::MENU_SPAN = 0.5;
//每个菜单矩形的长
const float Constant::MENU_HEIGHT_HALF = 0.08;
//每个菜单矩形的宽
const float Constant::MENU_WIDTH_HALF = 0.3;


//被点击的菜单编号
int Constant::TOUCHED_CURR_MENU = 0;
//是否被点击的标志位---数组长度为菜单种类
bool Constant::isTouched[15] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
const float Constant::MENU_SACLE_X = 1.1;
const float Constant::MENU_SACLE_Y = 1.1;
const float Constant::MENU_SACLE_Z = 1.0;
//没有菜单项被点击
const int Constant::TOUCHED_NONE_MENU = 0;
//开始游戏菜单被点击
const int Constant::TOUCHED_KSYX_MENU = 1;
//设置游戏菜单被点击
const int Constant::TOUCHED_SZ_MENU = 2;
//关于游戏菜单被点击
const int Constant::TOUCHED_GY_MENU = 3;
//帮助游戏菜单被点击
const int Constant::TOUCHED_BZ_MENU = 4;
//记录游戏菜单被点击
const int Constant::TOUCHED_JL_MENU = 5;
//退出游戏菜单被点击
const int Constant::TOUCHED_TC_MENU = 6;
//再来一局游戏菜单被点击
const int Constant::TOUCHED_ZLYJ_MENU = 7;
//返回菜单游戏菜单被点击
const int Constant::TOUCHED_FHCD_MENU = 8;

//确定菜单游戏菜单被点击
const int Constant::TOUCHED_QD_MENU = 9;
//返回菜单游戏菜单被点击
const int Constant::TOUCHED_FH_MENU = 10;

//停止菜单被点击
const int Constant::TOUCH_STOP_MENU = 11;
//暂停菜单被点击
const int Constant::TOUCH_PAUSE_MENU = 12;
//场景音乐菜单被点击
const int Constant::TOUCH_CJYY_KUANG_MENU = 13;
//背景音乐菜单被点击
const int Constant::TOUCH_BJYY_KUANG_MENU = 14;

//音乐的开关显示标志位
bool  Constant::isOpen_CJYY = true;
bool  Constant::isOpen_BJYY = true;

//暂停、停止菜单的半宽半高
const float Constant::MENU_STOP_PAUSE_WIDTH_HALF = 0.07;
const float Constant::MENU_STOP_PAUSE_HEIGHT_HALF = 0.07;

//确定、返回菜单的半宽半高
const float Constant::MENU_QD_FH_WIDTH_HALF = 0.2;
const float Constant::MENU_QD_FH_HEIGHT_HALF = 0.12;

//选中框菜单的半宽半高
const float Constant::MENU_KUANG_WIDTH_HALF = 0.08;
const float Constant::MENU_KUANG_HEIGHT_HALF = 0.08;


const float* Constant::mianFXL[5][2] = {//先是位置，然后是法向量
	{getBuffer(0,0.1f,0),getBuffer(0,1,0),},//地面
	{getBuffer(-CHANGJING_WIDTH/2+0.1f,0,0),getBuffer(1,0,0)},//左面
	{getBuffer(CHANGJING_WIDTH/2-0.1f,0,0),getBuffer(-1,0,0)},//右面
	{getBuffer(0,0,-CHANGJING_LENGTH/2+0.1f),getBuffer(0,0,1)},//篮板后面 的面
	{getBuffer(0,0,LANBAN_Z+0.08+0.1f),getBuffer(0,0,1)}//篮板影子平面
};
float* Constant::getBuffer(float x,float y,float z){//出入三个坐标，得到该数组的缓冲
	float* lightLocation=new float[3];
	lightLocation[0]=x;
	lightLocation[1]=y;
	lightLocation[2]=z;
	return lightLocation;
}
//记录的总数
const int Constant::RECORD_COUNT = 10;


//屏幕x坐标到视口x坐标
float Constant::fromScreenXToNearX(float x)
{
	return (x-Constant::SCREEN_WIDTH/2)/(Constant::SCREEN_HEIGHT/2);
}
//屏幕y坐标到视口y坐标
float Constant::fromScreenYToNearY(float y)
{
	return -(y-Constant::SCREEN_HEIGHT/2)/(Constant::SCREEN_HEIGHT/2);
}
