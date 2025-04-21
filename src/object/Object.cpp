#include "Object.hpp"

#include "../singleton/DataManager.hpp"
#include "../singleton/EngineCore.hpp"
#include "../singleton/MenuManager.hpp"
#include "../singleton/ObjectManager.hpp"
#include "../struct/object/AnimationInstance.hpp"
#include "../utility/Logger.hpp"
#include "../utility/UtilityFunctions.hpp"

Object::Object(float_t _x, float_t _y, uint16_t _index, size_t _id) :
	boundingBox(0i32, 0i32, 0i32, 0i32),
	bBoxX(0i32),
	bBoxY(0i32),
	x(_x),
	y(_y),
	instanceID(_id),
	objectIndex(_index),
	curState(STATE_INVALID),
	nextState(STATE_INVALID),
	lastState(STATE_INVALID),
	curAnimation(ANM_INVALID),
	nextAnimation(ANM_INVALID),
	lastAnimation(ANM_INVALID),
	flags(0u),
	spritesheet(nullptr),
	animationRef(nullptr),
	animTimer(0.0f),
	animSpeed(1.0f),
	animations()
{ // Reserve a small bit of memory to make room for potential animations the object may contain.
	animations.reserve(3ui64);
}

bool Object::OnUserDestroy() {
	for (auto& _anim : animations)
		delete _anim.second, _anim.second = nullptr;
	animations.clear();
	return true;
}

bool Object::OnUserRender(EngineCore* _engine) {
	if (!spritesheet || !OBJ_IS_VISIBLE || !animationRef)
		return true;

	animTimer  += EngineCore::deltaTime * animSpeed;
	flags	   &= ~FLAG_OBJ_ANIMATION_END; // Always clear this flag on the frame right after it is set by the above funtion returning true.
	if (animationRef->UpdateAnimation(animTimer))
		flags  |= FLAG_OBJ_ANIMATION_END;

	std::pair<int32_t, int32_t> _frameOffset = animationRef->GetCurFramePosition();
	_engine->DrawPartialSprite(
		int32_t(x),
		int32_t(y),
		spritesheet,
		_frameOffset.first,
		_frameOffset.second,
		animationRef->animationRef->width,
		animationRef->animationRef->height
	);

	//_engine->DrawRect(int32_t(x), int32_t(y), boundingBox.width, boundingBox.height);

	return true;
}

bool Object::OnBeforeUserUpdate() {
	if (OBJ_IS_DESTROYED) 
		DESTROY_OBJECT(instanceID);
	return true;
}

bool Object::OnAfterUserUpdate() {
	UPDATE_STATE(nextState);

	if (OBJ_HAS_BOUNDING_BOX) {
		boundingBox.x = int32_t(x) + bBoxX;
		boundingBox.y = int32_t(y) + bBoxY;
	}

	if (curAnimation == nextAnimation)
		return true;

	lastAnimation	= curAnimation;
	curAnimation	= nextAnimation;
	animationRef	= animations[curAnimation];
	return true;
}

void Object::InitBoundingBox(int32_t _xOffset, int32_t _yOffset, int32_t _width, int32_t _height) {
	flags			   |= FLAG_OBJ_BOUNDING_BOX;
	bBoxX				= _xOffset;
	bBoxY				= _yOffset;
	boundingBox.x		= int32_t(x) + _xOffset;
	boundingBox.y		= int32_t(y) + _yOffset;
	boundingBox.width	= _width;
	boundingBox.height	= _height;
}

void Object::AddAnimation(uint16_t _animInstID, uint16_t _id, float_t _animSpeed, uint16_t _loopStart) {
	Animation* _animation = GET_SINGLETON(DataManager)->LoadAnimation(_id);
	if (!_animation) {
		LOG_ERROR("Animation with provided ID couldn't be loaded!!!");
		return;
	}
	animations[_animInstID] = new AnimationInstance(_animation, _animSpeed, _loopStart);
}

void Object::RemoveAnimation(uint16_t _id) {
	for (auto& _anim : animations) {
		if (_anim.second->animationRef->id == _id) {
			animations.erase(_id);
			return;
		}
	}
	LOG_WARN("Animation with provided ID (" + std::to_string(_id) + ") does not exist for this object!");
}