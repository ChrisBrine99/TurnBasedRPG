#include "DynamicObject.hpp"

#include "../../utility/Logger.hpp"

DynamicObject::DynamicObject(float_t _x, float_t _y, uint16_t _index, size_t _id, float_t _xAccel, float_t _yAccel, float_t _xMaxSpeed, float_t _yMaxSpeed) :
	Object(_x, _y, _index, _id),
	xAccel(_xAccel),
	yAccel(_yAccel),
	xMaxSpeed(_xMaxSpeed),
	yMaxSpeed(_yMaxSpeed),
	xSpeed(0.0f),
	ySpeed(0.0f),
	xFraction(0.0f),
	yFraction(0.0f)
{}

void DynamicObject::UpdateMovementValues(float_t _deltaTime) {
	float_t _xDelta = 0.0f;
	if (xSpeed != 0.0f) {
		_xDelta		= xSpeed * _deltaTime;
		_xDelta	   += xFraction;
		xFraction	= _xDelta - (std::floor(std::abs(_xDelta)) * ValueSignF(_xDelta));
		_xDelta	   -= xFraction;
		x		   += _xDelta;
	}
	
	float_t _yDelta = 0.0f;
	if (ySpeed != 0.0f) {
		_yDelta		= ySpeed * _deltaTime;
		_yDelta	   += yFraction;
		yFraction	= _yDelta - (std::floor(std::abs(_yDelta)) * ValueSignF(_yDelta));
		_yDelta	   -= yFraction;
		y		   += _yDelta;
	}
}