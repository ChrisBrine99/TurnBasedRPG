#ifndef UTILITY_FUNCTIONS_HPP
#define UTILITY_FUNCTIONS_HPP

#include <cstdint>
#include <cmath>

// A generic function that assigns the next state to swap over to at the end of the current frame.
inline void SetNextState(uint8_t _state, uint8_t& _nextState) {
	_nextState = _state;
}
// Create a define that will allow this function to be called without having to place "lastState" into the parameter.
#define SET_NEXT_STATE(_state)			SetNextState(_state, nextState)

// Takes in the value for the new state to swap over to, as well as references to the location of the class's current
// state and last state variables so the data held within said variables can be altered through this generic function.
inline void UpdateCurrentState(uint8_t _state, uint8_t& _curState, uint8_t& _lastState) {
	if (_curState == _state)
		return;
	
	_lastState = _curState;
	_curState = _state;
}
// Create a define that will allow this function to be called without having to place "curState" and "lastState" into the parameters.
#define UPDATE_STATE(_state)			UpdateCurrentState(_state, curState, lastState)

// 
template<typename T>
inline T ValueSign(T _value) {
	return (_value >> ((sizeof(T) * 8ui64) - 1i64)) ? T(-1) : T(1);
}

// 
template<typename T>
inline T ValueSignF(T _value) {
	size_t _size = sizeof(T);
	if (_size == sizeof(double_t))
		return (*(int64_t*)&_value >> 63i32) ? -1.0f : 1.0f;
	return (*(int32_t*)&_value >> 31i32) ? -1.0f : 1.0f;
}

// A simple function that will ensure a value will not be allowed to go below the supplied limit. If it does, the value in
// question will be replaced by the limit. Otherwise, the funtion does nothing to the value.
template<typename T>
inline void ValueLowerLimit(T& _value, T _limit) {
	if (_value < _limit)
		_value = _limit;
}

// A simple function for ensuring a value will never go above or below the range determined by the second and third 
// arguments. If the value happens to be outside of the range, the value will be set to the minimum for undershooting the
// range and the maximum for overshooting it.
template<typename T>
inline void ValueClamp(T& _value, T _min, T _max) {
	if (_value < _min)		{ _value = _min; }
	else if (_value > _max) { _value = _max; }
}

// A simple function that allows a value to approach another by a linear amount; the true speed being determined by how
// many times per second the code requires this function to be called.
template<typename T>
inline void ValueSetLinear(T& _value, T _target, T _amount) {
	if (_value == _target)
		return; // The value and target are already equal.

	if (_value < _target) {			// The value is lower than the target.
		_value += _amount;
		if (_value >= _target)		// Prevent the value from overshooting.
			_value = _target;
	} else if (_value > _target) {	// The value is higher than the target.
		_value -= _amount;
		if (_value <= _target)		// Prevent the value from undershooting.
			_value = _target;
	}
}

// 
inline float_t PointDirection(float_t _x1, float_t _y1, float_t _x2, float_t _y2) {
	float_t _radians = std::atan2f(_y2 - _y1, _x2 - _x1);
	float_t _angle = (_radians * 180.0f / 3.1415926535f);
	if (_angle < 0.0f)
		return -_angle;
	_angle = (360.0f - _angle);
	return _angle < 360.0f ? _angle : 0.0f;
}

#endif