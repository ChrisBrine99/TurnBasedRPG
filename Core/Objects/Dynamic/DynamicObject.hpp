#ifndef DYNAMIC_OBJECT_HPP
#define DYNAMIC_OBJECT_HPP

#include "../Object.hpp"
#include "../../Utils/GeneralMacros.hpp"
#include "../../Utils/UtilityFunctions.hpp"

class DynamicObject : public Object {
public: // Constructor/Destructor Declarations
	DynamicObject(int32_t _x, int32_t _y, size_t _id);
	~DynamicObject() = default;

public: // Publicly Accessible Member Variable Declarations
	uint8_t curState;
	uint8_t nextState;
	uint8_t lastState;
};

#endif