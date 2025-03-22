#include "DynamicObject.hpp"

DynamicObject::DynamicObject(int32_t _x, int32_t _y, uint16_t _index, size_t _id) :
	Object(_x, _y, _index, _id),
	curState(STATE_INVALID),
	nextState(STATE_INVALID),
	lastState(STATE_INVALID)
{}

bool DynamicObject::OnAfterUserUpdate(float_t _deltaTime) {
	(void)(_deltaTime);
	SET_NEXT_STATE(nextState);
	return true;
}