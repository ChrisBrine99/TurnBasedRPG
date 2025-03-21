#include "ObjPlayer.hpp"

ObjPlayer::ObjPlayer(int32_t _x, int32_t _y, size_t _id) : 
	DynamicObject(_x, _y, _id)
{}

bool ObjPlayer::OnUserCreate() {
	return true;
}

bool ObjPlayer::OnUserDestroy() {
	return true;
}

bool ObjPlayer::OnUserUpdate(float_t _deltaTime) {
	return true;
}

bool ObjPlayer::OnUserRender(float_t _deltaTime) {
	return true;
}