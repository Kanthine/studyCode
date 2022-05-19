#ifndef __Vector3f_H__
#define __Vector3f_H__

class Vector3f{
private:
	float x;
	float y;
	float z;
public: 	
	Vector3f();
	Vector3f(float x,float y,float z);
	Vector3f(float* v);
	
	Vector3f* add(Vector3f* v);
 	Vector3f* vMinus(Vector3f* v);
  	Vector3f* multiK(float k);
  	void normalize();
	float module();
	
	float getX();
	float getY();
	float getZ();
	
	void setX(float tempx);
	void setY(float tempY);
	void setZ(float tempZ);
  	
	~Vector3f();
};

#endif
