#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../struct/object/Animation.hpp"
#include "../utility/ObjectMacros.hpp"
#include "../../third_party/olcPixelGameEngine.hpp"

class EngineCore;
struct Animation;

class Object {
public: // Constructor/Destructor Declarations
	Object(float_t _x, float_t _y, uint16_t _index, size_t _id);
	Object(const Object& _other) = delete;
	~Object() = default;

public: // Main Engine Function Declarations
	virtual bool OnUserCreate() = 0;
	virtual bool OnUserDestroy();
	virtual bool OnUserUpdate() = 0;
	virtual bool OnUserRender(EngineCore* _engine);

	virtual bool OnBeforeUserUpdate();
	virtual bool OnAfterUserUpdate();

protected: // Hidden (Accessible to Children Only) Utility Function Declarations
	void AddAnimation(uint8_t _id, float_t _width, float_t _height, float_t _frameLength, const std::initializer_list<olc::vf2d>& _frames, 
			uint8_t _startFrame = 0ui8, uint8_t _loopStart = 0ui8);
	void RemoveAnimation(uint8_t _id);
	Animation* GetAnimation(uint8_t _id);

public: // Publicly Accessible Member Variable Declarations
	float_t					x;
	float_t					y;

	const size_t			instanceID;
	const uint16_t			objectIndex;

	uint8_t					curState;
	uint8_t					nextState;
	uint8_t					lastState;

	uint8_t					nextAnimID;

	int32_t					flags;

protected: // Hidden (Accessible to Children Only) Member Variable Declarations
	olc::Pixel				blendColor;
	float_t					animTimer;
	float_t					animSpeed;
	size_t					animIndex;
	Animation*				prevAnimation;
	Animation*				curAnimation;
	std::vector<AnimData>	animations;
	olc::Decal*				spritesheet;
};

#endif