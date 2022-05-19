#include "Vector3f.h"
#include <math.h>
#include <iostream>
#include <string>

using namespace std;
Vector3f::Vector3f()
{
	setX(0);
	setY(0);
	setZ(0);
}
Vector3f::Vector3f(float tempx,float tempy,float tempz)
{
	setX(tempx);
	setY(tempy);
	setZ(tempz);
}

Vector3f::Vector3f(float* v)
{
	setX(v[0]);
	setY(v[1]);
	setZ(v[2]);
}
Vector3f*  Vector3f::add(Vector3f* v)
{
	return new Vector3f(x+v->getX(),y+v->getY(),z+v->getZ());
}

Vector3f*  Vector3f::vMinus(Vector3f* v)
{
	return new Vector3f(x-v->getX(),y-v->getY(),z-v->getZ());
}

Vector3f*  Vector3f::multiK(float k)
{
	return new Vector3f(x*k,y*k,z*k);
}

void Vector3f::normalize()
{
	float mod=module();
	x /= mod;
	y /= mod;
	z /= mod;
}

void Vector3f::setX(float tempX)
{
	x=tempX;
}

void Vector3f::setY(float tempY)
{
	y=tempY;
}

void Vector3f::setZ(float tempZ)
{
	z=tempZ;
}

float Vector3f::getX()
{
	return x;
}

float Vector3f::getY()
{
	return y;
}

float Vector3f::getZ()
{
	return z;
}

float Vector3f::module()
{
	return (float) sqrt(x*x+y*y+z*z);
}

Vector3f::~Vector3f()
{
	
}
