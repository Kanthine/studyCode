#ifndef __AABB_H__
#define __AABB_H__
#include <vector>
#include "Vector3f.h"
using namespace std;
class AABB3{
private:
	
public: 
	Vector3f* min;
	Vector3f* max;
	
	AABB3();
	AABB3(float* vertices,int count);
	
	void empty();
	void add(Vector3f* p);
	void add(float x, float y, float z);
	vector< Vector3f*> getAllCorners();
	Vector3f* getCorner(int i);
	AABB3* setToTransformedBox(float* m);
	float getXSize();
	float getYSize();
	float getZSize();
	Vector3f* getSize();
	Vector3f* getCenter();
	float rayIntersect(
		Vector3f* rayStart,
		Vector3f* rayDir
	);
	
	~AABB3();
};
#endif
