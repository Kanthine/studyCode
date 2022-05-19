#ifndef TexBall__h
#define TexBall__h

#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/LinearMath/btVector3.h"
#include "Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "util/MatrixState.h"
#include "MyBullet/Ball.h"

class TexBall {
    float r;//表示球的半径
	btRigidBody *body;//表示球刚体指针
	int texId;//球纹理id
	Ball* ball;//绘制球指针
public:

	TexBall(float r,
			int texId,
			btDynamicsWorld *dynamicsWorld,
			btScalar mass,
			btVector3 pos,
			btScalar restitutionIn,
			btScalar frictionIn);
    void drawSelf(int ballTexId,int isShadow,int planeId,int isLanbanYy);//绘制自身刚体的方法
	btRigidBody* getBody();//获取自身刚体指针的方法
	int ballState;//1表示处于篮筐上部且向下运动.其他情况下为0
	//篮球的X速度状态
	int isnoLanBan;//是否和篮板在前一刻碰撞了，0表示没有碰撞
};

#endif
