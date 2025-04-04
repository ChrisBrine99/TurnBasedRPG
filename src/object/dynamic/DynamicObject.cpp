#include "DynamicObject.hpp"

DynamicObject::DynamicObject(float_t _x, float_t _y, uint16_t _index, size_t _id, float_t _accel, float_t _maxSpeed) :
	Object(_x, _y, _index, _id),
	accel(_accel),
	maxSpeed(_maxSpeed),
	speed(0.0f),
	direction(0.0f),
	xFraction(0.0f),
	yFraction(0.0f)
{}

void DynamicObject::OnUserInteract() 
{}

void DynamicObject::MoveAndCollide(float_t _xSpeed, float_t _ySpeed, float_t _delta) {
	// Determine whole pixel movement along the x axis for the frame.
	if (_xSpeed != 0.0f) {
		_xSpeed		= _xSpeed * _delta;
		_xSpeed    += xFraction;
		xFraction	= _xSpeed - (std::floor(std::abs(_xSpeed)) * ValueSignF(_xSpeed));
		_xSpeed	   -= xFraction;
		
		// Skip collision check if the x speed isn't 1.0 or greater (After the code above it will only ever 
		// been a whole floating-point value).
		if (_xSpeed == 0.0f)
			goto process_vertical_movement;

		// If the object isn't set to collide with the environment, their horizontal position is simply updated 
		// and the vertical movement is processed. Collision is completely skipped.
		if (!OBJ_CAN_COLLIDE_WITH_WORLD) {
			x += _xSpeed;
			goto process_vertical_movement;
		}


	}
	
	// Determine whole pixel movement along the y axis for the frame.
process_vertical_movement:
	if (_ySpeed != 0.0f) {
		_ySpeed		= _ySpeed * _delta;
		_ySpeed    += yFraction;
		yFraction	= _ySpeed - (std::floor(std::abs(_ySpeed)) * ValueSignF(_ySpeed));
		_ySpeed    -= yFraction;

		// Skip vertical collision for the same reason that would cause the horizontal collision to skip but
		// with the _ySpeed value being the determining value.
		if (_ySpeed == 0.0f)
			return;

		// If the object isn't set to collide with the environment, their vertical position is simply updated 
		// and the function is exited from early.
		if (!OBJ_CAN_COLLIDE_WITH_WORLD) {
			y += _ySpeed;
			return;
		}


	}
}