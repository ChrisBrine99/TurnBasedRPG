#ifndef BATTLE_PARTY_INFO_UI_HPP
#define BATTLE_PARTY_INFO_UP_HPP

#include <string>
#include <vector>

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	
// ------------------------------------------------------------------------------------------------------------------------------------	//



// ------------------------------------------------------------------------------------------------------------------------------------	//
//	
// ------------------------------------------------------------------------------------------------------------------------------------	//


struct PlayerCharacter;
struct PartyMemberUI;

class BattlePartyInfoUI {
public: // Constructor and Destructor Declarations
	BattlePartyInfoUI();
	~BattlePartyInfoUI() = default;

public: // Main Engine Function Declarations
	bool OnUserCreate();
	bool OnUserRender(float _deltaTime);

public: // Publicly Accessible Utility Function Declarations
	void ActivateElement(size_t _activeSlot);
	void DeactivateElement(size_t _uiSlot);

private: // Publicly Accessible Member Variable Declarations
	uint32_t flags;

	std::vector<PartyMemberUI> uiElements;
};

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define FLAG_PMINFO_VISIBLE				0x00000001ui32
#define FLAG_PMINFO_OCCUPIED			0x00000002ui32

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define FLAG_IS_PMINFO_VISIBLE(_x)		(uiElements[_x].flags & FLAG_PMINFO_VISIBLE)
#define FLAG_IS_PMINFO_OCCUPIED(_x)		(uiElements[_x].flags & FLAG_PMINFO_OCCUPIED)

struct PartyMemberUI {
	int32_t				xPos;
	int32_t				yPos;

	std::string			name;
	std::string			level;
	std::string			curHitpoints;
	std::string			curMagicpoints;

	uint32_t			hpBarWidth;
	uint32_t			mpBarWidth;
		
	uint32_t			flags;

	PlayerCharacter*	character;

	// 
	PartyMemberUI() :
		xPos(0),
		yPos(0),
		name(""),
		level(""),
		curHitpoints(""),
		curMagicpoints(""),
		hpBarWidth(0u),
		mpBarWidth(0u),
		flags(0u),
		character(nullptr)
	{}
};

#endif