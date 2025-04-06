#include "AnimationInstance.hpp"

#include "../../singleton/DataManager.hpp"

AnimationInstance::AnimationInstance(Animation* _animation, float_t _speed, uint16_t _loopStart) :
	animationRef(_animation),
	speed(_speed),
	loopStart(_loopStart),
	curFrame(0ui16)
{}

bool AnimationInstance::UpdateAnimation(float_t& _animTimer) {
	bool _animEnded = false;

	// Processing forward animations by incrementing the frame and looping it until the value for "_animTimer" is 
	// below the threshold required for the animation's speed.
	while (_animTimer >= speed) {
		_animTimer -= speed;
		curFrame++;
		if (curFrame == animationRef->numFrames) { // Animation has looped; true will return if loop is valid.
			curFrame = 0ui16;
			_animEnded = true;
		}
	}

	// Doing the same thing as above, but for the animation playing in reverse if required.
	while (_animTimer <= -speed) {
		_animTimer += speed;
		curFrame--;
		if (curFrame == 0xFFFFui16) { // Animation has looped; true will return.
			curFrame = animationRef->numFrames - 1ui16;
			_animEnded = true;
		}
	}

	return _animEnded;
}