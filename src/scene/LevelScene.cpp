#include "LevelScene.hpp"

LevelScene::LevelScene() : 
	Scene(LEVEL_SCENE_INDEX)
{}

bool LevelScene::OnUserCreate() {
	return true;
}

bool LevelScene::OnUserDestroy() {
	return true;
}

bool LevelScene::OnUserUpdate(float_t _deltaTime) {
	return true;
}

bool LevelScene::OnUserRender(EngineCore* _engine, float_t _deltaTime) {
	return true;
}