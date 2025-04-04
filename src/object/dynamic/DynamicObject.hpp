#ifndef DYNAMIC_OBJECT_HPP
#define DYNAMIC_OBJECT_HPP

#include "../Object.hpp"
#include "../../utility/GeneralMacros.hpp"
#include "../../utility/UtilityFunctions.hpp"

class DynamicObject : public Object {
public: // Constructor/Destructor Declarations
	DynamicObject(float_t _x, float_t _y, uint16_t _index, size_t _id, float_t _accel, float_t _maxSpeed);
	~DynamicObject() = default;

public: // Publicly Accessible Utility Function Declarations
	virtual void OnUserInteract();

protected: // Hidden (Accessible to Children Only) Utility Function Decalarations
	void MoveAndCollide(float_t _xSpeed, float_t _ySpeed, float_t _delta);

protected: // Hidden (Accessible to Children Only) Member Variable Declarations
	const float_t			accel;
	const float_t			maxSpeed;
	float_t					speed;
	float_t					direction;

	float_t					xFraction;
	float_t					yFraction;
};

#endif