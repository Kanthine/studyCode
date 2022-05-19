#ifndef _TexBody_H_
#define _TexBody_H_

#include "Bullet/BulletDynamics/Dynamics/btRigidBody.h"
//纯虚类
class TexBody
{
public:
	//虚方法，继承此类必须实现---类似于Java的抽象方法
	virtual void drawSelf() = 0;
	virtual btRigidBody* getBody() = 0;
};

#endif
