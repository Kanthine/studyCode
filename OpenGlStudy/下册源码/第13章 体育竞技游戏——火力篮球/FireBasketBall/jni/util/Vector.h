#ifndef _Vector_H_
#define _Vector_H_

class Vector
{
public:
	Vector();
	Vector(float x,float y,float z);
	~Vector(){}
	void operator=(Vector vector);
public:
	float x;
	float y;
	float z;
};

#endif
