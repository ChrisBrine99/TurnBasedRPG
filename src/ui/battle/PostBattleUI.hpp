#ifndef POST_BATTLE_UI_HPP
#define POST_BATTLE_UI_HPP

#include <cmath>

class EngineCore;

class PostBattleUI {
public: // Constructor/Destructor Declarations
	PostBattleUI();
	PostBattleUI(const PostBattleUI& _other) = delete;
	~PostBattleUI() = default;

public: // Main Engine Function Declarations
	void OnUserCreate();
	void OnUserDestroy();
	void OnUserRender(EngineCore* _engine, float_t _deltaTime);
};

#endif