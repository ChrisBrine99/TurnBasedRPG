#ifndef BATTLE_UI_HPP
#define BATTLE_UI_HPP

#include "../../Structs/Battle/Combatant.hpp"
#include "../../Utils/BattleMacros.hpp"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Determines how fast the ui elements can update their visual representations per second (1.0f = one full second).					//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define BATUI_UPDATE_INTERVAL			0.01f

struct BattleUIElement;

class BattleUI {
public: // Constructor/Destructor Declarations
	BattleUI();
	BattleUI(const BattleUI& _other) = delete;
	~BattleUI() = default;

public: // Main Engine Function Declarations
	void OnUserCreate();
	void OnUserDestroy();
	void OnUserUpdate(float_t _deltaTime);
	void OnUserRender(float_t _deltaTime);

public: // Publicly Accessible Utility Function Declarations
	uint32_t ActivateElement(Combatant* _combatant);
	void DeactivateElement(uint32_t _element);
	void ShowElement(uint32_t _element, float_t _time);

public: // Publicly Accessible Member Variable Declarations
	std::array<BattleUIElement*, BATTLE_TOTAL_COMBATANTS> uiElements;
	uint8_t totalPartyMembers;
	float_t updateTimer;
};

#endif