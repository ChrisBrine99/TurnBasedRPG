#include "LevelScene.hpp"

#include "../singleton/ObjectManager.hpp"

LevelScene::LevelScene() : 
	Scene(LEVEL_SCENE_INDEX)
{}

bool LevelScene::OnUserCreate() {
	CREATE_OBJECT(OBJ_PLAYER, 100.0f, 100.0f);
	return true;
}

bool LevelScene::OnUserDestroy() {
	return true;
}

bool LevelScene::OnUserUpdate() {
	return true;
}

bool LevelScene::OnUserRender(EngineCore* _engine) {
	return true;
}