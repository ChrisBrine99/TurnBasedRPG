#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../Utils/ObjectMacros.hpp"
#include "../../ThirdParty/olcPixelGameEngine.hpp"

#include <vector>
#include <cstdint>
#include <math.h>

class EngineCore;
struct Animation;

class Object {
public: // Constructor/Destructor Declarations
	Object(int32_t _x, int32_t _y, uint16_t _index, size_t _id);
	Object(const Object& _other) = delete;
	~Object() = default;

public: // Main Engine Function Declarations
	virtual bool OnUserCreate() = 0;
	virtual bool OnUserDestroy();
	virtual bool OnUserUpdate(float_t _deltaTime) = 0;
	virtual bool OnUserRender(EngineCore* _engine, float_t _deltaTime);

	virtual bool OnBeforeUserUpdate(float_t _deltaTime);
	virtual bool OnAfterUserUpdate(float_t _deltaTime);

public: // Publicly Accessible Member Variable Declarations
	olc::Decal*				spritesheet;
	std::vector<Animation*>	animations;
	Animation*				curAnimation;
	size_t					animIndex;
	float_t					animTimer;
	float_t					animSpeed;

	const size_t	instanceID;
	const uint16_t	objectIndex;
	int32_t			flags;

	int32_t x;
	int32_t y;
	float_t xFraction;
	float_t yFraction;
};

struct Animation {
	std::vector<olc::vf2d>	frameData;
	olc::vf2d				size;
	uint8_t					curFrame;
	uint8_t					numFrames;
	uint8_t					loopStart;
	float_t					frameLength;

public: // Constructor/Destructor Definitions
	Animation(float_t _width, float_t _height, float_t _frameLength, uint8_t _numFrames, uint8_t _startFrame = 0ui8, uint8_t _loopStart = 0ui8) :
		frameData(),
		size({ _width, _height }),
		curFrame(_startFrame),
		numFrames(_numFrames),
		loopStart(_loopStart),
		frameLength(_frameLength)
	{ // Reserve a small bit of space to make room for potential frames within the animation.
		frameData.reserve(3ui64);
	};
	Animation(const Animation& _other) = delete;
	~Animation() = default;

public: // Publicly Accessible Utility Function Definitions
	inline bool UpdateAnimation(float_t& _animTimer) {
		bool _animEnded = false;

		// Processing forward animations by incrementing the frame and looping it until the value for "_animTimer" is 
		// below the threshold required for the animation's speed.
		while (_animTimer >= frameLength) {
			_animTimer -= frameLength;
			curFrame++;
			if (curFrame == numFrames) { // Animation has looped; true will return if loop is valid.
				curFrame	= loopStart;
				_animEnded	= true;
			}
		}

		// Doing the same thing as above, but for the animation playing in reverse if required.
		while (_animTimer <= -frameLength) {
			_animTimer += frameLength;
			curFrame--;
			if (curFrame == 0xFFui8) { // Animation has looped; true will return.
				curFrame	= loopStart;
				_animEnded	= true;
			}
		}

		return _animEnded;
	}

	inline olc::vf2d GetCurrentFrame() const {
		return frameData[curFrame];
	}
};

#endif