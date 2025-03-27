#ifndef BATTLE_UI_ELEMENT_HPP
#define BATTLE_UI_ELEMENT_HPP

#include "../../Structs/Battle/Combatant.hpp"
#include "../../Singletons/EngineCore.hpp"

#include <cstdint>
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

#define BATUI_ELEMENT_BAR_WIDTH				40ui16
#define BATUI_ELEMENT_BAR_WIDTH_F			40.0f
#define BATUI_ElEMENT_BAR_HEIGHT_F			2.0f

class EngineCore;

struct BattleUIElement {
	uint32_t		flags;
	int32_t			x;
	int32_t			y;
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

public: // Constructor/Destructor Declarations
	BattleUIElement() :
		flags(0ui32),
		visibleTime(0.0f),
		x(0i32),
		y(0i32),
		curHitpoints(0ui16),
		curMagicpoints(0ui16),
		curHpBarWidth(0ui16),
		curMpBarWidth(0ui16),
		hpBarX(0i32),
		hpBarY(0i32),
		mpBarX(0i32),
		mpBarY(0i32),
		combatant(nullptr)
	{}
	BattleUIElement(const BattleUIElement& _other) = delete;
	~BattleUIElement() = default;

public: // Main Engine Function Declarations
	virtual void OnUserUpdate();
	virtual void OnUserRender(EngineCore* _engine);

public: // Publicly Accessible Utility Function Declarations
	virtual void ActivateElement(int32_t _x, int32_t _y, int32_t _hpBarX, int32_t _hpBarY, int32_t _mpBarX, int32_t _mpBarY, Combatant* _combatant, uint32_t _flags);
	void ShowElement(float_t _time);
};

#endif