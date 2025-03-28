#include "SceneManager.hpp"
INIT_SINGLETON_CPP(SceneManager)

#include "../Utils/SceneMacros.hpp"
#include "../Singletons/EngineCore.hpp"
#include "../Scenes/BattleScene.hpp"

#include <iostream>

SceneManager::SceneManager() :
	scenes(),
	flags(INVALID_SCENE_INDEX),
	currentSceneIndex(INVALID_SCENE_INDEX),
	currentScene(nullptr)
{}

bool SceneManager::OnUserCreate() {
	LoadScene(new BattleScene(), BATTLE_SCENE_INDEX, true);
	return !(currentScene == nullptr);
}

bool SceneManager::OnUserDestroy() {
	for (auto& _scene : scenes) {
		_scene.second->OnUserDestroy();
		delete _scene.second, _scene.second = nullptr;
	}
	scenes.clear();

	currentScene = nullptr;
	return true;
}

bool SceneManager::OnUserUpdate(float_t _deltaTime) {
	return currentScene->OnUserUpdate(_deltaTime);
}

bool SceneManager::OnUserRender(EngineCore* _engine, float_t _deltaTime) {
	return currentScene->OnUserRender(_engine, _deltaTime);
}

bool SceneManager::OnBeforeUserUpdate(float_t _deltaTime) {
	if (SCENE_SHOULD_CHANGE) {
		flags &= ~FLAG_SCENE_CHANGE;

		if (!currentScene->OnUserCreate())
			return false;
	}

	return currentScene->OnBeforeUserUpdate(_deltaTime);
}

bool SceneManager::OnAfterUserUpdate(float_t _deltaTime) {
	currentScene->OnAfterUserUpdate(_deltaTime);

	if (!SCENE_SHOULD_CHANGE)
		return true;

	if (!currentScene->OnUserDestroy()) {
		std::cout << "Previous scene failed to properly destroy itself!" << std::endl;
		return false;
	}

	currentScene = scenes.at(currentSceneIndex);
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
		currentScene = scenes.at(_index);
	}
}

void SceneManager::UnloadScene(uint32_t _index) {
	auto _iter = scenes.find(_index);
	if (_iter == scenes.end())
		return;

	if (_index == currentSceneIndex) {
		if (!currentScene->OnUserDestroy())
			return;
	}
	scenes.erase(_index);
}

void SceneManager::ChangeScene(uint32_t _index) {
	if (_index == currentSceneIndex)
		return;

	auto _iter = scenes.find(_index);
	if (_iter == scenes.end())
		return;

	currentSceneIndex = _index;
	flags |= FLAG_SCENE_CHANGE;
}