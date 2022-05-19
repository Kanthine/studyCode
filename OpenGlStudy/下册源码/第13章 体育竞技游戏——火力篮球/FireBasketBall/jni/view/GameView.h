#ifndef _GameView_H_
#define _GameView_H_
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <jni.h>
#include "util/Constant.h"

#include "Bullet/LinearMath/btAlignedObjectArray.h"
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/BulletSoftBody/btSoftBody.h"
#include "Bullet/BulletSoftBody/btSoftBodyHelpers.h"
#include "Bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "Bullet/BulletSoftBody/btSoftBodySolvers.h"
#include "Bullet/BulletSoftBody/btDefaultSoftBodySolver.h"
#include "Bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"

#include "MyBullet/TexBody.h"
#include "MyBullet/TextureRect.h"
#include "MyBullet/TexBall.h"
#include "MyBullet/Digital.h"
#include "MyBullet/SoftObj.h"

#include <vector>
#include <thread>

#define BIT(x) (1<<(x))//定义宏，表示1的左移操作
enum collisiontypes {
	COL_NOTHING = 0, //<Collide with nothing//表示什么都不碰撞
	COL_LEFT_WALL = BIT(1),//左墙
	COL_RIGHT_WALL = BIT(2),//右墙
	COL_DOWN_WALL = BIT(3),//下墙
	COL_FRONT_WALL = BIT(4),//前墙
	COL_LANBAN_WALL = BIT(5),//篮板
	COL_LANKUANG_WALL = BIT(6),//篮筐
	COL_LANWANG = BIT(7),//篮网
	COL_BALL = BIT(8)//球//表示与球碰撞
};

class GameView
{
public:
	GameView();
	~GameView(){};
	//初始化该界面的数据
	void initGameViewData();
	//创建时候被调用
	void onViewCreated(JNIEnv * env,jobject obj);
	void onViewchanged(int w, int h);
	//画自己
	void drawSelf();
	//画仪表板
	void drawDeshBoard();
	//初始化纹理矩形
	void initTextureRect();
	//初始化着色器
	void initShader();
	//初始化纹理Id
	void initTextureId(JNIEnv * env,jobject obj);
	//计算相机位置
	void callCamera();
	//角度转弧度
	float degreesToRadians(float angle);
	//初始化物理世界
	void initPhysicalWorld();
	//清除储存向量
	void cleanVector();
	//绘制物理世界
	void drawPhysicalWorld();
	//初始化物理世界物体
	void initPhysicalWorldBody();
    //触摸回调方法
    void onTouchBegan(float touchX,float touchY);
    void onTouchMoved(float touchX,float touchY);
    void onTouchEnded(float touchX,float touchY);
    //线程
    void threadTask();

    void ballControlUtil();//判断篮球是否进球

    //绘制帮助界面
    void onDrawShiping();

    //帮助时候的线程
    void basketBallForHelpThread();

    int judgeMenuTouched(float touchX,float touchY);

    void playSound(int sound,int loop);
public:
	float ratio ;

	TexBody* downWall;
	TexBody* frontWall;
	TexBody* leftWall;
	TexBody* rightWall;
	TexBody* lanbanWall;
	TexBall* basketball_texBody[3];
	SoftObj* basketBallNet;
	TexBody* lanKuang;
	//篮网的纹理Id
	GLuint lanwangTexId;

	GLuint frontTexId;		//前纹理Id
	GLuint downTexId;		//下纹理Id
	GLuint leftTexId;		//左纹理Id
	GLuint rightTexId;		//右纹理Id
	GLuint ballTexId;		//篮球纹理Id

	TextureRect* ybb;		//仪表板纹理Id
	GLuint shijianxiansBeijingId;		//纹理纹理Id

	GLuint lanbanTexId;		//篮板的Id
	GLuint lankuangId;		//篮框的Id
	GLuint shuziId;			//数字纹理的Id

	GLuint tingZTexId;		//停止菜单的纹理Id
	TextureRect* tingZTexRect;//停止菜单的纹理矩形
	GLuint zhanTTexId;	//暂停菜单纹理Id
	GLuint bofangTexId;	//播放菜单纹理Id

	TextureRect* BZWZTexRect;//帮助文字纹理矩形
	GLuint BZwenziTexId;		//帮助文字的纹理Id
	float BZWZTexRect_Y;

	TextureRect* shouTexRect;//帮助文字纹理矩形
	GLuint shouTexId;		//手掌纹理Id

	//仰角
	float xAngle;
	float yAngle;
	//摄像机位置
	float cx;
	float cy;
	float cz;
	//目标点位置
	float tx;
	float ty;
	float tz;
	//物理世界参数
	//碰撞配置---在管线各个阶段尝试不同的算法组合
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	//碰撞检测算法分配器---调度器对象
	btCollisionDispatcher* m_dispatcher;
	//阶段碰撞检测-----尽量剔除没有相互作用的物体对
	btBroadphaseInterface* m_broadphase;
	//创建解算器(solver),解算器就是求解约束方程，得到物体在一些外力约束情况下（比如重力）最终的位置，速度等。
	btConstraintSolver* m_solver;
	//物理世界
	btDynamicsWorld* m_dynamicsWorld;
	vector<TexBody*> tca;	//存储刚体封装类对象的vector（包括 立方体，球体，地面）

	//软体物理世界
	btSoftBodyWorldInfo	m_softBodyWorldInfo;
	btSoftRigidDynamicsWorld*	m_softRigidDynamicsWorld;



	vector<TexBall*> tball_vector;	//存储篮球对象的vector

	TexBall* curr_ball;//临时篮球的引用---当时触摸到的篮球

	//记录开始触摸时的X,Y坐标
	float touchXOrigin ;
	float touchYOrigin ;
	//数字
	Digital* digital;

	thread* t;
	bool threadFlag;
	//帮助界面-演示玩法
	thread* helpThred;
	//线程标志位
	bool helpThreadFlag;
	//是否暂停帮助界面的播放
	bool isnoPlay;

	bool isStart;//是否开始绘制游戏场景了
	//是否为帮助界面暂停
	bool isHelpPause;

    JNIEnv *env;
    jclass cls;
    jmethodID mid;

};

#endif
