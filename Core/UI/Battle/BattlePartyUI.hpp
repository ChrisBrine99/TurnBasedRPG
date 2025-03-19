#ifndef BATTLE_PARTY_INFO_UI_HPP
#define BATTLE_PARTY_INFO_UP_HPP

#include "../../Structs/Battle/Combatant.hpp"

#include <string>
#include <vector>

struct PartyMemberUI;

class BattlePartyUI {
public: // Constructor and Destructor Declarations
	BattlePartyUI();
	~BattlePartyUI() = default;

public: // Main Engine Function Declarations
	bool OnUserCreate();
	bool OnUserUpdate(float_t _deltaTime);
	bool OnUserRender(float_t _deltaTime);

public: // Publicly Accessible Utility Function Declarations
	void ActivateElement(Combatant* _combatant, int32_t _startX, int32_t _startY);
	void DeactivateElement(Combatant* _combatant);
	void UpdateOnScreenElements(uint32_t _numerator, uint32_t _denominator, uint32_t& _barValueWidth, std::string& _barValueString);

public: // Publicly Accessible Member Variable Declarations
	int32_t xPos;
	int32_t yPos;

private: // Hidden Member Variable Declarations
	std::vector<PartyMemberUI> uiElements;
};

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Values that are positions for bits within the player member infomation "flags" variable. Toggling them will allow or prevent		//
//  aspects of the struct from executing specific pieces of code during rendering.														//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define FLAG_PMINFO_VISIBLE				0x00000001ui32
#define FLAG_PMINFO_OCCUPIED			0x00000002ui32

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that condense the check required to see if a flag bit is set to 0 or 1 within a given party member ui struct.				//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define PMINFO_IS_VISIBLE(_x)			(_x.flags & FLAG_PMINFO_VISIBLE)
#define PMINFO_IS_OCCUPIED(_x)			(_x.flags & FLAG_PMINFO_OCCUPIED)

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Define for the maximum possible width of the HP and MP bars on the party info UI.													//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define PMINFO_UI_BAR_WIDTH				48

struct PartyMemberUI {
	int32_t				xPos;
	int32_t				yPos;

	std::string			sName;
	std::string			sLevel;
	std::string			sCurHitpoints;
	std::string			sCurMagicpoints;

	uint32_t			flags;

	uint32_t			hpBarWidth;
	uint32_t			mpBarWidth;

	uint16_t			curHitpoints;
	uint16_t			curMagicpoints;
	float_t				updateTimer;

	Combatant*			combatant;

	// The default constructor for the PartyMemberUI struct. It simply initializes all variables with default values.
	PartyMemberUI() :
		xPos(0),
		yPos(0),
		sName(""),
		sLevel(""),
		sCurHitpoints(""),
		sCurMagicpoints(""),
		flags(0u),
		hpBarWidth(0u),
		mpBarWidth(0u),
		curHitpoints(0ui16),
		curMagicpoints(0ui16),
		updateTimer(0.0f),
		combatant(nullptr)
	{}
};

#endif