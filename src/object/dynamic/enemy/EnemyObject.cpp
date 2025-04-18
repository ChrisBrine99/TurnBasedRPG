#include "EnemyObject.hpp"

#include "../../../singleton/DataManager.hpp"

EnemyObject::EnemyObject(float_t _x, float_t _y, uint16_t _index, size_t _id, float_t _accel, float_t _maxSpeed) :
	DynamicObject(_x, _y, _index, _id, _accel, _maxSpeed),
	encounterID(ENC_INVALID)
{}