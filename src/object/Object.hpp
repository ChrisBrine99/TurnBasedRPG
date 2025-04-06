#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../struct/object/AnimationInstance.hpp"
#include "../utility/ObjectMacros.hpp"
#include "../../third_party/olcPixelGameEngine.hpp"

class EngineCore;

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

public: // Publicly Accessible Utility Function DEclarations
	inline bool CanUpdateObject()	const { return OBJ_IS_ACTIVE && !OBJ_IS_DESTROYED; }
	inline bool CanRenderObject()	const { return OBJ_IS_ACTIVE && OBJ_IS_VISIBLE; }
	inline bool CanDestroyObject()	const { return OBJ_IS_DESTROYED; }

protected: // Hidden (Accessible to Children Only) Utility Function Declarations
	void AddAnimation(uint16_t _animInstID, uint16_t _id, float_t _animSpeed, uint16_t _loopStart = 0ui16);
	void RemoveAnimation(uint16_t _id);

public: // Publicly Accessible Member Variable Declarations
	float_t						x;
	float_t						y;

	const size_t				instanceID;
	const uint16_t				objectIndex;

	uint8_t						curState;
	uint8_t						nextState;
	uint8_t						lastState;

protected: // Hidden (Accessible to Children Only) Member Variable Declarations
	uint16_t					curAnimation;
	uint16_t					nextAnimation;
	uint16_t					lastAnimation;

	int32_t						flags;

	olc::Sprite*				spritesheet;
	AnimationInstance*			animationRef;
	float_t						animTimer;
	float_t						animSpeed;
	std::unordered_map<uint16_t, AnimationInstance*>		
								animations;
};

#endif