#ifndef LEVEL_SCENE_HPP
#define LEVEL_SCENE_HPP

#include "Scene.hpp"

class LevelScene : public Scene {
public: // Constructor/Destructor Declarations
	LevelScene();
	LevelScene(const LevelScene& _other) = delete;
	~LevelScene() = default;
public: // Main Engine Function Declarations
	bool OnUserCreate() override;
	bool OnUserDestroy() override;
	bool OnUserUpdate(float_t _deltaTime) override;
	bool OnUserRender(EngineCore* _engine, float_t _deltaTime) override;
};

#endif