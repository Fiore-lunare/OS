// VectorLib.cpp : Defines the exported functions for the DLL.
//

#include "framework.h"
#include "VectorLib.h"
#include "NumberConstants.h"


Vector::Vector() {
	this->x_ = zero;
	this->y_ = zero;
}

Vector::Vector(Number x, Number y) {
	this->x_ = x;
	this->y_ = y;
}

Vector Vector::operator+(const Vector& v) const {
	return Vector(this->x_ + v.x_, this->y_ + v.y_);
}

Vector& Vector::operator+=(const Vector& v) {
	this->x_ += v.x_;
	this->y_ += v.y_;
	return *this;
}

Vector Vector::operator-(const Vector& v) const {
	return Vector(this->x_ - v.x_, this->y_ - v.y_);

}

Vector& Vector::operator-=(const Vector& v) {
	this->x_ -= v.x_;
	this->y_ -= v.y_;
	return *this;
}

Number Vector::r() const {
	Number to_return((this->x_ * this->x_) + (this->y_ * this->y_));
	return to_return.square_root();
}

Number Vector::a() const {
	return this->x_.arcTan(this->y_);
}

extern "C" Vector* CreateVectorWithParameters(Number* n1, Number* n2) {
	return new Vector(*n1, *n2);
}

extern "C" Vector* CreateVector()
{
	return new Vector();
}

extern "C" void DeleteVector(Vector* p)
{
	delete p;
}

extern "C" Number GetR(Vector* v) {
	return v->r();
}
