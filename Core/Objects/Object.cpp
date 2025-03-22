#include "Object.hpp"

#include "../Singletons/MenuManager.hpp"
#include "../Singletons/ObjectManager.hpp"

Object::Object(int32_t _x, int32_t _y, uint16_t _index, size_t _id) :
	instanceID(_id),
	objectIndex(_index),
	flags(0u),
	x(_x),
	y(_y)
{}

bool Object::OnBeforeUserUpdate(float_t _deltaTime) {
	(void)(_deltaTime);
	if (OBJ_IS_DESTROYED) { DESTROY_OBJECT(instanceID); }
	return true;
}

bool Object::OnAfterUserUpdate(float_t _deltaTime) {
	(void)(_deltaTime);
	return true;
}