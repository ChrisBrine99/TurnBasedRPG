#include "LevelScene.hpp"

#include "../object/Object.hpp"
#include "../singleton/ObjectManager.hpp"
#include "../struct/level/Map.hpp"
#include "../utility/Logger.hpp"

LevelScene::LevelScene() : 
	Scene(LEVEL_SCENE_INDEX),
	loadedMaps(),
	curMap(nullptr),
	curMapID(0ui16),
	nextMapID(0ui16),
	lastMapID(0ui16),
	playerID(0ui64),
	testEnemyID(0ui64)
{ // Reserve a small bit of memory within the loaded maps unordered map in case multiple maps are loaded into memory at once.
	loadedMaps.reserve(4ui64);
}

bool LevelScene::OnUserCreate() {
	playerID = CREATE_OBJECT(OBJ_PLAYER, 100.0f, 100.0f);
	testEnemyID = CREATE_OBJECT(OBJ_TEST_ENEMY, 180.0f, 180.0f);
	return true;
}

bool LevelScene::OnUserDestroy() {
	for (auto& _map : loadedMaps)
		delete _map.second, _map.second = nullptr;
	loadedMaps.clear();
	return true;
}

bool LevelScene::OnUserUpdate() {
	Object* _player = GET_SINGLETON(ObjectManager)->GetObjectRef(playerID);
	if (!_player) { return false; }

	Object* _enemy = GET_SINGLETON(ObjectManager)->GetObjectRef(testEnemyID);
	if (!_enemy) { return false; }

	if (_player->boundingBox.BoxCollision(_enemy->boundingBox)) {
		LOG_TRACE("COLLISION");
	}

	return true;
}

bool LevelScene::OnUserRender(EngineCore* _engine) {
	return true;
}

void LevelScene::LoadMap(const std::string& _filepath, uint16_t _id) {
	std::ifstream _file(_filepath, std::ios::in);
	if (!_file.good()) { // The filestream flipped an error bit; don't try to load the map and log the error.
		LOG_ERROR("Map with filepath " + _filepath + " failed to load!!!");
		return;
	}
}

void LevelScene::UnloadMap(uint16_t _id) {

}

inline uint32_t LevelScene::GetTileIndex(uint32_t _x, uint32_t _y) const {
	return curMap->GetTileIndex(_x, _y);
}

inline uint32_t LevelScene::GetTileFlags(uint32_t _x, uint32_t _y) const {
	return curMap->GetTileFlags(_x, _y);
}