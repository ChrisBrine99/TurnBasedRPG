#ifndef DYNAMIC_OBJECT_HPP
#define DYNAMIC_OBJECT_HPP

#include "../Object.hpp"
#include "../../utility/GeneralMacros.hpp"
#include "../../utility/UtilityFunctions.hpp"

class DynamicObject : public Object {
public: // Constructor/Destructor Declarations
	DynamicObject(int32_t _x, int32_t _y, uint16_t _index, size_t _id);
	~DynamicObject() = default;
};

#endif