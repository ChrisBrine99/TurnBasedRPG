#ifndef SCENE_HPP
#define SCENE_HPP

#include "../utility/SceneMacros.hpp"

#include <cstdint>

class EngineCore;

class Scene {
public: // Constructor/Destructor Declarations and Definitions
	Scene(uint32_t _index) :
		sceneIndex(_index)
	{}
	Scene(const Scene& _other) = delete;
	~Scene() = default;

public: // Main Engine/Engine Extension Function Declarations
	virtual bool OnUserCreate() = 0;
	virtual bool OnUserDestroy() = 0;
	virtual bool OnUserUpdate() = 0;
	virtual bool OnUserRender(EngineCore* _engine) = 0;

	virtual bool OnBeforeUserUpdate() { return true; };
	virtual bool OnAfterUserUpdate() { return true; };

protected: // Child-Only Accessible Member Variable Declarations
	uint32_t sceneIndex;
};

#endif