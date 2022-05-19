/*
 * SoftObj.cpp
 *
 *  Created on: 2016年9月17日
 *      Author: Cy
 */

#include "SoftObj.h"
#include "util/MatrixState.h"
#include "util/ShaderManager.h"

float* SoftObj::vertices_soft = NULL;
int SoftObj::numsVer_soft = 0;
int* SoftObj::indices_soft = NULL;
int SoftObj::numsInd_soft = 0;
float* SoftObj::texs_soft = NULL;
int SoftObj::numsTex_soft = 0;
float* SoftObj::normal_soft = NULL ;
int SoftObj::numNormal_soft = 0 ;
SoftObj::SoftObj(btSoftRigidDynamicsWorld* m_dynamicsWorld,
		btSoftBodyWorldInfo& m_softBodyWorldInfo,
		btVector3 pos,
		float* m_gVertices,		//顶点数据
		int m_nums_ver,			//顶点个数
		int* m_gIndices,		//顶点索引数据
		int m_nums_ind,			//索引个数
		float* m_texs,			//纹理坐标数据
		int m_numtexs,			//纹理坐标个数
		float scale
		)
{
	// TODO Auto-generated constructor stub
	this->m_dynamicsWorld = m_dynamicsWorld;//记录物理世界指针
	this->m_softBodyWorldInfo = m_softBodyWorldInfo;//记录物理世界信息
	this->m_pos = pos;//记录位置坐标
	this->m_gVertices = m_gVertices;//记录顶点数组
	this->m_nums_ind = m_nums_ind;
	this->m_numtexs = m_numtexs;
	for(int i = 0; i<m_nums_ver; i++)//遍历顶点数组
	{
		this->m_gVertices[i] *= scale;//更改顶点数据
	}
	this->m_gIndices = m_gIndices;//记录索引数组

	this->m_numsTriangles = m_nums_ind / 3;//计算三角形个数

	nodeVector = new std::vector<btVector3>();

	initObj();//初始化软体方法
	this->m_numT = psb->m_faces.size();//三角形个数
	this->m_vertices=new float[m_numT*3*3];//创建顶点数组

	//将单个float数据，组成新的btVector3数组
	verticesBt = floatToBtVector3(m_gVertices,m_nums_ver);

	//创建绘制对象
	this->asbv = new AnyShapeByVectices(
			this->m_vertices,//顶点坐标数组    无重复值
			m_nums_ver,//顶点坐标数组的长度
			m_gIndices,
			m_nums_ind,
			m_texs,
			m_numtexs,
			true,
			1.0f,
			ShaderManager::getCommTextureShaderProgram()
			);//三角形问题
	this->initVertex();//初始化顶点数据
}
void SoftObj::initVertex()
{
	int count=0;//顶点计数器
	//nSize = 144
	int nSize = psb->m_nodes.size();
	nodeVector->clear();
	for(int i=0;i<nSize;i++)
	{
		const btSoftBody::Node& n = psb->m_nodes.at(i);
		//储存到本地数组
		nodeVector->push_back((n.m_x));
	}
	for(int j=0;j<m_nums_ind;j++)
	{
		int index = m_gIndices[j];
		m_vertices[count++] = nodeVector->at(index).x();
		m_vertices[count++] = nodeVector->at(index).y();
		m_vertices[count++] = nodeVector->at(index).z();
	}
	//	m_numT = 240
//	for(int i=0;i<m_numT;++i)//遍历所有三角形
//	{
//		const btSoftBody::Face	f=psb->m_faces.at(i);//获取软体上的一个面
//		const btVector3	x[]={f.m_n[0]->m_x,f.m_n[1]->m_x,f.m_n[2]->m_x};//获取组成面的数组
//
//		m_vertices[count++]=x[0].x();//获取第一个点的x坐标
//		m_vertices[count++]=x[0].y();//获取第一个点的y坐标
//		m_vertices[count++]=x[0].z();//获取第一个点的z坐标
//
//		m_vertices[count++]=x[1].x();//获取第二个点的x坐标
//		m_vertices[count++]=x[1].y();//获取第二个点的y坐标
//		m_vertices[count++]=x[1].z();//获取第二个点的z坐标
//
//		m_vertices[count++]=x[2].x();//获取第三个点的x坐标
//		m_vertices[count++]=x[2].y();//获取第三个点的y坐标
//		m_vertices[count++]=x[2].z();//获取第三个点的z坐标
//	}
}

void SoftObj::initObj()
{
	//水袋
	psb=createSoftBody(m_pos,btVector3(0,0,0));//调用创建软体方法
	psb->generateClusters(32);
	psb->m_cfg.kDF=0;
}

btSoftBody*	SoftObj::createSoftBody(const btVector3& x,const btVector3& a)
{
	btSoftBody*	psb=btSoftBodyHelpers::CreateFromTriMesh(m_softBodyWorldInfo,m_gVertices,&m_gIndices[0],m_numsTriangles);//创建三角形组软体方法
	btSoftBody::Material*	pm=psb->appendMaterial();//获取软体材质
	pm->m_kLST				=	1;//设置软体材质信息
	psb->generateBendingConstraints(2,pm);//设置弯曲系数
	psb->m_cfg.piterations	=	2;
	psb->m_cfg.collisions	=	btSoftBody::fCollision::CL_SS+btSoftBody::fCollision::CL_RS;
	psb->setPose(true,true);

	psb->randomizeConstraints();//设置软体随机系数
	psb->rotate(btQuaternion(a[0],a[1],a[2]));//对软体进行旋转变换
	psb->translate(x);//对软体进行平移变换
	psb->setTotalMass(0.1,true);
	psb->generateClusters(32);//设置软体群集数
	m_dynamicsWorld->addSoftBody(psb);//将软体添加到物理世界中
	return(psb);//返回软体指针
}

//SoftObj::~SoftObj() {
//	// TODO Auto-generated destructor stub
//}

void SoftObj::drawSelf(GLuint texIdIn)
{
	MatrixState::pushMatrix();//保护现场

	btTransform trans;
	trans = psb->getWorldTransform();//获取刚体的变换信息对象
	trans.getOpenGLMatrix(MatrixState::currMatrix);//将当前的矩阵设置给变换信息对象

//	画侧边
	MatrixState::pushMatrix();//保护现场
	initVertex();//更新顶点数据
	this->asbv->drawSelf(texIdIn);//绘制软布
	MatrixState::popMatrix();//恢复现场

	MatrixState::popMatrix();//恢复现场
}

btSoftBody* SoftObj::getBody()
{
	return psb;
}
btVector3* SoftObj::floatToBtVector3(float* vertex, int nums)//顶点数组，顶点总数
{
	//返回数组的长度为 nums/3
	btVector3* answer = new btVector3[nums / 3];
	for(int i = 0; i < nums / 3; i++)
	{
		answer[i].setValue(vertex[i*3+0],vertex[i*3+1],vertex[i*3+2]);
	}
	return answer;
}
float* SoftObj::btVector3ToFloat(btVector3* vertex, int nums)//顶点数组，顶点总数
{
	//返回数组的长度为 nums*3
	float* answer = new float[nums * 3];
	for(int i = 0; i < nums; i++)
	{
		answer[i*3+0] = vertex[i].x();
		answer[i*3+1] = vertex[i].y();
		answer[i*3+2] = vertex[i].z();
	}
	return answer;
}
