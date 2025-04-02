#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "../../../third_party/olcPixelGameEngine.hpp"

#include <initializer_list>
#include <vector>

struct Animation {
	std::vector<olc::vf2d>	frameData;
	olc::vf2d				size;
	uint8_t					id;
	uint8_t					curFrame;
	uint8_t					numFrames;
	uint8_t					loopStart;
	float_t					frameLength;

private: // Constructor Declarations
	Animation() = delete;
	Animation(uint8_t _id, float_t _width, float_t _height, float_t _frameLength, uint8_t _numFrames, uint8_t _startFrame = 0ui8, uint8_t _loopStart = 0ui8);
	Animation(const Animation& _other) = delete;
public: // Destructor Declaration
	~Animation() = default;

public: // Publicly Accessible Utility Function Declarations/Definitions
	static Animation* CreateAnimation(uint8_t _id, float_t _width, float_t _height, float_t _frameLength, const std::initializer_list<olc::vf2d>& _frames,
				uint8_t _startFrame = 0ui8, uint8_t _loopStart = 0ui8);
	bool UpdateAnimation(float_t& _animTimer);

	// Simply fetches the frame the sprite's animation is at currently.
	inline olc::vf2d GetCurrentFrame() const {
		return frameData[curFrame];
	}
};

typedef std::pair<uint8_t, Animation*> AnimData;

#endif