#include "ObjPlayer.hpp"

#include "../../singleton/DataManager.hpp"

ObjPlayer::ObjPlayer(int32_t _x, int32_t _y, uint16_t _index, size_t _id) : 
	DynamicObject(_x, _y, _index, _id)
{}

bool ObjPlayer::OnUserCreate() {
	spritesheet = GET_SINGLETON(DataManager)->LoadSprite(0x0000ui16, "res/graphics/test_spritesheet.png");
	if (!spritesheet) { return false; }

	Animation* _testAnimation = new Animation(16.0f, 16.0f, 0.25f, 8ui8, 0ui8, 0ui8);
	_testAnimation->frameData.push_back({   0.0f, 0.0f });
	_testAnimation->frameData.push_back({  16.0f, 0.0f });
	_testAnimation->frameData.push_back({  32.0f, 0.0f });
	_testAnimation->frameData.push_back({  48.0f, 0.0f });
	_testAnimation->frameData.push_back({  64.0f, 0.0f });
	_testAnimation->frameData.push_back({  80.0f, 0.0f });
	_testAnimation->frameData.push_back({  96.0f, 0.0f });
	_testAnimation->frameData.push_back({ 112.0f, 0.0f });
	animations.push_back(_testAnimation);
	curAnimation = _testAnimation;

	flags |= FLAG_OBJ_ACTIVE | FLAG_OBJ_VISIBLE;
	return true;
}

bool ObjPlayer::OnUserUpdate(float_t _deltaTime) {
	return true;
}