#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "../../../third_party/olcPixelGameEngine.hpp"

#include <vector>

struct Animation {
	std::vector<olc::vf2d>	frameData;
	olc::vf2d				size;
	uint8_t					curFrame;
	uint8_t					numFrames;
	uint8_t					loopStart;
	float_t					frameLength;

public: // Constructor/Destructor Declarations
	Animation(float_t _width, float_t _height, float_t _frameLength, uint8_t _numFrames, uint8_t _startFrame = 0ui8, uint8_t _loopStart = 0ui8);
	Animation(const Animation& _other) = delete;
	~Animation() = default;

public: // Publicly Accessible Utility Function Declarations/Definitions
	bool UpdateAnimation(float_t& _animTimer);

	// Simply fetches the frame the sprite's animation is at currently.
	inline olc::vf2d GetCurrentFrame() const {
		return frameData[curFrame];
	}
};

#endif