#include "ObjTestEnemy.hpp"

#include "../../../singleton/DataManager.hpp"

ObjTestEnemy::ObjTestEnemy(float_t _x, float_t _y, uint16_t _index, size_t _id) :
	EnemyObject(_x, _y, _index, _id, 0.3f, 1.5f)
{}

bool ObjTestEnemy::OnUserCreate() {
	encounterID = ENC_TEST;

	spritesheet = ADD_SPRITE(SPR_PLAYER, "res/graphics/test_spritesheet.png");
	if (!spritesheet) { return false; }

	AddAnimation(0x0000ui16, ANM_TEST, 8.0f);
	nextAnimation = 0x0000ui16;

	InitBoundingBox(0i32, 0i32, 16i32, 16i32);
	flags |= FLAG_OBJ_ACTIVE | FLAG_OBJ_VISIBLE;
	return true;
}

bool ObjTestEnemy::OnUserUpdate() {
	return true;
}