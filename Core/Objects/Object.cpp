#include "Object.hpp"

#include "../Singletons/EngineCore.hpp"
#include "../Singletons/MenuManager.hpp"
#include "../Singletons/ObjectManager.hpp"
#include "../Utils/UtilityFunctions.hpp"

Object::Object(int32_t _x, int32_t _y, uint16_t _index, size_t _id) :
	spritesheet(nullptr),
	animations(),
	curAnimation(nullptr),
	animIndex(OBJ_INVALID_ANIMATION_INDEX),
	animTimer(0.0f),
	animSpeed(1.0f),
	objectIndex(_index),
	instanceID(_id),
	flags(0u),
	x(_x),
	y(_y),
	xFraction(0.0f),
	yFraction(0.0f)
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
		{ float_t(x), float_t(y) },			// Position converted form int32s to floats
		spritesheet,						// The image resource to reference
		curAnimation->GetCurrentFrame(),	// Offset into the image resources to sample
		curAnimation->size					// Size of the sampled region
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
	return true;
}