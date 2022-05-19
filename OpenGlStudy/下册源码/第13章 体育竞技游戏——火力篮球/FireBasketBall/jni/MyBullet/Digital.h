#ifndef _Digital_H_
#define _Digital_H_

#include "MyBullet/TextureRect.h"

class Digital{
public:
	Digital(GLuint mprogram);
	~Digital(){}
	void drawSelf(int score,GLuint texId);
public:
	TextureRect* textureRectArray[10];
};

#endif
