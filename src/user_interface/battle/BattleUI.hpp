#ifndef BATTLE_UI_HPP
#define BATTLE_UI_HPP

#include "../../utility/BattleMacros.hpp"

#include <array>
#include <math.h>

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Determines how fast the ui elements can update their visual representations per second (1.0f = one full second).					//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define BATUI_UPDATE_INTERVAL			0.01f

class EngineCore;
struct BattleUIElement;
struct Combatant;

class BattleUI {
public: // Constructor/Destructor Declarations
	BattleUI();
	BattleUI(const BattleUI& _other) = delete;
	~BattleUI() = default;

public: // Main Engine Function Declarations
	void OnUserCreate();
	void OnUserDestroy();
	void OnUserUpdate(float_t _deltaTime);
	void OnUserRender(EngineCore* _engine, float_t _deltaTime);

public: // Publicly Accessible Utility Function Declarations
	void ActivateElement(Combatant* _combatant, size_t _index);

public: // Publicly Accessible Member Variable Declarations
	std::array<BattleUIElement*, BATTLE_TOTAL_COMBATANTS> uiElements;
	uint8_t totalPartyMembers;
	float_t updateTimer;
};

#endif