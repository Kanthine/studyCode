#include "util/Vector.h"

Vector::Vector(float x,float y,float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
Vector::Vector()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}
void Vector::operator=(Vector vector)
{

}
