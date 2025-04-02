#ifndef BATTLE_UI_HPP
#define BATTLE_UI_HPP

#include "../../singleton/EngineCore.hpp"
#include "../../utility/BattleMacros.hpp"

#include <array>

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Determines how fast the ui elements can update their visual representations per second (1.0f = one full second).					//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define BATUI_UPDATE_INTERVAL			2.0f

class EngineCore;
class BattleUIElement;
struct Combatant;
struct TextElement;

class BattleUI {
public: // Constructor/Destructor Declarations
	BattleUI();
	BattleUI(const BattleUI& _other) = delete;
	~BattleUI() = default;

public: // Main Engine Function Declarations
	void OnUserCreate();
	void OnUserDestroy();
	void OnUserUpdate();
	void OnUserRender(EngineCore* _engine);

public: // Publicly Accessible Utility Function Declarations
	void ActivateElement(Combatant* _combatant, size_t _index);
	void CreateText(const std::string& _string, size_t _index, olc::Pixel _color = COLOR_WHITE, float_t _xOffset = 0.0f, float_t _yOffset = 0.0f, float_t _scale = 1.0f);
	void CreateDamageText(uint16_t _value, size_t _index, olc::Pixel _color = COLOR_WHITE, float_t _xOffset = 0.0f, float_t _yOffset = 0.0f, float_t _scale = 1.0f);

public: // Publicly Accessible Member Variable Declarations
	std::array<BattleUIElement*, BATTLE_TOTAL_COMBATANTS>	uiElements;
	std::vector<std::pair<float_t, TextElement*>>			textElements;
	uint8_t totalPartyMembers;
	float_t updateTimer;
};

#endif