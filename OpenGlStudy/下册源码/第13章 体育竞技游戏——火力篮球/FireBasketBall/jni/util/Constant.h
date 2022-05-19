#ifndef _Constant_H_
#define _Constant_H_
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include <string>
#include <jni.h>

using namespace std;

class Constant
{
public:
	static std::string SDCardPath ;

	static float RADIO ;

	//onViewChange的w,h
	static int w;
	static int h;
	//界面切换的标志位
	static bool SWAP_VIEW_FLAG;

	static const float PI;
	//屋子的长、宽、高
	static const float CHANGJING_WIDTH;
	static const float CHANGJING_HEIGHT;
	static const float CHANGJING_LENGTH;
	//仪表版
	static const float YBB_WIDTH;
	static const float YBB_HEIGHT;

	//摄像机目标位置
	static float CAMERA_X;
	static float CAMERA_Y;
	static float CAMERA_Z;
	//LENGTH;
	static const float DISTANCE;

	//界面索引
	static int VIEW_CUR_GL_INDEX;
	//游戏界面
	static const int VIEW_GAME_INDEX;
	//游戏菜单界面
	static const int VIEW_MENU_INDEX;
	//关于菜单界面
	static const int VIEW_ABOUT_INDEX;
	//设置界面
	static const int VIEW_SET_INDEX;
	//加载界面
	static const int VIEW_LOADING_INDEX;
	//记录界面
	static const int VIEW_RECORD_INDEX;
	//当前需要显示的菜单编号
	static int MENU_CURR_MENUINDEX;
	//开始界面菜单
	static const int MENU_STARTVIEW_START ;
	//游戏结束菜单
	static const int MENU_GAMEVIEW_END ;
	//判断是否是帮助界面的标志位
	static bool isnoHelpView;
	static int shipingJs;//视频线程计时器

	static const float STARTBALL_V[3][3];

	//帮助手的坐标
	static float shouX;
	static float shouY;
	//屏幕尺寸
	static float SCREEN_WIDTH ;
	static float SCREEN_HEIGHT ;

	//屏幕缩放比
	static float SCREEN_WIDTH_SCALE;
	static float SCREEN_HEIGHT_SCALE;

	//篮球半径
	static float BASKETBALL_R;
	//篮球初始位置
//	static float BASKETBALL_ORIGIN_X[3];
//	static float BASKETBALL_ORIGIN_Y[3];
//	static float BASKETBALL_ORIGIN_Z[3];

	static const float LANBAN_X;//篮板的位置x坐标
	static const float LANBAN_Y;//篮板的位置y坐标
	static const float LANBAN_Z;//篮板的位置z坐标

	static const float LANKUANG_X;//篮板的位置x坐标
	static const float LANKUANG_Y;//篮板的位置y坐标
	static const float LANKUANG_Z;//篮板的位置z坐标

	static const float LANKUANG_R;//篮筐半径

	static const float LANWANG_H;//篮网的高度

	static float gFactor;//重力加速度缩放比例
	static float vFactor;//y方向速度缩放比例

	//屏幕起始的X,Y
	static float sXtart;
	static float sYtart;

	//仪表板中单个数字的大小
	static float const SHUZI_KUANDU;
	static float const SHUZI_GAODU;

	static const float STARTBALL_1[3];
	static const float STARTBALL_2[3] ;
	static const float STARTBALL_3[3] ;
	static const float STARTBALL[3][3] ;

	//得分
	static int DIGITAL_SCORE;
	//总时间
	static float DIGITAL_TOTAL_TIME;
	//消耗的时间
	static float DIGITAL_TIME;
	//游戏时间结束标志位
	static bool GAMEOVER;


	//每个菜单的间距
	static const float MENU_SPAN ;
	//每个菜单矩形的长
	static const float MENU_HEIGHT_HALF ;
	//每个菜单矩形的宽
	static const float MENU_WIDTH_HALF ;

	//被点击的菜单编号
	static int TOUCHED_CURR_MENU;
	//是否被点击的标志位---数组长度为菜单种类
	static bool isTouched[15];
	static const float MENU_SACLE_X ;
	static const float MENU_SACLE_Y ;
	static const float MENU_SACLE_Z ;
	//没有游戏菜单被点击
	static const int TOUCHED_NONE_MENU;
	//开始游戏菜单被点击
	static const int TOUCHED_KSYX_MENU;
	//设置游戏菜单被点击
	static const int TOUCHED_SZ_MENU;
	//关于游戏菜单被点击
	static const int TOUCHED_GY_MENU;
	//帮助游戏菜单被点击
	static const int TOUCHED_BZ_MENU;
	//记录游戏菜单被点击
	static const int TOUCHED_JL_MENU;
	//退出游戏菜单被点击
	static const int TOUCHED_TC_MENU;
	//再来一局游戏菜单被点击
	static const int TOUCHED_ZLYJ_MENU;
	//返回菜单游戏菜单被点击
	static const int TOUCHED_FHCD_MENU;

	//确定菜单游戏菜单被点击
	static const int TOUCHED_QD_MENU;
	//返回菜单游戏菜单被点击
	static const int TOUCHED_FH_MENU;

	//停止菜单被点击
	static const int TOUCH_STOP_MENU;
	//暂停菜单被点击
	static const int TOUCH_PAUSE_MENU;

	//场景音乐菜单被点击
	static const int TOUCH_CJYY_KUANG_MENU;
	//背景音乐菜单被点击
	static const int TOUCH_BJYY_KUANG_MENU;
	//音乐的开关显示标志位
	static bool isOpen_CJYY;
	static bool isOpen_BJYY;

	//暂停、停止菜单的半宽半高
	static const float  MENU_STOP_PAUSE_WIDTH_HALF;
	static const float  MENU_STOP_PAUSE_HEIGHT_HALF;

	//确定、返回菜单的半宽半高
	static const float  MENU_QD_FH_WIDTH_HALF;
	static const float  MENU_QD_FH_HEIGHT_HALF;

	//选中框菜单的半宽半高
	static const float  MENU_KUANG_WIDTH_HALF;
	static const float  MENU_KUANG_HEIGHT_HALF;

	static const float* mianFXL[5][2];//先是位置，然后是法向量
	static float* getBuffer(float x,float y,float z);//出入三个坐标，得到该数组的缓冲

	//记录的总数
	static const int RECORD_COUNT;

	//屏幕x坐标到视口x坐标
	static float fromScreenXToNearX(float x);
	//屏幕y坐标到视口y坐标
	static float fromScreenYToNearY(float y);
};

#endif
