#ifndef TEST_ENEMY_HPP
#define TEST_ENEMY_HPP

#include "EnemyObject.hpp"

class ObjTestEnemy : public EnemyObject {
public: // Constructor/Destructor Declarations
	ObjTestEnemy(float_t _x, float_t _y, uint16_t _index, size_t _id);
	~ObjTestEnemy() = default;

public: // Main Engine Function Overrides
	bool OnUserCreate() override;
	bool OnUserUpdate() override;
};

#endif