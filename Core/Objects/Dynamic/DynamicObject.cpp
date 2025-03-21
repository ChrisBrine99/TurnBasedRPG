#include "DynamicObject.hpp"

DynamicObject::DynamicObject(int32_t _x, int32_t _y, size_t _id) :
	Object(_x, _y, _id),
	curState(STATE_INVALID),
	nextState(STATE_INVALID),
	lastState(STATE_INVALID)
{}