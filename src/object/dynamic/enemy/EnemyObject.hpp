#ifndef ENEMY_OBJECT_HPP
#define ENEMY_OBJECT_HPP

#include "../DynamicObject.hpp"

class EnemyObject : public DynamicObject {
public: // Constructor/Destructor Declarations
	EnemyObject(float_t _x, float_t _y, uint16_t _index, size_t _id, float_t _accel, float_t _maxSpeed);
	~EnemyObject() = default;

protected: // Publicly Accessible Member Variable Declarations
	uint16_t encounterID;
};

#endif