#include <stdio.h>
#include <math.h>

#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <vector>
#include "Bullet/LinearMath/btAlignedObjectArray.h"
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/BulletSoftBody/btSoftBody.h"
#include "Bullet/BulletSoftBody/btSoftBodyHelpers.h"
#include "Bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "Bullet/BulletSoftBody/btSoftBodySolvers.h"
#include "Bullet/BulletSoftBody/btDefaultSoftBodySolver.h"
#include "Bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "myEncapsulation/MatrixState.h"
#include "../myEncapsulation/TexCuboid.h"
#include "../myEncapsulation/TexBody.h"
#include "../myEncapsulation/TexPlane.h"
#include "../myEncapsulation/TexBall.h"
#include "../myEncapsulation/TexCapsule.h"
#include "../myEncapsulation/TexCone.h"
#include "../myEncapsulation/TexCylinderShape.h"
#include "../myEncapsulation/FileUtil.h"

#include "SoftObj.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

using namespace std;

//地形  光线投射
extern "C" {

	int boxTexId;	//立方体纹理id
	int ballTexId;	//球体纹理id
	int planeTexId;	//地面纹理id
	int wood_binId;//门id
	int floorId;//室内地面纹理id


	float UNIT_SIZE = 1.0;	//单位长度

	vector<TexBody*> tca;	//存储刚体封装类对象的vector（包括 立方体，球体，地面）
	TexBody* tbTemp;		//临时刚体封装类对象

	int addBodyId = 0;		//添加刚体的id

	//摄像机的位置坐标
	float cx = 0;
	float cy = 20;
	float cz = 20;

	//up向量
	float upx = 0;
	float upy = 1;
	float upz = 0;

	btDefaultCollisionConfiguration* m_collisionConfiguration;//系统默认碰撞检测配置信息
	btCollisionDispatcher*	m_dispatcher;	//碰撞检测算法分配器
	btBroadphaseInterface*	m_broadphase;	//宽度阶段碰撞检测
	btConstraintSolver*		m_solver;		//碰撞约束解决器
	btSoftRigidDynamicsWorld*		m_dynamicsWorld;//物理世界
	btSoftBodyWorldInfo	m_softBodyWorldInfo;

	//设置摄像机位置
	void setCamera(float x,float y,float z,float upxt,float upyt,float upzt)
	{
		cx = x;
		cy = y;
		cz = z;
		upx = upxt;
		upy = upyt;
		upz = upzt;
	}

	//初始化物理世界的方法
	void initWorld()
	{
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
		m_softBodyWorldInfo.air_density		=	(btScalar)1.2;
//		m_softBodyWorldInfo.water_density	=	0;
//		m_softBodyWorldInfo.water_offset	=	0;
//		m_softBodyWorldInfo.water_normal	=	btVector3(0,0,0);

		//创建物理世界对象
		m_dynamicsWorld = new btSoftRigidDynamicsWorld(dispatcher, overlappingPairCache, solver,collisionConfiguration,softBodySolver);
		//设置重力加速度
		btVector3 gvec = btVector3(0, -10, 0);
		m_dynamicsWorld->setGravity(gvec);

	}

	#define RUANGUAN_OBJ 1
	#define YUANHUAN_OBJ 2

	float* vertices_yuanhuan;
	int numsVer_yuanhuan;
	int* indices_yuanhuan;
	int numsInd_yuanhuan;

	float* vertices_ruanguan;
	int numsVer_ruanguan;
	int* indices_ruanguan;
	int numsInd_ruanguan;

	void initCreateBodys()
	{
		{
			tbTemp = new TexPlane(
					m_dynamicsWorld,//物理世界对象
					btVector3(100, 1, 100),//长方体的半区域
					0.0f,//长方体的质量
					btVector3(0,-20,0),//刚体的位置
					0.8f,//恢复系数
					0.8f,//摩擦系数
					UNIT_SIZE,//单位长度
					planeTexId,planeTexId,planeTexId,
					planeTexId,planeTexId,planeTexId
			);
			//将新立方体加入到列表中
			tca.push_back(tbTemp);
		}


		{
			tbTemp = new SoftObj(m_dynamicsWorld,
					m_softBodyWorldInfo,
					btVector3(0,0,0),
					vertices_yuanhuan,
					numsVer_yuanhuan,
					indices_yuanhuan,
					numsInd_yuanhuan,
					3.0f
					);
			//将新立方体加入到列表中
			tca.push_back(tbTemp);
		}
		{
			tbTemp = new SoftObj(m_dynamicsWorld,
					m_softBodyWorldInfo,
					btVector3(0,20,0),
					vertices_ruanguan ,//顶点坐标数组    无重复值
					numsVer_ruanguan ,//顶点坐标数组的长度
					indices_ruanguan ,
					numsInd_ruanguan ,
					3.0f
					);
			//将新立方体加入到列表中
			tca.push_back(tbTemp);
		}
	}

	void loadObjData(int objId, float* vertices, int numsVer, float* normals, int numsNor)
	{
	}

	void loadObjDataWd(int objId, float* vertices, int numsVer, int* indices, int numsInd,
					float* texs, int numsTex)
	{
		if(objId == YUANHUAN_OBJ)
		{
			vertices_yuanhuan = vertices;
			numsVer_yuanhuan = numsVer;
			indices_yuanhuan = indices;
			numsInd_yuanhuan = numsInd;
		}
		else if(objId == RUANGUAN_OBJ)
		{
			vertices_ruanguan = vertices;
			numsVer_ruanguan = numsVer;
			indices_ruanguan = indices;
			numsInd_ruanguan = numsInd;
		}

	}

	void setAddBodyId(int id)
	{
		addBodyId = id;

	}
	void addBody(int id)
	{
		tbTemp = new TexCuboid(
				m_dynamicsWorld,//物理世界对象
				btVector3(UNIT_SIZE*2, UNIT_SIZE*2, UNIT_SIZE*2),//长方体的半区域
				10.0f,//长方体的质量
				btVector3(cx,cy-10,cz),//刚体的位置
				0.2f,//恢复系数
				0.8f,//摩擦系数
				UNIT_SIZE,//单位长度
				boxTexId,boxTexId,boxTexId,
				boxTexId,boxTexId,boxTexId
		);
    	//设置箱子的初始速度
		//btVector3 vvec = btVector3(0,2,-10);
		btVector3 vvec = btVector3(-cx,-cy+10,-cz);
		btVector3 avec = btVector3(0,0,0);
		tbTemp->getBody()->setLinearVelocity(vvec);//箱子直线运动的速度--Vx,Vy,Vz三个分量
		tbTemp->getBody()->setAngularVelocity(avec); //箱子自身旋转的速度--绕箱子自身的x,y,x三轴旋转的速度
    	//将新立方体加入到列表中
    	tca.push_back(tbTemp);
	}
	void cleanVector()
	{
		tca.clear();
	}

	bool onSurfaceChanged(int w, int h) {
	    glViewport(0, 0, w, h);
	    float ratio = (float) w/h;
	    MatrixState::setProjectFrustum(-ratio, ratio, -1, 1, 2, 100);

		initWorld();
		cleanVector();
		initCreateBodys();


		return true;
	}


	bool onSurfaceCreated(JNIEnv * env,jobject obj) {

	    MatrixState::setCamera(0, 20, 20, 0, 0, -5, 0, 1, 0);
	    MatrixState::setLightLocation(0, 20, 20);//设置光源位置
	    MatrixState::setInitStack();
	    glClearColor(0, 0, 0, 0);
	    glEnable(GL_DEPTH_TEST);


		jclass cl = env->FindClass("com/bn/bullet/GL2JNIView");
		jmethodID id = env->GetStaticMethodID(cl,"initTextureRepeat","(Landroid/opengl/GLSurfaceView;Ljava/lang/String;)I");
		jstring name = env->NewStringUTF("box.jpg");
		boxTexId = env->CallStaticIntMethod(cl,id,obj,name);
		name = env->NewStringUTF("basketball.jpg");
		ballTexId = env->CallStaticIntMethod(cl,id,obj,name);
		name = env->NewStringUTF("grass.png");
		planeTexId = env->CallStaticIntMethod(cl,id,obj,name);
		name = env->NewStringUTF("wood_bin.jpg");//注意，文件名中不能出现数字！！！wood_bin1.jpg是错误的
		wood_binId = env->CallStaticIntMethod(cl,id,obj,name);
		name = env->NewStringUTF("floor.jpg");
		floorId = env->CallStaticIntMethod(cl,id,obj,name);

//		initWorld();
//		cleanVector();
//		initCreateBodys();

		return true;
	}

	void renderFrame() {
	    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);//清除缓冲
        //设置camera位置
        MatrixState::setCamera
        (
        		cx,	//人眼位置的X
        		cy, //人眼位置的Y
        		cz, //人眼位置的Z
        		0, 	//人眼球看的点X
        		0,  //人眼球看的点Y
        		0,  //人眼球看的点Z
        		upx, 	//up向量
        		upy,
        		upz
        );
        MatrixState::setLightLocation(cx, cy, cz);//设置光源位置


	    for ( int i = 0; i < tca.size(); ++i )//遍历物体列表
	    {
	    	 tca[i]->drawSelf();//回调刚体的绘制方法
	    }

	    if(addBodyId!=0)
	    {
	    	addBody(addBodyId);//调用添加刚体的方法
	    	addBodyId = 0;//设置刚体id为0
	    }

	    m_dynamicsWorld->stepSimulation(1.0/60,5);//进行物理模拟计算
	}

	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_setCamera
	  (JNIEnv *env, jclass jc,jfloat cx,jfloat cy,jfloat cz,jfloat upx,jfloat upy,jfloat upz)
	{
		setCamera(cx,cy,cz,upx,upy,upz);
	}

	float* copyFloats(float* src,int count)
	{
		float* dst=new float[count];
		for(int i=0;i<count;i++)
		{
			dst[i]=src[i];
		}
		return dst;
	}
	int* copyInts(int* src,int count)
	{
		int* dst=new int[count];
		for(int i=0;i<count;i++)
		{
			dst[i]=src[i];
		}
		return dst;
	}

	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_loadObjData
	  (JNIEnv *env, jclass jc, jint objId, jfloatArray vertices, jint numsVer, jfloatArray normals, jint numsNor)
	{
		jfloat*  jfVertexData= (jfloat*)(env->GetFloatArrayElements(vertices,0));
		jfloat*  jfNormalData= (jfloat*)(env->GetFloatArrayElements(normals,0));

		jsize vlen = env->GetArrayLength(vertices);
		jsize nlen = env->GetArrayLength(normals);


		loadObjData(
				(int)objId,
				copyFloats((float*)jfVertexData,(int)vlen),
				(int)numsVer,
				copyFloats((float*)jfNormalData,(int)nlen),
				(int)numsNor
				);


		env->ReleaseFloatArrayElements(vertices,jfVertexData,0);
		env->ReleaseFloatArrayElements(normals,jfNormalData,0);
	}

	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_loadObjDataWd
	  (JNIEnv *env, jclass jc, jint objId, jfloatArray vertices, jint numsVer, jintArray indices, jint numsInd,
			  jfloatArray tex, jint numsTex)
	{
		jfloat*  jfVertexData= (jfloat*)(env->GetFloatArrayElements(vertices,0));
		jint* jiIndexData = (jint*)(env->GetIntArrayElements(indices,0));
		jfloat*  jfTexData= (jfloat*)(env->GetFloatArrayElements(tex,0));

		loadObjDataWd(
				(int)objId,
				copyFloats((float*)jfVertexData,(int)numsVer),
				(int)numsVer,
				copyInts((int*)jiIndexData,(int)numsInd),
				(int)numsInd,
				copyFloats((float*)jfTexData,(int)numsTex),
				(int)numsTex
				);


		env->ReleaseFloatArrayElements(vertices,jfVertexData,0);
		env->ReleaseIntArrayElements(indices,jiIndexData,0);
		env->ReleaseFloatArrayElements(tex,jfTexData,0);
	}

	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_nativeSetAssetManager
	  (JNIEnv *env, jclass cls, jobject assetManager)
	{
		AAssetManager* aamIn = AAssetManager_fromJava( env, assetManager );
	    FileUtil::setAAssetManager(aamIn);
	}


}

