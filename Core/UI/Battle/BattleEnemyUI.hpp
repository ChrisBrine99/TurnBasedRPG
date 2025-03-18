#ifndef BATTLE_ENEMY_UI_HPP
#define BATTLE_ENEMY_UI_HPP

#include "../../Structs/Battle/Combatant.hpp"

#include <string>
#include <vector>

struct EnemyUI;

class BattleEnemyUI {
public: // Constructor and Destructor Declarations
	BattleEnemyUI();
	~BattleEnemyUI() = default;

public: // Main Engine Function Declarations
	bool OnUserCreate();
	bool OnUserUpdate(float_t _deltaTime);
	bool OnUserRender(float_t _deltaTime);

public: // Publicly Accessible Utility Function Declarations
	void ActivateElement(Combatant* _combatant, int32_t _startX, int32_t _startY);
	void DeactivateElement(Combatant* _combatant);
	void UpdateElement(Combatant* _combatant);

private: // Hidden Member Variable Declarations
	std::vector<EnemyUI> uiElements;
};

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Values that are positions for bits within the player member infomation "flags" variable. Toggling them will allow or prevent		//
//  aspects of the struct from executing specific pieces of code during rendering.														//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define FLAG_ENINFO_VISIBLE				0x00000001ui32
#define FLAG_ENINFO_OCCUPIED			0x00000002ui32

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that condense the check required to see if a flag bit is set to 0 or 1 within a party member ui struct within the code.		//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define ENINFO_IS_VISIBLE(_x)			(_x.flags & FLAG_ENINFO_VISIBLE)
#define ENINFO_IS_OCCUPIED(_x)			(_x.flags & FLAG_ENINFO_OCCUPIED)

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Define for the maximum possible width of the HP and MP bars on the party info UI.													//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define EN_INFO_HP_BAR_WIDTH			48

struct EnemyUI {
	int32_t		xPos;
	int32_t		yPos;
	uint32_t	flags;
	uint32_t	hpBarWidth;
	uint16_t	curHitpoints;
	uint16_t	targetHitpoints;
	float_t		barUpdateTimer;
	float_t		visibilityTimer;
	Combatant*	combatant;

	// The default constructor for the EnemyUI struct. It simply initializes all variables with default values.
	EnemyUI() : 
		xPos(0),
		yPos(0),
		flags(0u),
		hpBarWidth(0u),
		curHitpoints(0ui16),
		targetHitpoints(0ui16),
		barUpdateTimer(0.0f),
		visibilityTimer(0.0f),
		combatant(nullptr)
	{}
};

#endif