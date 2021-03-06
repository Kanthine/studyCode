#include <math.h>
#include "TexBall.h"
#include "util/ShaderUtil.h"
#include "util/MatrixState.h"
#include "util/Shadermanager.h"

TexBall::TexBall(float r,int texId,
		btDynamicsWorld *dynamicsWorld,
		btScalar mass,
		btVector3 pos,
		btScalar restitutionIn,
		btScalar frictionIn)
{
	bool isDynamic = (mass != 0);//物体是否可以运动
	btVector3 localInertia = btVector3(0, 0, 0);//表示惯性向量
	//创建球形形状
	btCollisionShape* colShape = new btSphereShape(r);
	if(isDynamic)
	{
		colShape->calculateLocalInertia(mass, localInertia);//计算惯性
	}
	btTransform startTransform = btTransform();//创建刚体的初始变换对象
	startTransform.setIdentity();//变换对象初始化
	startTransform.getOrigin().setValue(pos.x(), pos.y(), pos.z());//设置变换对象的位置
	btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);//创建刚体运动状态对象

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);//创建刚体信息对象
	rbInfo.m_restitution = restitutionIn;//设置反弹系数
	rbInfo.m_friction = frictionIn;//设置摩擦系数
	body = new btRigidBody(rbInfo);//创建刚体

	dynamicsWorld->addRigidBody(body);//将刚体添加进物理世界

	this->r = r;//记录球半径
	this->texId = texId;//记录纹理id

	ball = new Ball(
			texId,
			r,
			ShaderManager::getShadowshaderProgram()
			);//创建绘制球对象

	ballState = 0;

	isnoLanBan = 0;
}
void TexBall::drawSelf(int ballTexId,int isShadow,int planeId,int isLanbanYy)
{
	MatrixState::pushMatrix();//保护现场

	btTransform trans;									//获取这个箱子的变换信息对象
	trans = body->getWorldTransform();				//获取刚体的变换信息对象
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//将当前的矩阵设置给变换信息对象

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	MatrixState::pushMatrix();	//保护现场
	ball->drawSelf(ballTexId,isShadow,planeId,isLanbanYy);	//绘制球
	MatrixState::popMatrix();	//恢复现场

	glDisable(GL_BLEND);

	MatrixState::popMatrix();	//恢复现场
}
btRigidBody* TexBall::getBody()
{
	return body;//返回刚体指针
}
