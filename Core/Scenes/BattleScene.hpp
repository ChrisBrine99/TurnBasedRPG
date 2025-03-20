#ifndef BATTLE_SCENE_HPP
#define BATTLE_SCENE_HPP

#include "Scene.hpp"

class BattleScene : public Scene {
public: // Constructor/Destructor Declarations
	BattleScene(uint32_t _index);
	~BattleScene() = default;

public: // Main Engine/Engine Extension Function Declarations
	bool OnUserCreate() override;
	bool OnUserDestroy() override;
	bool OnUserUpdate(float_t _deltaTime) override;
	bool OnUserRender(float_t _deltaTime) override;

	bool OnBeforeUserUpdate(float_t _deltaTime) override;
	bool OnAfterUserUpdate(float_t _deltaTime) override;
};

#endif