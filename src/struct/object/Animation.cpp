#include "Animation.hpp"

Animation::Animation(uint8_t _id, float_t _width, float_t _height, float_t _frameLength, uint8_t _numFrames, uint8_t _startFrame, uint8_t _loopStart) :
	frameData(),
	size({ _width, _height }),
	id(_id),
	curFrame(_startFrame),
	numFrames(_numFrames),
	loopStart(_loopStart),
	frameLength(_frameLength) 
{ // Reserve enough frames for the supplied length of the animation.
	frameData.reserve(_numFrames);
};

Animation* Animation::CreateAnimation(uint8_t _id, float_t _width, float_t _height, float_t _frameLength, const std::initializer_list<olc::vf2d>& _frames, 
		uint8_t _startFrame, uint8_t _loopStart) 
{
	Animation* _newAnimation = new Animation(_id, _width, _height, _frameLength, uint8_t(_frames.size()), _startFrame, _loopStart);
	for (auto& _frame : _frames)
		_newAnimation->frameData.push_back(_frame);
	return _newAnimation;
}

bool Animation::UpdateAnimation(float_t& _animTimer) {
	bool _animEnded = false;

	// Processing forward animations by incrementing the frame and looping it until the value for "_animTimer" is 
	// below the threshold required for the animation's speed.
	while (_animTimer >= frameLength) {
		_animTimer -= frameLength;
		curFrame++;
		if (curFrame == numFrames) { // Animation has looped; true will return if loop is valid.
			curFrame = loopStart;
			_animEnded = true;
		}
	}

	// Doing the same thing as above, but for the animation playing in reverse if required.
	while (_animTimer <= -frameLength) {
		_animTimer += frameLength;
		curFrame--;
		if (curFrame == 0xFFui8) { // Animation has looped; true will return.
			curFrame = loopStart;
			_animEnded = true;
		}
	}

	return _animEnded;
}