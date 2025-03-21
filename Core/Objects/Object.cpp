#include "Object.hpp"

Object::Object(int32_t _x, int32_t _y, size_t _id) :
	instanceID(_id),
	flags(0u),
	x(_x),
	y(_y)
{}

bool Object::OnBeforeUserUpdate(float_t _deltaTime) {
	(void)(_deltaTime);
	return true;
}

bool Object::OnAfterUserUpdate(float_t _deltaTime) {
	(void)(_deltaTime);
	return true;
}