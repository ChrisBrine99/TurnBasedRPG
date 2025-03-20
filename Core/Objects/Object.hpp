#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../Singletons/ObjectManager.hpp"

#include <cstdint>
#include <math.h>

class Object {
public: // Constructor/Destructor Declarations
	Object() = delete;
	Object(int32_t _x, int32_t _y, size_t _id);
	~Object() = default;

public: // Main Engine Function Declarations
	virtual bool OnUserCreate() = 0;
	virtual bool OnUserDestroy() = 0;
	virtual bool OnUserCreate(float_t _deltaTime) = 0;
	virtual bool OnUserRender(float_t _deltaTime) = 0;

	bool OnBeforeUserUpdate(float_t _deltaTime);
	void OnAfterUserUpdate(float_t _deltaTime);

public: // Publicly Accessible Member Variable Declarations
	const size_t instanceID;

	int32_t flags;
	int32_t x;
	int32_t y;
};

#endif