#ifndef BATTLE_MAIN_MENU_HPP
#define BATTLE_MAIN_MENU_HPP

#include "../Menu.hpp"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that explain how the number they contain is utilized by this menu's state machine. Defined here since these values aren't	//
//	used in this context anywhere outside of this menu.	The first two values are already used by inherited menu states.					//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define STATE_BTLMENU_IN_SKILLS			2ui8
#define STATE_BTLMENU_IN_ITEMS			3ui8

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that are the index values for the options they represent within the menu's option vector. Defined here since no other		//
//	class/piece of code should ever require these defines outside of this menu.															//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define OPTION_BTLMENU_SKILLS			0ui64
#define OPTION_BTLMENU_ITEMS			1ui64
#define OPTION_BTLMENU_GUARD			2ui64
#define OPTION_BTLMENU_SWITCH			3ui64
#define OPTION_BTLMENU_ESCAPE			4ui64

struct Combatant;
class BattleSkillMenu;
class ConfirmWindow;

class BattleMainMenu : public Menu {
public: // Constructor/Destructor Declaration
	BattleMainMenu();
	~BattleMainMenu() = default;

public: // Inherited Function Declarations
	bool OnUserCreate() override;
	bool OnUserUpdate(float_t _deltaTime) override;

public: // Publicly Accessibly Utility Function Declarations
	void PrepareForActivation(uint8_t _state, Combatant* _curCombatant);

private: // State Function Declarations
	bool StateProcessSelection() override;
	bool StateInsideSkills();
	bool StateInsideItems();

private: // Hidden Member Variable Declarations
	BattleSkillMenu*	skillMenu;
	ConfirmWindow*		confirmWindow;
};

#endif