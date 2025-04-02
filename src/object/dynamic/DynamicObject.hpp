#ifndef DYNAMIC_OBJECT_HPP
#define DYNAMIC_OBJECT_HPP

#include "../Object.hpp"
#include "../../utility/GeneralMacros.hpp"
#include "../../utility/UtilityFunctions.hpp"

class DynamicObject : public Object {
public: // Constructor/Destructor Declarations
	DynamicObject(float_t _x, float_t _y, uint16_t _index, size_t _id, float_t _xAccel, float_t _yAccel, float_t _xMaxSpeed, float_t _yMaxSpeed);
	~DynamicObject() = default;

protected: // Hidden (Accessible to Children Only) Utility Function Decalarations
	void UpdateMovementValues(float_t _deltaTime);

protected: // Hidden (Accessible to Children Only) Member Variable Declarations
	float_t					xAccel;
	float_t					yAccel;

	float_t					xMaxSpeed;
	float_t					yMaxSpeed;
	float_t					xSpeed;
	float_t					ySpeed;

	float_t					xFraction;
	float_t					yFraction;
};

#endif