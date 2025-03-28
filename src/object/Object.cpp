#include "Object.hpp"

#include "../singleton/EngineCore.hpp"
#include "../singleton/MenuManager.hpp"
#include "../singleton/ObjectManager.hpp"
#include "../utility/UtilityFunctions.hpp"

Object::Object(int32_t _x, int32_t _y, uint16_t _index, size_t _id) :
	x(_x),
	y(_y),
	xFraction(0.0f),
	yFraction(0.0f),
	instanceID(_id),
	objectIndex(_index),
	curState(STATE_INVALID),
	nextState(STATE_INVALID),
	lastState(STATE_INVALID),
	flags(0u),
	drawX(float_t(_x)),
	drawY(float_t(_y)),
	blendColor(olc::Pixel(0xFFFFFFFFui32)),
	animTimer(0.0f),
	animSpeed(1.0f),
	curAnimation(nullptr),
	animIndex(OBJ_INVALID_ANIMATION_INDEX),
	animations(),
	spritesheet(nullptr)
{ // Reserve a small bit of memory to make room for potential animations the object may contain.
	animations.reserve(3ui64);
}

bool Object::OnUserDestroy() {
	for (Animation* _anim : animations)
		delete _anim, _anim = nullptr;
	animations.clear();
	animations.shrink_to_fit();
	return true;
}

bool Object::OnUserRender(EngineCore* _engine, float_t _deltaTime) {
	if (!spritesheet || !OBJ_IS_VISIBLE)
		return true;

	animTimer  += _deltaTime * animSpeed;
	flags	   &= ~FLAG_OBJ_ANIMATION_END; // Always clear this flag on the frame right after it is set by the above funtion returning true.
	if (curAnimation->UpdateAnimation(animTimer))
		flags  |= FLAG_OBJ_ANIMATION_END;

	_engine->DrawPartialDecal(
		{ drawX, drawY },					// Utilize the float versions of the object's x and y position
		spritesheet,						// The image resource to reference
		curAnimation->GetCurrentFrame(),	// Offset into the image resources to sample
		curAnimation->size,					// Size of the sampled region
		{ 1.0f, 1.0f },						// Scale of the sprite (Unused currently)
		blendColor							// Optional blending color for the sprite
	);

	_engine->DrawStringDecal({ 5.0f, 5.0f }, std::to_string(curAnimation->curFrame) + '\n' + std::to_string(animTimer));
	return true;
}

bool Object::OnBeforeUserUpdate(float_t _deltaTime) {
	(void)(_deltaTime);
	if (OBJ_IS_DESTROYED) 
		DESTROY_OBJECT(instanceID);
	return true;
}

bool Object::OnAfterUserUpdate(float_t _deltaTime) {
	(void)(_deltaTime);
	SET_NEXT_STATE(nextState);
	return true;
}