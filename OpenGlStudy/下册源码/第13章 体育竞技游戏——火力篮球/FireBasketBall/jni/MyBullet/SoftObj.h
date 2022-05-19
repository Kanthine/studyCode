/*
 * SoftObj.h
 *
 *  Created on: 2015年1月31日
 *      Author: Nr
 */

#ifndef _SoftObj_H_
#define _SoftObj_H_

#include "Bullet/LinearMath/btAlignedObjectArray.h"
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/LinearMath/btVector3.h"
#include "Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "Bullet/BulletSoftBody/btSoftBody.h"
#include "Bullet/BulletSoftBody/btSoftBodyHelpers.h"
#include "Bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "Bullet/BulletSoftBody/btSoftBodySolvers.h"
#include "Bullet/BulletSoftBody/btDefaultSoftBodySolver.h"
#include "Bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"

#include "MyBullet/AnyShapeByVectices.h"
#include "MyBullet/TexBody.h"
#include "MyBullet/TexCuboid.h"


using namespace std;
#include <vector>

class SoftObj {

	btSoftBody *psb;//软体指针
	btSoftRigidDynamicsWorld *m_dynamicsWorld;//软体物理世界指针
	btSoftBodyWorldInfo m_softBodyWorldInfo;//软体物理世界信息
	AnyShapeByVectices* asbv;
	float* m_vertices;//顶点数组

	int m_texId;//纹理id

	int m_numT;//三角形个数

	float* m_gVertices;//顶点数组
	int m_nums_ver;//顶点数组长度
	int* m_gIndices;//顶点索引数组
	int m_nums_ind;//顶点索引数组长度

	int m_numsTriangles;//三角形个数

	float* m_texs;
	int m_numtexs;

	btVector3 m_pos;//位置坐标

	std::vector<btVector3 >* nodeVector;

public:
	SoftObj(
			btSoftRigidDynamicsWorld* m_dynamicsWorld,
			btSoftBodyWorldInfo& m_softBodyWorldInfo,
			btVector3 pos,
			float* m_gVertices,		//顶点数据
			int m_nums_ver,			//顶点个数
			int* m_gIndices,		//索引数据
			int m_nums_ind,			//索引个数
			float* m_texs,			//纹理坐标数据
			int m_numtexs,			//纹理坐标个数
			float scale
			);//构造函数
//	virtual ~SoftObj();

	void drawSelf(GLuint texIdIn);//绘制自身方法
	btSoftBody* getBody();//获取刚体指针方法
	void initVertexTemp();
	void initVertex();//初始化顶点数据方法

	void initObj();//初始化软体方法
	btSoftBody*	createSoftBody(const btVector3& x,const btVector3& a);//创建软体方法

	btVector3* floatToBtVector3(float* vertex, int nums);//顶点数组，顶点总数
	float* btVector3ToFloat(btVector3* vertex, int nums);//顶点数组，顶点总数
public:
	btVector3* verticesBt;					//根据传入的单个float数组组成新的btVector3顶点数组

	static float* vertices_soft;
	static int numsVer_soft;
	static int* indices_soft;
	static int numsInd_soft;
	static float* texs_soft;
	static int numsTex_soft;
	static float* normal_soft ;
	static int numNormal_soft ;

	bool templin = true;
};

#endif /* JNI_HELLOCPP_SoftObj_H_ */

