#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include "../utility/GeneralMacros.hpp"
#include "../scene/Scene.hpp"

#include <unordered_map>

class EngineCore;

class SceneManager {
	// Singleton Initialization (Creates Constructor/Destrcutor Declarations)
	INIT_SINGLETON_HPP(SceneManager)
public: // Main Engine/Engine Extension Function Declarations
	bool OnUserCreate();
	bool OnUserDestroy();
	bool OnUserUpdate();
	bool OnUserRender(EngineCore* _engine);

	bool OnBeforeUserUpdate();
	bool OnAfterUserUpdate();

public: //Scene Management Function Declarations
	void LoadScene(Scene* _scene, uint32_t _index, bool _swapInstantly = false);
	void UnloadScene(uint32_t _index);
	void ChangeScene(uint32_t _index);

public: // Accessible Member Variable Declarations
	Scene* curScene;

private: // Hidden Member Variable Declarations
	uint32_t flags;

	uint32_t curSceneIndex;
	uint32_t nextSceneIndex;
	uint32_t prevSceneIndex;

	std::unordered_map<uint32_t, Scene*> scenes;
};

#endif