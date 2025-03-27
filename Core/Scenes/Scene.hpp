#ifndef SCENE_HPP
#define SCENE_HPP

#include "../Utils/SceneMacros.hpp"

#include <math.h>
#include <cinttypes>

class EngineCore;

class Scene {
public: // Constructor/Destructor Declarations and Definitions
	Scene(uint32_t _index) :
		sceneIndex(_index)
	{}
	~Scene() = default;

public: // Main Engine/Engine Extension Function Declarations
	virtual bool OnUserCreate() = 0;
	virtual bool OnUserDestroy() = 0;
	virtual bool OnUserUpdate(float_t _deltaTime) = 0;
	virtual bool OnUserRender(EngineCore* _engine, float_t _deltaTime) = 0;

	virtual bool OnBeforeUserUpdate(float_t _deltaTime) = 0;
	virtual bool OnAfterUserUpdate(float_t _deltaTime) = 0;

protected: // Child-Only Accessible Member Variable Declarations
	uint32_t sceneIndex;
};

#endif