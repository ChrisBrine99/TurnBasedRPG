#ifndef BATTLE_UI_HPP
#define BATTLE_UI_HPP

#include "../../Singletons/BattleManager.hpp"

struct CombatantInfo;

class BattleUI {
public: // Constructor/Destructor Declarations
	BattleUI() = delete;
	BattleUI(int32_t _xPos, int32_t _yPos);
	~BattleUI() = default;

	// The Battle UI class will never need to be copied, so delete those unnecessary constructors to signal to the compiler
	// that they don't need to be generated during compilation.
	BattleUI(BattleUI& _other) = delete;
	BattleUI(BattleUI&& _other) = delete;

public: // Main Engine Function Declarations
	bool OnUserCreate();
	bool OnUserDestroy();
	bool OnUserUpdate(float_t _deltaTime);
	bool OnUserRender(float_t _deltaTime);

public: // Publicly Accessible Utility Function Declarations
	void InitializeElement(Combatant* _combatant, int32_t _xPos = 0, int32_t _yPos = 0);
	//void UpdateElementName()

public: // Member Variable Declarations
	std::unordered_map<size_t, CombatantInfo> uiElements;
	int32_t xPos;
	int32_t yPos;
};

struct CombatantInfo {
	std::string sName;
	std::string sLevel;
	std::string sCurHitpoints;
	std::string sCurMagicpoints;

	int32_t xPos;
	int32_t yPos;
	uint32_t maxHitpoints;
	uint32_t maxMagicpoints;

	Combatant* combatant;

	CombatantInfo() = delete; // Default constructor shouldn't exist. The Combatant UI element requires a combatant pointer and positional data.

	// 
	CombatantInfo(Combatant* _combatant, int32_t _xPos, int32_t _yPos) :
		sName(""),
		sLevel(""),
		sCurHitpoints("0"),
		sCurMagicpoints("0"),
		xPos(_xPos),
		yPos(_yPos),
		maxHitpoints(0u),
		maxMagicpoints(0u),
		combatant(_combatant)
	{}

	// The Combatant UI element struct should never be copied (Only ever referenced), so its copy constructor should be 
	// unavailable to utilize anywhere within the code.
	CombatantInfo(const Combatant& _other) = delete;
	CombatantInfo(const Combatant&& _other) = delete;
};

#endif