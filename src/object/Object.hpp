#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../struct/object/Animation.hpp"
#include "../utility/ObjectMacros.hpp"
#include "../../third_party/olcPixelGameEngine.hpp"

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
	int32_t					x;
	int32_t					y;
	float_t					xFraction;
	float_t					yFraction;

	const size_t			instanceID;
	const uint16_t			objectIndex;

	uint8_t					curState;
	uint8_t					nextState;
	uint8_t					lastState;

	int32_t					flags;

protected: // Hidden (Accessible to Children Only) Member Variable Declarations
	float_t					drawX;
	float_t					drawY;

	olc::Pixel				blendColor;
	float_t					animTimer;
	float_t					animSpeed;
	size_t					animIndex;
	Animation*				curAnimation;
	std::vector<Animation*>	animations;
	olc::Decal*				spritesheet;
};

#endif