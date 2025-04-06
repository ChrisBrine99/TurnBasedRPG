#ifndef ANIMATION_INSTANCE_HPP
#define ANIMATION_INSTANCE_HPP

#include "Animation.hpp"

struct AnimationInstance {
	Animation*		animationRef;
	float_t			speed;
	uint16_t		loopStart;
	uint16_t		curFrame;

public: // Constructor/Destructor Declarations
	AnimationInstance(Animation* _animation, float_t _speed, uint16_t _loopStart);
	AnimationInstance(const AnimationInstance& _other) = delete;
	~AnimationInstance() = default;

public: // Publicly Accessible Utility Function Declarations
	bool UpdateAnimation(float_t& _animTimer);

	// Simply fetches the frame the sprite's animation is at currently.
	inline std::pair<int32_t, int32_t> GetCurFramePosition() const {
		std::vector<int32_t>&	_frameData	= animationRef->frameData;
		size_t					_index		= size_t(curFrame) * 2ui64;
		return std::make_pair(_frameData[_index], _frameData[_index + 1ui64]);
	}
};

#endif