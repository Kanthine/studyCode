#include "GameView.h"
#include "util/MatrixState.h"

#include "util/ShaderManager.h"
#include <math.h>

#include "util/Constant.h"

#include "MyBullet/TexCuboid.h"
#include "MyBullet/TexTriangleMesh.h"
#include <iostream>
#include <stdlib.h>
#include "util/SYSUtil.h"

#include "SQLiteUtil/SQLiteUtil.h"


using namespace std;
#include "FireBasketBallcpp/main.h"

GameView::GameView()
{

}
//初始化该界面的数据
void GameView::initGameViewData()
{
	//仰角
	xAngle=0.0f;
	yAngle=0.0f;
	//摄像机位置
	cx = Constant::CAMERA_X;
	cy = Constant::CAMERA_Y;
	cz = Constant::CAMERA_Z;
	//目标点位置
	tx = 0;
	ty = Constant::CAMERA_Y;
	tz = 0;
	callCamera();
	//初始化当前摸中的篮球为空
	curr_ball = NULL;
	threadFlag = true;
	Constant::GAMEOVER = false;
	BZWZTexRect_Y = -1.5;
	isStart=false;//是否开始绘制游戏场景了
	//帮助界面-演示玩法
	helpThreadFlag = true;
	isnoPlay = true;
	//是否为帮助界面暂停
	isHelpPause = false;
	Constant::DIGITAL_SCORE = 0;
	Constant::DIGITAL_TIME = 0;
	Constant::DIGITAL_TOTAL_TIME = 60;
	int ballIndex =0;
	std::vector<TexBall*>::iterator ballIterator ;
	for(ballIterator = tball_vector.begin();ballIterator!=tball_vector.end();ballIterator++)
	{
		btTransform btt ;
		btt.setIdentity();
		btt.setOrigin(btVector3(Constant::STARTBALL[ballIndex][0],Constant::STARTBALL[ballIndex][1],Constant::STARTBALL[ballIndex][2]));
		(*ballIterator)->getBody()->setCenterOfMassTransform(btt);
		(*ballIterator)->getBody()->activate(false);
		(*ballIterator)->getBody()->clearForces();
		ballIndex++;
	}
	{
		t = new thread(&GameView::threadTask,this);
	}
}
//被创建
void GameView::onViewCreated(JNIEnv * env,jobject obj)
{
	//设置背景颜色
	glClearColor(0.5f, 0.5f, 0.5f, 1);
    //初始化光源位置
    MatrixState::setLightLocation(3, 7, 5);
	//初始化变换矩阵
    MatrixState::setInitStack();
}

//被改变
void GameView::onViewchanged(int w, int h)
{
	//设置视口
    glViewport(0, 0, w, h);
    //计算宽长比
    ratio = (float) w/h;
    //打开背面剪裁
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

}
//画自己
void GameView::drawSelf()
{
    //打开背面剪裁
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
	//设置背景颜色
	glClearColor(0.5f, 0.5f, 0.5f, 1);
    //初始化光源位置
    MatrixState::setLightLocation(3, Constant::CHANGJING_HEIGHT*1.7f, 5);
	//初始化变换矩阵
    MatrixState::setInitStack();
	//清缓存
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //调用此方法计算产生透视投影矩阵
    MatrixState::setProjectFrustum(-ratio, ratio, -1, 1, 4.0f, 100);
    //相机位置(0,3.15,14.9)目标位置是篮框(0,3.15,0)
    MatrixState::setCamera(cx, cy, cz, tx, ty,tz, 0, 1, 0);
    //保护场景
    MatrixState::pushMatrix();
    //开启混合
    glEnable(GL_BLEND);
    //设置混合因子
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //绘制物理世界
    drawPhysicalWorld();

    //绘制仪表板
    //关闭混合
    glDisable(GL_BLEND);
    //画帮助界面
    onDrawShiping();
    drawDeshBoard();

    MatrixState::popMatrix();
    if(!isStart)
    {
    	isStart = true;
    	if(Constant::isnoHelpView)
		{
			//初速度
			float vx = Constant::STARTBALL_V[1][0];
			float vy = Constant::STARTBALL_V[1][1];
			float vz = Constant::STARTBALL_V[1][2];

			tball_vector.at(1)->getBody()->activate(true);
			tball_vector.at(1)->getBody()->setLinearVelocity(btVector3(vx,vy,vz));
			tball_vector.at(1)->getBody()->setAngularVelocity(btVector3(5,0,0));
			//开启一个线程---演示玩法
			helpThred = new thread(&GameView::basketBallForHelpThread,this);
		}
    }
}
void GameView::drawPhysicalWorld()
{
	MatrixState::pushMatrix();
    for ( int i = 0; i < tca.size(); ++i )
    {
    	tca[i]->drawSelf();//回调刚体的绘制方法
    }
    for(int i=0;i<3;i++)
    {
    	basketball_texBody[i]->drawSelf(ballTexId,0,0,0);	//绘制篮球本身
    }
    for(int i=0;i<3;i++)
	{
		basketball_texBody[i]->drawSelf(ballTexId,1,0,0);	//绘制地面阴影
		basketball_texBody[i]->drawSelf(ballTexId,1,3,0);	//绘制背后墙板的阴影
		basketball_texBody[i]->drawSelf(ballTexId,1,4,1);	//绘制篮板阴影
	}
    glDisable  (GL_CULL_FACE);
    //开启混合
    glEnable(GL_BLEND);
    //设置混合因子
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    MatrixState::pushMatrix();
    basketBallNet->drawSelf(this->lanwangTexId);
    MatrixState::popMatrix();
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    MatrixState::popMatrix();
}

void GameView::initPhysicalWorldBody()
{
	int ballCollidesWith = COL_LEFT_WALL | COL_RIGHT_WALL | COL_DOWN_WALL | COL_FRONT_WALL | COL_LANBAN_WALL | COL_LANKUANG_WALL | COL_LANWANG ;	//球与所有碰撞
	int lanBanCollidesWith = COL_BALL ;		//篮板只与球碰撞
	int lanKuangCollidesWith = COL_BALL;	//篮筐只与球碰撞
	int lanWangCollidesWith = COL_LEFT_WALL | COL_RIGHT_WALL | COL_DOWN_WALL | COL_BALL;	//篮筐只与球碰撞

	int downWallCollidesWith = COL_BALL | COL_LANWANG;	//篮筐只与球碰撞
	int leftWallCollidesWith = COL_BALL | COL_LANWANG;	//篮筐只与球碰撞
	int rightWallCollidesWith = COL_BALL | COL_LANWANG;	//篮筐只与球碰撞
	int frontWallCollidesWith = COL_BALL | COL_LANWANG;	//篮筐只与球碰撞

	//篮球场
	TexBody* texBody;
	cleanVector();

	{
		downWall = new TexCuboid(
				m_softRigidDynamicsWorld,//物理世界对象
				btVector3(Constant::CHANGJING_WIDTH/2,0.01,Constant::CHANGJING_LENGTH/2),//长方体的半区域
				0.0f,//长方体的质量
				btVector3(0,0,0),//刚体的位置
				0.75f,//恢复系数
				0.8f,//摩擦系数
				1,//单位长度
				downTexId,downTexId,downTexId,
				downTexId,downTexId,downTexId
		);
		m_softRigidDynamicsWorld->removeRigidBody(downWall->getBody());
		m_softRigidDynamicsWorld->addRigidBody(downWall->getBody(),COL_DOWN_WALL,downWallCollidesWith);
		//将新立方体加入到列表中
		tca.push_back(downWall);

		frontWall = new TexCuboid(
				m_softRigidDynamicsWorld,//物理世界对象
				btVector3(Constant::CHANGJING_WIDTH/2,(Constant::CHANGJING_HEIGHT-0.2)/2,0.01),//长方体的半区域
				0.0f,//长方体的质量
				btVector3(0,(Constant::CHANGJING_HEIGHT-0.2)/2,-Constant::CHANGJING_LENGTH/2),//刚体的位置
				0.2f,//恢复系数
				0.8f,//摩擦系数
				1,//单位长度
				frontTexId,frontTexId,frontTexId,
				frontTexId,frontTexId,frontTexId
		);
		m_softRigidDynamicsWorld->removeRigidBody(frontWall->getBody());
		m_softRigidDynamicsWorld->addRigidBody(frontWall->getBody(),COL_FRONT_WALL,frontWallCollidesWith);

		//将新立方体加入到列表中
		tca.push_back(frontWall);

		texBody = new TexCuboid(
				m_softRigidDynamicsWorld,//物理世界对象
				btVector3(Constant::CHANGJING_WIDTH/2,3.5,0.01),//长方体的半区域
				0.0f,//长方体的质量
				btVector3(0,3.5,2),//刚体的位置
				0.0f,//恢复系数
				0.0f,//摩擦系数
				1,//单位长度
				0,0,0,
				0,0,0
		);
		//将新立方体加入到列表中
		tca.push_back(texBody);

		leftWall = new TexCuboid(
				m_softRigidDynamicsWorld,//物理世界对象
				btVector3(0.01,(Constant::CHANGJING_HEIGHT-0.2)/2,Constant::CHANGJING_LENGTH/2),//长方体的半区域
				0.0f,//长方体的质量
				btVector3(-Constant::CHANGJING_WIDTH/2,(Constant::CHANGJING_HEIGHT-0.2)/2,0),//刚体的位置
				0.2f,//恢复系数
				0.8f,//摩擦系数
				1,//单位长度
				leftTexId,leftTexId,leftTexId,
				leftTexId,leftTexId,leftTexId
		);
		m_softRigidDynamicsWorld->removeRigidBody(leftWall->getBody());
		m_softRigidDynamicsWorld->addRigidBody(leftWall->getBody(),COL_LEFT_WALL,leftWallCollidesWith);

		//将新立方体加入到列表中
		tca.push_back(leftWall);

		rightWall = new TexCuboid(
				m_softRigidDynamicsWorld,//物理世界对象
				btVector3(0.01,(Constant::CHANGJING_HEIGHT-0.2)/2,Constant::CHANGJING_LENGTH/2),//长方体的半区域
				0.0f,//长方体的质量
				btVector3(Constant::CHANGJING_WIDTH/2,(Constant::CHANGJING_HEIGHT-0.2)/2,0),//刚体的位置
				0.2f,//恢复系数
				0.8f,//摩擦系数
				1,//单位长度
				rightTexId,rightTexId,rightTexId,
				rightTexId,rightTexId,rightTexId
		);
		m_softRigidDynamicsWorld->removeRigidBody(rightWall->getBody());
		m_softRigidDynamicsWorld->addRigidBody(rightWall->getBody(),COL_RIGHT_WALL,rightWallCollidesWith);

		//将新立方体加入到列表中
		tca.push_back(rightWall);

		lanbanWall = new TexCuboid(
				m_softRigidDynamicsWorld,//物理世界对象
				btVector3(1.05,0.8,0.02),//长方体的半区域
				0.0f,//长方体的质量
				btVector3(0,Constant::LANBAN_Y,Constant::LANBAN_Z),//刚体的位置
				0.5f,//恢复系数
				0.8f,//摩擦系数
				1,//单位长度
				lanbanTexId,lanbanTexId,lanbanTexId,
				lanbanTexId,lanbanTexId,lanbanTexId
		);
		m_softRigidDynamicsWorld->removeRigidBody(lanbanWall->getBody());
		m_softRigidDynamicsWorld->addRigidBody(lanbanWall->getBody(),COL_LANBAN_WALL,lanBanCollidesWith);
		//将新立方体加入到列表中
		tca.push_back(lanbanWall);

		lanKuang = new TexTriangleMesh(
			0.0f,//质量
			TexTriangleMesh::vertices_yuanhuan,
			TexTriangleMesh::numsVer_yuanhuan ,//顶点坐标数组的长度
			TexTriangleMesh::indices_yuanhuan ,
			TexTriangleMesh::numsInd_yuanhuan ,
			TexTriangleMesh::texs_yuanhuan ,
			TexTriangleMesh::numsTex_yuanhuan ,
			TexTriangleMesh::normal_yuanhuan ,
			TexTriangleMesh::numNormal_yuanhuan ,
			1.0f,//缩放因子
			btVector3(Constant::LANKUANG_X,Constant::LANKUANG_Y,Constant::LANKUANG_Z),//位置
			0.5f,//恢复系数
			0.5f,//摩擦系数
			m_softRigidDynamicsWorld,//物理世界指针
			lankuangId//三角形纹理id
			);
		m_softRigidDynamicsWorld->removeRigidBody(lanKuang->getBody());
		m_softRigidDynamicsWorld->addRigidBody(lanKuang->getBody(),COL_LANKUANG_WALL,lanKuangCollidesWith);

		tca.push_back(lanKuang);//将新立方体加入到列表中
	}
	{
		basketBallNet = new SoftObj(
			m_softRigidDynamicsWorld,
			m_softBodyWorldInfo,
			btVector3(Constant::LANKUANG_X,Constant::LANWANG_H,Constant::LANKUANG_Z),
//			btVector3(0,0,0),
			SoftObj::vertices_soft,
			SoftObj::numsVer_soft,
			SoftObj::indices_soft,
			SoftObj::numsInd_soft,
			SoftObj::texs_soft,
			SoftObj::numsTex_soft,
			1.0f
			);
		m_softRigidDynamicsWorld->removeSoftBody(basketBallNet->getBody());
		m_softRigidDynamicsWorld->addSoftBody(basketBallNet->getBody(),COL_LANWANG,lanWangCollidesWith);
		//固定一些点
		for(int i=1;i<=24;i++)
		{
			basketBallNet->getBody()->appendAnchor(basketBallNet->getBody()->m_nodes.size()-i,lanKuang->getBody());
		}
	}
	{
		for(int i=0;i<3;i++)
		{
			//球体
			basketball_texBody[i]=new TexBall(Constant::BASKETBALL_R,ballTexId,m_softRigidDynamicsWorld,5.0,
					btVector3(Constant::STARTBALL[i][0],Constant::STARTBALL[i][1],Constant::STARTBALL[i][2]),
					0.75, 0.5);
			m_softRigidDynamicsWorld->removeRigidBody(basketball_texBody[i]->getBody());
			m_softRigidDynamicsWorld->addRigidBody(basketball_texBody[i]->getBody(),COL_BALL,ballCollidesWith);
			//存放到篮球列表中
			tball_vector.push_back(basketball_texBody[i]);
		}
	}
}
void GameView::threadTask()
{
    //Attach主线程
    if((*main::g_jvm).AttachCurrentThread(&env, NULL) != JNI_OK)
    {
    	return ;
    }
    cls = env->GetObjectClass(main::g_obj);

    //再获得类中的方法
    mid = env->GetStaticMethodID(cls,"shengyinBoFang","(II)V");

	while(threadFlag)
	{
		if(Constant::VIEW_CUR_GL_INDEX != Constant::VIEW_GAME_INDEX)
		{
			threadFlag = false;
		     //Detach主线程
		     if((*main::g_jvm).DetachCurrentThread() != JNI_OK)
		     {
		     }
		}
	    m_softRigidDynamicsWorld->stepSimulation(1.0/60,5);//进行物理模拟计算
		ballControlUtil();//判断篮球是否进球

		if(!Constant::GAMEOVER&&!Constant::isnoHelpView)
		{
			Constant::DIGITAL_TIME += 10;
		}
		if(Constant::DIGITAL_TIME>=Constant::DIGITAL_TOTAL_TIME*1000)
		{
			//结束线程
			threadFlag = false;
			Constant::GAMEOVER = true;
			//插入游戏记录
			std::string sSQL1 = "INSERT INTO paihangbang VALUES('";
			//得分
			char str[10];
			//整形转字符串
			sprintf(str,"%d",Constant::DIGITAL_SCORE);
			sSQL1 = sSQL1 + str ;
		   	std::string sSQL2 = "','";

		   	//当前时间
		   	time_t tt = time(NULL);
		   	//这句返回的只是一个时间cuo
		   	tm* t= localtime(&tt);

		   	int yearTemp = t->tm_year + 1900;
		   	int monthTemp =t->tm_mon + 1;
		   	int dayTemp =t->tm_mday;
		   	int hourTemp = t->tm_hour;
		   	int minTemp = t->tm_min;
		   	int secTemp = t->tm_sec;
		   	//时间数组
		   	char strTime[20];
			//整形转字符串
			sprintf(strTime,"%d-%02d %02d:%02d:%02d",
					monthTemp,
					dayTemp,
					hourTemp,
					minTemp,
					secTemp);
		   	sSQL2 = sSQL2 + strTime;
		   	std::string sSQL3 = "')";;
			std::string sSQL = sSQL1 + sSQL2 + sSQL3;
			//插入得分和时间记录
		    SQLiteUtil::doSqlite3_exec(sSQL);
			//设置菜单界面
			Constant::VIEW_CUR_GL_INDEX = Constant::VIEW_MENU_INDEX;
			//当前显示游戏结束界面
			Constant::MENU_CURR_MENUINDEX = Constant::MENU_GAMEVIEW_END;

			if(!basketball_texBody[0]->getBody()->isActive()&&!basketball_texBody[1]->getBody()->isActive()&&!basketball_texBody[2]->getBody()->isActive())
			{
				threadFlag = false;//现场停止
			}
			playSound(2,0);
		     //Detach主线程
		     if((*main::g_jvm).DetachCurrentThread() != JNI_OK)
		     {
		     }else
		     {
		     }
		 }
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
void GameView::playSound(int sound,int loop)
{
  if(mid != NULL && Constant::isOpen_CJYY)
	{
    	env->CallStaticVoidMethod(cls, mid,(jint)sound,(jint)loop);
	}
}
void GameView::ballControlUtil()//这里主要是用于对球的物理引擎部分进行操作
{
	std::vector<TexBall*>::iterator ballIterator ;
	for(ballIterator = tball_vector.begin();ballIterator!=tball_vector.end();ballIterator++)
	{
		//如果篮球是运动的,那么每一次跟地板碰撞的时候给篮球增加一个沿Z轴正方向的冲量
		if((*ballIterator)->getBody()->isActive()&&SYSUtil::isCollided(m_softRigidDynamicsWorld,(*ballIterator)->getBody(),downWall->getBody()))
		{
			(*ballIterator)->getBody()->applyForce(btVector3(0,0,60),btVector3(0,0,0));
		}

		//获取篮球的运动组件
		btTransform transform;
		(*ballIterator)->getBody()->getMotionState()->getWorldTransform(transform);//------------------------------可能有错------error

		float position_X = transform.getOrigin().x();
		float position_Y = transform.getOrigin().y();
		float position_Z = transform.getOrigin().z();
		//获取篮球的速度和旋转
		btVector3 linearVelocity = (*ballIterator)->getBody()->getLinearVelocity();
		btVector3 angularVelocity = (*ballIterator)->getBody()->getAngularVelocity();
		//这里对篮球的进框进行处理
		//这里获取篮筐的中心位置坐标值
		float lankuang_X = Constant::LANKUANG_X;
		float lankuang_Y = Constant::LANKUANG_Y;
		float lankuang_Z = Constant::LANKUANG_Z;

		float lankuang_Radius = Constant::LANKUANG_R;
		float ball_Radius = Constant::BASKETBALL_R;
		//如果篮球刚好位于篮筐中
		float temp_distance = (float)sqrt((position_X-lankuang_X)*(position_X-lankuang_X)+(position_Z-lankuang_Z)*(position_Z-lankuang_Z));
		if(linearVelocity.y()<0&&temp_distance<(lankuang_Radius-ball_Radius)&&position_Y>lankuang_Y)
		{
			(*ballIterator)->ballState = 1;
		}

		if((*ballIterator)->ballState==1
							&&position_Y<lankuang_Y
							&&position_X>lankuang_X-Constant::LANKUANG_R&&position_X<lankuang_X+Constant::LANKUANG_R&&
							position_Z>lankuang_Z-Constant::LANKUANG_R&&position_Z<lankuang_Z+Constant::LANKUANG_R
					)
		{
			Constant::DIGITAL_SCORE++;
			//需要加篮网抖动
			playSound(3,0);
			(*ballIterator)->ballState = 0;
		}

		btVector3 vt = (*ballIterator)->getBody()->getLinearVelocity();
		if((vt.y()>1.0f&&vt.y()<6.5f)&&SYSUtil::isCollided(m_softRigidDynamicsWorld,(*ballIterator)->getBody(),downWall->getBody()))//判断是否和地面撞
		{
			playSound(1,0);
		}else if
		(
				vt.x()>1.0f&&
				SYSUtil::isCollided(m_softRigidDynamicsWorld,(*ballIterator)->getBody(),leftWall->getBody())
		)
		{//左面
			//播放撞墙声音
			playSound(1,0);
		}
		else if
		(		vt.x()<-1&&
				SYSUtil::isCollided(m_softRigidDynamicsWorld,(*ballIterator)->getBody(),rightWall->getBody())
		)
		{//右面
			//播放撞墙声音
			playSound(1,0);
		}
		else if
		(
				vt.z()>1.0&&
				SYSUtil::isCollided(m_softRigidDynamicsWorld,(*ballIterator)->getBody(),frontWall->getBody())
		)
		{//后面
			//播放撞墙声音
			playSound(1,0);
		}

		if(SYSUtil::isCollided(m_softRigidDynamicsWorld,(*ballIterator)->getBody(),lanbanWall->getBody()))
		{
			if((abs(vt.x())+abs(vt.y())+abs(vt.z()))>2)
			{
				//播放声音
				playSound(1,0);
			}
			//和篮板前臂碰撞了
			(*ballIterator)->isnoLanBan=1;
		}else
		{
			(*ballIterator)->isnoLanBan=0;
		}
	}
}
void GameView::cleanVector()
{
	tca.clear();
}

//画仪表板
void GameView::drawDeshBoard()
{
	MatrixState::pushMatrix();
    //开启混合
    glEnable(GL_BLEND);
    //设置混合因子
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//设置为正交投影
    MatrixState::setProjectOrtho(-this->ratio, this->ratio, -1, 1, 1, 10);
    MatrixState::setCamera(0, 0, 5, 0, 0,0, 0, 1, 0);

    glEnable(GL_DEPTH_TEST);

	MatrixState::pushMatrix();
    MatrixState::translate(0, 0.85, 0);
	//绘制矩形
    ybb->drawSelf(shijianxiansBeijingId);  //绘制背景
    MatrixState::popMatrix();

    MatrixState::pushMatrix();
    MatrixState::translate(-0.35, 0.80, 0.1);
    digital->drawSelf(Constant::DIGITAL_SCORE,shuziId);
    MatrixState::popMatrix();

    MatrixState::pushMatrix();
    MatrixState::translate(0.35, 0.80, 0.1);
    digital->drawSelf(Constant::DIGITAL_TOTAL_TIME-(Constant::DIGITAL_TIME/1000),shuziId);
    MatrixState::popMatrix();

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    MatrixState::popMatrix();
}
void GameView::initTextureId(JNIEnv * env,jobject obj)
{
	jclass cl = env->FindClass("com/bn/bullet/GL2JNIView");
	jmethodID id = env->GetStaticMethodID(cl,"initTextureRepeat","(Landroid/opengl/GLSurfaceView;Ljava/lang/String;)I");

	jstring name = env->NewStringUTF("pic/swall1.jpg");
	leftTexId = env->CallStaticIntMethod(cl,id,obj,name);

	name = env->NewStringUTF("pic/swall2.jpg");
	frontTexId = env->CallStaticIntMethod(cl,id,obj,name);

	name = env->NewStringUTF("pic/floor.png");
	downTexId = env->CallStaticIntMethod(cl,id,obj,name);

	name = env->NewStringUTF("pic/swall3.jpg");
	rightTexId = env->CallStaticIntMethod(cl,id,obj,name);

	name = env->NewStringUTF("pic/yibiaoban.png");
	shijianxiansBeijingId = env->CallStaticIntMethod(cl,id,obj,name);

	name = env->NewStringUTF("pic/basketball.jpg");
	ballTexId = env->CallStaticIntMethod(cl,id,obj,name);

	name = env->NewStringUTF("pic/lanban.png");
	lanbanTexId = env->CallStaticIntMethod(cl,id,obj,name);

	name = env->NewStringUTF("pic/lankuang.png");
	lankuangId = env->CallStaticIntMethod(cl,id,obj,name);

	name = env->NewStringUTF("pic/number.png");
	shuziId = env->CallStaticIntMethod(cl,id,obj,name);

	name = env->NewStringUTF("pic/wenzi.png");
	BZwenziTexId = env->CallStaticIntMethod(cl,id,obj,name);

	name = env->NewStringUTF("pic/shou.png");
	shouTexId = env->CallStaticIntMethod(cl,id,obj,name);

	//停止纹理Id
	name = env->NewStringUTF("pic/stop.png");
	tingZTexId = env->CallStaticIntMethod(cl,id,obj,name);

	//暂停纹理Id
	name = env->NewStringUTF("pic/pause.png");
	zhanTTexId = env->CallStaticIntMethod(cl,id,obj,name);

	//播放菜单纹理Id
	name = env->NewStringUTF("pic/play.png");
	bofangTexId = env->CallStaticIntMethod(cl,id,obj,name);

	//篮网纹理Id
	name = env->NewStringUTF("pic/basketnet.png");
	lanwangTexId = env->CallStaticIntMethod(cl,id,obj,name);
}
//初始化Object
void GameView::initTextureRect()
{
	//这里编译3D场景中欢迎欢迎界面中的shader
	ShaderManager::compileShader();
	//创建标志板
	ybb = new TextureRect(Constant::RADIO,Constant::YBB_HEIGHT/2,0,1,ShaderManager::getCommTextureShaderProgram(),NULL);	//创建矩形形对象

	BZWZTexRect = new TextureRect(0.4,0.4,0,1,ShaderManager::getCommTextureShaderProgram(),NULL);	//创建矩形形对象

	digital = new Digital(ShaderManager::getCommTextureShaderProgram());

	shouTexRect = new TextureRect(0.1,0.1,0,1,ShaderManager::getCommTextureShaderProgram(),NULL);	//创建矩形形对象

	tingZTexRect = new TextureRect(Constant::MENU_STOP_PAUSE_WIDTH_HALF,Constant::MENU_STOP_PAUSE_HEIGHT_HALF,0,1,ShaderManager::getCommTextureShaderProgram(),NULL);	//创建矩形形对象

}
void GameView::callCamera()
{
	//设置摄像机的位置
    cx=(float)(tx+cos(degreesToRadians(xAngle))*
    		sin(degreesToRadians(yAngle))*Constant::DISTANCE);//摄像机x坐标
    cz=(float)(tz+cos(degreesToRadians(xAngle))*
    		cos(degreesToRadians(yAngle))*Constant::DISTANCE);//摄像机z坐标
    cy=(float)(ty+sin(degreesToRadians(xAngle))*Constant::DISTANCE);//摄像机y坐标
}
float GameView::degreesToRadians(float angle)
{
	float result = angle/180*Constant::PI;
	return result;
}
//初始化物理世界
void GameView::initPhysicalWorld()
{
	//创建碰撞配置---在管线各个阶段尝试不同算法的组合
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	//创建调度对象
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	//创建阶段碰撞检测---剔除没有互相作用的物理对
	m_broadphase = new btSimpleBroadphase();
	//创建解算器----求解约束方程
	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver();
	m_solver = sol;
	//创建物理世界
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	//设置重力加速度
	m_dynamicsWorld->setGravity(btVector3(0,-10,1));

	//软体
	m_softBodyWorldInfo = btSoftBodyWorldInfo();

	//创建碰撞检测配置信息对象
	btSoftBodyRigidBodyCollisionConfiguration *collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
	//创建碰撞检测算法分配者对象，其功能为扫描所有的碰撞检测对，并确定适用的检测策略对应的算法
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	m_softBodyWorldInfo.m_dispatcher = dispatcher;
	//设置整个物理世界的边界信息
	btVector3 worldAabbMin = btVector3(-10000, -10000, -10000);
	btVector3 worldAabbMax = btVector3(10000, 10000, 10000);
	int maxProxies = 1024;
	//创建碰撞检测粗测阶段的加速算法对象
	btAxisSweep3 *overlappingPairCache =new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);
	m_softBodyWorldInfo.m_broadphase = overlappingPairCache;
	//创建推动约束解决者对象
	btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();
	btSoftBodySolver* softBodySolver = new btDefaultSoftBodySolver();
	m_softBodyWorldInfo.m_gravity.setValue(0,-10,0);
	m_softBodyWorldInfo.m_sparsesdf.Initialize();
//		m_softBodyWorldInfo.m_sparsesdf.Reset();
	m_softBodyWorldInfo.air_density		=	(btScalar)0.5;
//		m_softBodyWorldInfo.water_density	=	0;
//		m_softBodyWorldInfo.water_offset	=	0;
//		m_softBodyWorldInfo.water_normal	=	btVector3(0,0,0);

	//创建物理世界对象
	m_softRigidDynamicsWorld = new btSoftRigidDynamicsWorld(dispatcher, overlappingPairCache, solver,collisionConfiguration,softBodySolver);
	//设置重力加速度
	btVector3 gvec = btVector3(0, -10, 0);
	m_softRigidDynamicsWorld->setGravity(gvec);

}
//绘制帮助界面
void GameView::onDrawShiping()
{
	if(!Constant::isnoHelpView)
	{
		return ;
	}

	MatrixState::pushMatrix();
	//设置为正交投影
    MatrixState::setProjectOrtho(-this->ratio, this->ratio, -1, 1, 1, 100);
    MatrixState::setCamera(0, 0, 5, 0, 0,1, 0, 1, 0);
    //开启混合
    glEnable(GL_BLEND);
    //设置混合因子
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //关闭深度测试
    glDisable(GL_DEPTH_TEST);

    MatrixState::pushMatrix();
    MatrixState::translate(-0.48, -0.9, 0);
    tingZTexRect->drawSelf(tingZTexId);
    MatrixState::popMatrix();

    MatrixState::pushMatrix();
    MatrixState::translate(0.48, -0.9, 0);
	if(!isHelpPause)
	{
		tingZTexRect->drawSelf(zhanTTexId);
	}else
	{
		tingZTexRect->drawSelf(bofangTexId);
	}

    MatrixState::popMatrix();

	MatrixState::pushMatrix();
	MatrixState::translate(Constant::shouX, Constant::shouY, 0.8);
	shouTexRect->drawSelf(shouTexId);
	MatrixState::popMatrix();

	MatrixState::pushMatrix();
	if(!isHelpPause)
	{
		BZWZTexRect_Y = BZWZTexRect_Y + 0.001;
	}
	MatrixState::translate(0, BZWZTexRect_Y, 0);
	BZWZTexRect->drawSelf(BZwenziTexId);
	MatrixState::popMatrix();

	//开启深度测试
	glEnable(GL_DEPTH_TEST);
    //关闭混合
    glDisable(GL_BLEND);
    MatrixState::popMatrix();
}
//触摸回调方法
void GameView::onTouchBegan(float touchX,float touchY)
{
	if(Constant::GAMEOVER)
	{
		return ;
	}
	this->touchXOrigin = touchX;
	this->touchYOrigin = touchY;

	//屏幕x坐标到视口x坐标
	float tx = Constant::fromScreenXToNearX(this->touchXOrigin);
	//屏幕y坐标到视口y坐标
	float ty = Constant::fromScreenYToNearY(this->touchYOrigin);

	//触摸点转为3D空间中的x,y
	float x3d = Constant::CHANGJING_WIDTH*((touchXOrigin)/Constant::SCREEN_WIDTH) - 0.5*Constant::CHANGJING_WIDTH;

	float y3d = Constant::CHANGJING_HEIGHT*(Constant::SCREEN_HEIGHT-(touchYOrigin))/Constant::SCREEN_HEIGHT;
	std::vector<TexBall*>::iterator ballIterator ;
	for(ballIterator = tball_vector.begin();ballIterator!=tball_vector.end();ballIterator++)
	{
		//获取篮球的位置
		float ball_X = (*ballIterator)->getBody()->getWorldTransform().getOrigin().getX();
		float ball_Y = (*ballIterator)->getBody()->getWorldTransform().getOrigin().getY();
		float ball_Z = (*ballIterator)->getBody()->getWorldTransform().getOrigin().getZ();

		//篮球半径放大1.5倍，方便触摸
		float ball_scale = 1.5f*Constant::BASKETBALL_R;

		if(x3d<ball_X+ball_scale&&x3d>ball_X-ball_scale
				&&y3d<ball_Y+ball_scale&&y3d>ball_Y-ball_scale
				&&ball_Z>1.55f
				)
		{
			//记录被点击的篮球
			curr_ball = (*ballIterator);
			break;
		}
	}
	//如果没有选中球
	if(curr_ball == NULL)
	{

	}
	if(Constant::isnoHelpView)
	{
		//获取哪个纹理矩形菜单被选中
		int result = judgeMenuTouched(tx,ty);
		//记录当前选中的菜单
		Constant::TOUCHED_CURR_MENU = result;
	}
}
void GameView::onTouchMoved(float touchX,float touchY){}
void GameView::onTouchEnded(float touchX,float touchY)
{
	//屏幕x坐标到视口x坐标
	float tx = Constant::fromScreenXToNearX(touchX);
	//屏幕y坐标到视口y坐标
	float ty = Constant::fromScreenYToNearY(touchY);

	//差值
	float dx = (touchX - touchXOrigin);
	//最大的触摸
	float max_fingerTouch = 110*0.9*Constant::SCREEN_HEIGHT_SCALE;
	float dy=(touchY-touchYOrigin)>0?0:((touchY-touchYOrigin)<-max_fingerTouch?-max_fingerTouch:(touchY-touchYOrigin));//Y方向上的移动距离
	if(curr_ball != NULL)//如果有球被触摸到
	{
		float vTZ = 1.0f;
		btVector3 linearVelocity = curr_ball->getBody()->getLinearVelocity();
		btScalar btvx =	linearVelocity.x();
		btScalar btvy = linearVelocity.y();
		btScalar btvz = linearVelocity.z();
		if(linearVelocity.x()>-vTZ&&linearVelocity.x()<vTZ
				&&linearVelocity.y()<vTZ&&linearVelocity.y()>-vTZ&&
				linearVelocity.z()>-vTZ&&linearVelocity.z()<vTZ
				)
		{
	           	float vx = dx*10/Constant::SCREEN_WIDTH;
	           	float vy = -dy*76*2*Constant::vFactor/Constant::SCREEN_HEIGHT;
	           	float vz = dy*28*2/Constant::SCREEN_HEIGHT;

	           	curr_ball->getBody()->activate(true);
	           	curr_ball->getBody()->setLinearVelocity(btVector3(vx,vy,vz));
	           	curr_ball->getBody()->setAngularVelocity(btVector3(5,0,0));
	           	curr_ball= NULL;
		}
	}
	curr_ball= NULL;
	if(Constant::isnoHelpView)
	{
		//获取哪个纹理矩形菜单被选中
		int result = judgeMenuTouched(tx,ty);
		//记录当前选中的菜单
		if(Constant::TOUCHED_CURR_MENU == result)
		{
			switch(Constant::TOUCHED_CURR_MENU)
			{
			case 11://执行停止菜单被点击的代码
				helpThreadFlag = false;
				Constant::isnoHelpView = false;
				Constant::DIGITAL_SCORE = 0;
				break;
			case 12://执行暂停菜单被点击的代码
				isHelpPause = !isHelpPause;
				break;
			}
		}
	}
}
int GameView::judgeMenuTouched(float touchX,float touchY)
{
	//暂停、停止菜单的半宽半高---170是纹理矩形从左侧到右侧的间距
	for(int i = -1;i<2;i++)
	{
		if(i == 0)
		{
			continue;
		}
		if(touchX<(i*0.48+Constant::MENU_STOP_PAUSE_WIDTH_HALF)
				&&touchX>(i*0.48-Constant::MENU_STOP_PAUSE_WIDTH_HALF))
		{
			if(touchY<(- 0.9 + Constant::MENU_STOP_PAUSE_HEIGHT_HALF )
					&&touchY>(- 0.9 - Constant::MENU_STOP_PAUSE_HEIGHT_HALF ))
			{
				switch(i)
				{
				case -1:return Constant::TOUCH_STOP_MENU;
				case 1:return Constant::TOUCH_PAUSE_MENU;
				}
			}
		}
	}
	return Constant::TOUCHED_NONE_MENU;
}

void GameView::basketBallForHelpThread()
{
	int array_id=0;//发射第几个球
	bool isnoFashe=false;//是否可以进行手的动画

	while(helpThreadFlag)
	{
		if(Constant::VIEW_CUR_GL_INDEX != Constant::VIEW_GAME_INDEX)
		{
			helpThreadFlag = false;
		}
		if(!isnoPlay){//如果是暂停界面
			continue;
		}
		array_id = array_id%3;
		Constant::shipingJs += 100;//记录播放时间
		//如果不可以进行手的动画，并且时间没有达到4秒
		if(!isnoFashe&&(Constant::shipingJs)%5000==4000)
		{
			int ballIndex =0;
			std::vector<TexBall*>::iterator ballIterator ;
			for(ballIterator = tball_vector.begin();ballIterator!=tball_vector.end();ballIterator++)
			{
				btTransform btt ;
				btt.setIdentity();
				btt.setOrigin(btVector3(Constant::STARTBALL[ballIndex][0],Constant::STARTBALL[ballIndex][1],Constant::STARTBALL[ballIndex][2]));
				(*ballIterator)->getBody()->setCenterOfMassTransform(btt);
				(*ballIterator)->getBody()->clearForces();
				ballIndex++;
			}

			Constant::shouX=Constant::STARTBALL[array_id][0]/2;
			Constant::shouY=-0.9f;
			isnoFashe=true;
		}
		if(isnoFashe)
		{
			if(array_id==0)
			{
				Constant::shouX+=0.03f;
			}else if(array_id==2)
			{
				Constant::shouX-=0.03f;
			}
			Constant::shouY+=0.1f;
			if(Constant::shouY>1.2f)
			{
				Constant::shouY=5.0f;
			}
		}else{
			Constant::shouY=5.0f;
		}
		if((Constant::shipingJs)%5000==0)
		{
			isnoFashe=false;//发射完毕后
			Constant::shouY=4;

			float vx=Constant::STARTBALL_V[array_id][0];//*10/SCREEN_WIDHT;
           	float vy=Constant::STARTBALL_V[array_id][1];
           	float vz=Constant::STARTBALL_V[array_id][2];

           	tball_vector.at(array_id)->getBody()->activate();
           	tball_vector.at(array_id)->getBody()->setLinearVelocity(btVector3(vx,vy,vz));//设置线速度
           	tball_vector.at(array_id)->getBody()->setAngularVelocity(btVector3(5,0,0));//设置角速度

			array_id ++;
		}
		if(Constant::shipingJs == 50000)
		{
			helpThreadFlag = false;
			Constant::isnoHelpView = false;
			Constant::DIGITAL_SCORE = 0;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
