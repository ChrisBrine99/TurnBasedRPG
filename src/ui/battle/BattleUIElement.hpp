#ifndef BATTLE_UI_ELEMENT_HPP
#define BATTLE_UI_ELEMENT_HPP

#include "../../struct/battle/Combatant.hpp"
#include "../../singleton/EngineCore.hpp"

#include <math.h>

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that represent the bits within the battle ui element's flags variable. These bits will allow or diable certain functions	//
//	of the ui element as the ui is processed/rendered to the screen.																	//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define FLAG_BATUI_ELEMENT_VISIBLE			0x00000001ui32
#define FLAG_BATUI_ELEMENT_IN_USE			0x00000002ui32
#define FLAG_BATUI_ELEMENT_USE_TIMER		0x00000004ui32
#define FLAG_BATUI_ELEMENT_HP_SHOWN			0x00000008ui32
#define FLAG_BATUI_ELEMENT_MP_SHOWN			0x00000010ui32

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that condense the checks for each respective flag bit into a single macro each.												//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define BATUI_ELEMENT_IS_VISIBLE			(flags & FLAG_BATUI_ELEMENT_VISIBLE)
#define BATUI_ELEMENT_IS_IN_USE				(flags & FLAG_BATUI_ELEMENT_IN_USE)
#define BATUI_ELEMENT_USES_TIMER			(flags & FLAG_BATUI_ELEMENT_USE_TIMER)
#define BATUI_ELEMENT_IS_HP_SHOWN			(flags & FLAG_BATUI_ELEMENT_HP_SHOWN)
#define BATUI_ELEMENT_IS_MP_SHOWN			(flags & FLAG_BATUI_ELEMENT_MP_SHOWN)

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines for the width of the ui element's HP and MP bars as both an integer and float, respectively.								//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define BATUI_ELEMENT_BAR_WIDTH				40i32
#define BATUI_ElEMENT_BAR_HEIGHT			2i32

class EngineCore;

class BattleUIElement {
public: // Constructor/Destructor Declarations
	BattleUIElement();
	BattleUIElement(const BattleUIElement& _other) = delete;
	~BattleUIElement() = default;

public: // Main Engine Function Declarations
	virtual void OnUserUpdate();
	virtual void OnUserRender(EngineCore* _engine);

public: // Publicly Accessible Utility Function Declarations
	virtual void ActivateElement(int32_t _x, int32_t _y, int32_t _hpBarX, int32_t _hpBarY, int32_t _mpBarX, int32_t _mpBarY, Combatant* _combatant, uint32_t _flags);
	void ShowElement(float_t _time);

public: // Publicly Accessible Member Variable Declarations
	uint32_t		flags;
	int32_t			x;
	int32_t			y;

protected: // Hidden (Only Accesible To Children) Member Variable Declarations
	uint16_t		curHitpoints;
	uint16_t		curMagicpoints;
	uint16_t		curHpBarWidth;
	uint16_t		curMpBarWidth;
	int32_t			hpBarX;
	int32_t			hpBarY;
	int32_t			mpBarX;
	int32_t			mpBarY;
	float_t			visibleTime;
	Combatant*		combatant;
};

#endif