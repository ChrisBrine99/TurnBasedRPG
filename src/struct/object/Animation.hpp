#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "../../utility/DataMacros.hpp"

#include <vector>

struct Animation {
	std::vector<int32_t>	frameData;
	int32_t					width;
	int32_t					height;
	uint16_t				id;
	uint16_t				spriteID;
	uint16_t				numFrames;

public: // Constructor/Destructor Declarations
	Animation() : 
		frameData(),
		width(0i32),
		height(0i32),
		id(ANM_INVALID),
		spriteID(SPR_INVALID),
		numFrames(0ui16)
	{};
	Animation(const Animation& _other) = delete;
	~Animation() = default;
};

#endif