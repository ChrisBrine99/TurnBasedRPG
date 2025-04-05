#include "SceneManager.hpp"
INIT_SINGLETON_CPP(SceneManager)

#include "../utility/SceneMacros.hpp"
#include "../singleton/EngineCore.hpp"
#include "../scene/BattleScene.hpp"
#include "../scene/LevelScene.hpp"

SceneManager::SceneManager() :
	scenes(),
	flags(INVALID_SCENE_INDEX),
	curSceneIndex(INVALID_SCENE_INDEX),
	nextSceneIndex(INVALID_SCENE_INDEX),
	lastSceneIndex(INVALID_SCENE_INDEX),
	curScene(nullptr)
{}

bool SceneManager::OnUserCreate() {
	LoadScene(new BattleScene(), BATTLE_SCENE_INDEX, true);
	//LoadScene(new LevelScene(), LEVEL_SCENE_INDEX, true);
	return !(curScene == nullptr);
}

bool SceneManager::OnUserDestroy() {
	for (auto& _scene : scenes) {
		_scene.second->OnUserDestroy();
		delete _scene.second, _scene.second = nullptr;
	}
	scenes.clear();

	curScene = nullptr;
	return true;
}

bool SceneManager::OnUserUpdate() {
	return curScene->OnUserUpdate();
}

bool SceneManager::OnUserRender(EngineCore* _engine) {
	return curScene->OnUserRender(_engine);
}

bool SceneManager::OnBeforeUserUpdate() {
	if (SCENE_SHOULD_CHANGE) {
		flags &= ~FLAG_SCENE_CHANGE;

		lastSceneIndex	= curSceneIndex;
		curSceneIndex	= nextSceneIndex;
		if (!curScene->OnUserCreate())
			return false;
	}

	return curScene->OnBeforeUserUpdate();
}

bool SceneManager::OnAfterUserUpdate() {
	curScene->OnAfterUserUpdate();

	if (!SCENE_SHOULD_CHANGE)
		return true;

	if (!curScene->OnUserDestroy()) {
		return false;
	}
	curScene = scenes.at(curSceneIndex);
	return true;
}

void SceneManager::LoadScene(Scene* _scene, uint32_t _index, bool _swapInstantly) {
	auto _iter = scenes.find(_index);
	if (_iter != scenes.end()) {
		delete _scene, _scene = nullptr;
		return;
	}

	scenes.emplace(_index, _scene);
	if (_swapInstantly) {
		ChangeScene(_index);
		curScene = scenes.at(_index);
	}
}

void SceneManager::UnloadScene(uint32_t _index) {
	auto _iter = scenes.find(_index);
	if (_iter == scenes.end())
		return;

	if (_index == curSceneIndex) {
		if (!curScene->OnUserDestroy())
			return;
	}
	scenes.erase(_index);
}

void SceneManager::ChangeScene(uint32_t _index) {
	if (_index == curSceneIndex)
		return;

	auto _iter = scenes.find(_index);
	if (_iter == scenes.end())
		return;

	nextSceneIndex = _index;
	flags |= FLAG_SCENE_CHANGE;
}