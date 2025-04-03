#include "Object.hpp"

#include "../singleton/EngineCore.hpp"
#include "../singleton/MenuManager.hpp"
#include "../singleton/ObjectManager.hpp"
#include "../utility/Logger.hpp"
#include "../utility/UtilityFunctions.hpp"

Object::Object(float_t _x, float_t _y, uint16_t _index, size_t _id) :
	x(_x),
	y(_y),
	instanceID(_id),
	objectIndex(_index),
	curState(STATE_INVALID),
	nextState(STATE_INVALID),
	lastState(STATE_INVALID),
	nextAnimID(ANIM_INVALID),
	flags(0u),
	blendColor(COLOR_TRUE_WHITE),
	animTimer(0.0f),
	animSpeed(1.0f),
	curAnimation(nullptr),
	prevAnimation(nullptr),
	animIndex(OBJ_INVALID_ANIMATION_INDEX),
	animations(),
	spritesheet(nullptr)
{ // Reserve a small bit of memory to make room for potential animations the object may contain.
	animations.reserve(3ui64);
}

bool Object::OnUserDestroy() {
	delete spritesheet, spritesheet = nullptr;

	prevAnimation = nullptr;
	curAnimation = nullptr;
	for (auto& _anim : animations)
		delete _anim.second, _anim.second = nullptr;
	animations.clear();
	animations.shrink_to_fit();
	return true;
}

bool Object::OnUserRender(EngineCore* _engine) {
	if (!spritesheet || !OBJ_IS_VISIBLE || !curAnimation)
		return true;

	animTimer  += EngineCore::deltaTime * animSpeed;
	flags	   &= ~FLAG_OBJ_ANIMATION_END; // Always clear this flag on the frame right after it is set by the above funtion returning true.
	if (curAnimation->UpdateAnimation(animTimer))
		flags  |= FLAG_OBJ_ANIMATION_END;

	_engine->DrawPartialDecal(
		{ std::floor(x), std::floor(y) },	// Utilize the floored versions of the object's x and y position
		spritesheet,						// The image resource to reference
		curAnimation->GetCurrentFrame(),	// Offset into the image resources to sample
		curAnimation->size,					// Size of the sampled region
		{ 1.0f, 1.0f },						// Scale of the sprite (Unused currently)
		blendColor							// Optional blending color for the sprite
	);

	return true;
}

bool Object::OnBeforeUserUpdate() {
	if (OBJ_IS_DESTROYED) 
		DESTROY_OBJECT(instanceID);
	return true;
}

bool Object::OnAfterUserUpdate() {
	UPDATE_STATE(nextState);

	if (nextAnimID == ANIM_INVALID)
		return true;

	Animation* _nextAnimation = GetAnimation(nextAnimID);
	nextAnimID = ANIM_INVALID;

	if (!_nextAnimation)
		return true;

	prevAnimation = curAnimation;
	curAnimation = _nextAnimation;
	return true;
}

void Object::AddAnimation(uint8_t _id, float_t _width, float_t _height, float_t _frameLength, const std::initializer_list<olc::vf2d>& _frames,
		uint8_t _startFrame, uint8_t _loopStart) 
{
	if (_id == ANIM_INVALID) {
		LOG_ERROR("Animation id cannot be set to 0xFFui8 (255 in decimal) !!!");
		return;
	}

	for (auto& _anim : animations) {
		if (_anim.first == _id) {
			LOG_WARN("Attempted to load in an animation with an id (" + std::to_string(_id) + ") that already exists!");
			return;
		}
	}
	animations.push_back(std::make_pair(_id, Animation::CreateAnimation(_id, _width, _height, _frameLength, _frames, _startFrame, _loopStart)));
}

void Object::RemoveAnimation(uint8_t _id) {
	auto _index = animations.begin();
	for (auto& _anim : animations) {
		if (_anim.first == _id) {
			if (curAnimation == _anim.second)
				curAnimation = prevAnimation;
			delete _anim.second, _anim.second = nullptr;
			animations.erase(_index);
			return;
		}
		_index++;
	}
	LOG_WARN("Animation with provided ID (" + std::to_string(_id) + ") does not exist for this object!");
}

Animation* Object::GetAnimation(uint8_t _id) {
	for (auto& _anim : animations) {
		if (_anim.first == _id)
			return _anim.second;
	}
	LOG_ERROR("Attempted to set animation that doesn't exist on the current object!!!");
	return nullptr;
}