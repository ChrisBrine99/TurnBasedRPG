#ifndef UTILITY_FUNCTIONS_HPP
#define UTILITY_FUNCTIONS_HPP

#include <inttypes.h>

// A generic function that assigns the next state to swap over to at the end of the current frame.
inline void SetNextState(uint8_t _state, uint8_t& _nextState) {
	_nextState = _state;
}

// Takes in the value for the new state to swap over to, as well as references to the location of the class's current
// state and last state variables so the data held within said variables can be altered through this generic function.
inline void UpdateCurrentState(uint8_t _state, uint8_t& _curState, uint8_t& _lastState) {
	if (_curState == _state)
		return;
	
	_lastState = _curState;
	_curState = _state;
}

// A simple function that checks a value to see if has gone below the second supplied value. If so, the function will 
// return the second value. Otherwise, the value passed into the first argument parameter is returned.
template<typename T>
inline T ValueLowerLimit(T _value, T _limit) {
	if (_value < _limit)
		return _limit;
	return _value;
}

// A simple function that will limit the given value to be between the supplied minimum and maximum values. If the given
// value has to be outside of that range, the minimum or maximum will be returned depending on if the value is lower than
// the minimum or higher than that maximum, respectively.
template<typename T>
inline T ValueClamp(T _value, T _min, T _max) {
	if (_value < _min)
		return _min;

	if (_value > _max)
		return _max;

	return _value;
}

// 
template<typename T>
inline void ValueSetLinear(T& _curValue, T _targetValue, T _amount) {
	if (_curValue == _targetValue)
		return;

	if (_curValue < _targetValue) {
		_curValue += _amount;
		if (_curValue >= _targetValue)
			_curValue = _targetValue;
		return;
	}
	
	if (_curValue > _targetValue) {
		_curValue -= _amount;
		if (_curValue <= _targetValue)
			_curValue = _targetValue;
		return;
	}
}

#endif