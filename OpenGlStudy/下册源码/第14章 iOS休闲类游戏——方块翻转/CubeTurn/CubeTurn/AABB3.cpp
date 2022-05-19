#include "AABB3.h"
#include "Vector3f.h"
#include "Matrix.hpp"
#include <vector>
#include <limits> 
using namespace std;

AABB3::AABB3()
{
	min = new Vector3f();
	max = new Vector3f();
	empty();
}

AABB3::AABB3(float *vertices,int count)
{
    max = new Vector3f();
	min = new Vector3f();
    
    empty();
	
	for(int i=0; i<count; i+=3){
		add(vertices[i], vertices[i+1], vertices[i+2]);
	}

}

void AABB3::add(Vector3f *p)
{
	if (p->getX() < min->getX()) { min->setX(p->getX()); }
	if (p->getX() > max->getX()) { max->setX(p->getX()); }
	if (p->getY() < min->getY()) { max->setY(p->getY()); }
	if (p->getY() > max->getY()) { max->setY(p->getY()); }
	if (p->getZ() < min->getZ()) { min->setZ(p->getZ()); }
	if (p->getZ() > max->getZ()) { max->setZ(p->getZ()); }
}

void AABB3::add(float nx, float ny, float nz){
    
    if (nx < min->getX()) { min->setX(nx); }
    if (nx > max->getX()) { max->setX(nx); }
    
    if (ny < min->getY()) { min->setY(ny); }
    if (ny > max->getY()) { max->setY(ny); }
    
    if (nz < min->getZ()) { min->setZ(nz); }
    if (nz > max->getZ()) { max->setZ(nz); }
    
}

vector< Vector3f* > AABB3::getAllCorners()
{
	vector< Vector3f*> result;
	for(int i=0; i<8; i++){
		result.push_back(getCorner(i));
	}
	return result;
}


Vector3f* AABB3::getCorner(int i)
{
	return new Vector3f(
		((i & 1) == 0) ? max->getX() : min->getX(),
		((i & 2) == 0) ? max->getY() : min->getY(),
		((i & 4) == 0) ? max->getZ() : min->getZ());
}

AABB3* AABB3::setToTransformedBox(float *m)
{
	vector< Vector3f*> result = getAllCorners();
    float* transformedCorners=new float[24];
	float* tmpResult=new float[4];
    int countTemp=0;
	for(int i=0;i<result.size();i++){
		float* point=new float[4];
		point[0]=result[i]->getX();
		point[1]=result[i]->getY();
		point[2]=result[i]->getZ();
        point[3]=1.0f;
		
		Matrix::multiplyMV(tmpResult,0,m,0,point,0);
		transformedCorners[countTemp++]=tmpResult[0];
		transformedCorners[countTemp++]=tmpResult[1];
		transformedCorners[countTemp++]=tmpResult[2];
	}
    delete tmpResult;
//    delete result;
	return new AABB3(transformedCorners,countTemp);
}

float AABB3::getXSize()
{
	return max->getX() - min->getX();
}

float AABB3::getYSize()
{
	return max->getY() - min->getY();
}

float AABB3::getZSize()
{
	return max->getZ() - min->getZ();
}
Vector3f* AABB3::getSize()
{
	return max->vMinus(min);
}

Vector3f* AABB3::getCenter()
{
	Vector3f* temp=min->add(max);
	return temp->multiK(0.5f);
}

float AABB3::rayIntersect(
	Vector3f* rayStart,
	Vector3f* rayDir
)
{
	const float kNoIntersection = numeric_limits<float>::max();
	bool inside = true;
    float xt=0.0f;
	if(rayStart->getX()	<min->getX()){
		xt = min->getX() - rayStart->getX();
		if(xt>rayDir->getX()){ return kNoIntersection; }
		xt /= rayDir->getX();
		inside = false;
	}
	else if(rayStart->getX()>max->getX()){
		xt = max->getX() - rayStart->getX();
		if(xt<rayDir->getX()){ return kNoIntersection; }
		xt /= rayDir->getX();
		inside = false;
	}
	else{
		xt = -1.0f;
	}
    float yt=0.0f;
	if(rayStart->getY()	<min->getY()){
		yt = min->getY() - rayStart->getY();
		if(yt>rayDir->getY()){ return kNoIntersection; }
		yt /= rayDir->getY();
		inside = false;
	}
	else if(rayStart->getY()>max->getY()){
		yt = max->getY() - rayStart->getY();
		if(yt<rayDir->getY()){ return kNoIntersection; }
		yt /= rayDir->getY();
		inside = false;
	}
	else{
		yt = -1.0f;
	}
	
    float zt=0.0f;
	if(rayStart->getZ()	<min->getZ()){
		zt = min->getZ() - rayStart->getZ();
		if(zt>rayDir->getZ()){ return kNoIntersection; }
		zt /= rayDir->getZ();
		inside = false;
	}
	else if(rayStart->getZ()>max->getZ()){
		zt = max->getZ()- rayStart->getZ();
		if(zt<rayDir->getZ()){ return kNoIntersection; }
		zt /= rayDir->getZ();
		inside = false;
	}
	else{
		zt = -1.0f;
	}
	
	if(inside){
		return 0.0f;
	}
	
	int which = 0;
	float t = xt;
	if(yt>t){
		which = 1;
		t=yt;
	}
	if(zt>t){
		which = 2;
		t=zt;
	}
	switch(which){
		case 0:
		{
			float y=rayStart->getY()+rayDir->getY()*t;
			if(y<min->getY()||y>max->getY()){return kNoIntersection;}
			float z=rayStart->getZ()+rayDir->getZ()*t;
			if(z<min->getZ()||z>max->getZ()){return kNoIntersection;}
		}
		break;
		case 1:
		{
			float x=rayStart->getX()+rayDir->getX()*t;
			if(x<min->getX()||x>max->getX()){return kNoIntersection;}
			float z=rayStart->getZ()+rayDir->getZ()*t;
			if(z<min->getZ()||z>max->getZ()){return kNoIntersection;}
		}
		break;
		case 2:
		{
			float x=rayStart->getX()+rayDir->getX()*t;
			if(x<min->getX()||x>max->getX()){return kNoIntersection;}
			float y=rayStart->getY()+rayDir->getY()*t;
			if(y<min->getY()||y>max->getY()){return kNoIntersection;}
		}
		break;
	}
	return t;
}

void AABB3::empty()
{
    //设置为最大值
	min->setX(numeric_limits<float>::max());
 	min->setY(numeric_limits<float>::max());
 	min->setZ(numeric_limits<float>::max());
 	
 	//设置为最小值
    float inf=-100000;
	max->setX(inf);
	max->setY(inf);
	max->setZ(inf);
}
AABB3::~AABB3()
{
	
}
