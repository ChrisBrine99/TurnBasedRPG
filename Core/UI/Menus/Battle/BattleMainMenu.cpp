#include "BattleMainMenu.hpp"

#include "../../../Structs/Battle/Combatant.hpp"
#include "../General/ConfirmWindow.hpp"
#include "BattleSkillMenu.hpp"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that explain how the number they contain is utilized by this menu's state machine. Defined here since these values aren't	//
//	used in this context anywhere outside of this menu.	The first two values are already used by inherited menu states.					//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define BTL_MENU_STATE_IN_SKILLS		2ui8
#define BTL_MENU_STATE_IN_ITEMS			3ui8

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that are the index values for the options they represent within the menu's option vector. Defined here since no other		//
//	class/piece of code should ever require these defines outside of this menu.															//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define BTL_MENU_OPTION_SKILLS			0ui64
#define BTL_MENU_OPTION_ITEMS			1ui64
#define BTL_MENU_OPTION_GUARD			2ui64
#define BTL_MENU_OPTION_SWITCH			3ui64
#define BTL_MENU_OPTION_ESCAPE			4ui64

BattleMainMenu::BattleMainMenu() :
	Menu(),
	skillMenu(nullptr),
	confirmWindow(nullptr)
{}

bool BattleMainMenu::OnUserCreate() {
	InitializeParams(INVALID_STATE, 1ui8, 7ui8, 1ui8, 0ui8, 0ui8, 0xFFui8, FLAG_MENU_BLOCK_INPUT);
	InitializeOptionParams(15, 200, 0, 10);
	//InitializeDescriptionParams(VIEWPORT_WIDTH >> 1, VIEWPORT_HEIGHT - 15);

	AddOption(0, 0, "Skills",	"Opens the menu that lists all active skills for the current party member.");
	AddOption(0, 0, "Items",	"Choose an item to use on the enemy or your allies.");
	AddOption(0, 0, "Guard",	"Forgo your turn to reduce effects from all incoming attacks until the next turn.");
	AddOption(0, 0, "Switch",	"Switch the current party member out for another within the party roster.");
	AddOption(0, 0, "Escape",	"Attempt to run away from the current battle.");

	skillMenu = new BattleSkillMenu();
	skillMenu->OnUserCreate();

	return true;
}

bool BattleMainMenu::OnUserUpdate(float_t _deltaTime) {
	switch (curState) {
	case MENU_STATE_DEFAULT:			return StateDefault(_deltaTime);
	case MENU_STATE_PROCESS_SELECTION:	return StateProcessSelection();
	case BTL_MENU_STATE_IN_SKILLS:		return StateInsideSkills();
	case BTL_MENU_STATE_IN_ITEMS:		return StateInsideItems();
	case INVALID_STATE:					return true;
	}

	return false;
}

void BattleMainMenu::PrepareForActivation(uint8_t _state, Combatant* _curCombatant) {
	Menu::PrepareForActivation(_state);
	skillMenu->GenerateMenuOptions(_curCombatant);
}

bool BattleMainMenu::StateProcessSelection() {
	switch (selOption) {
	case BTL_MENU_OPTION_SKILLS: // Switches primary control to the skill menu so the use can select a skill to utilize.
		SET_NEXT_STATE(BTL_MENU_STATE_IN_SKILLS);
		skillMenu->PrepareForActivation(MENU_STATE_DEFAULT, this);
		break;
	case BTL_MENU_OPTION_ITEMS:
		SET_NEXT_STATE(BTL_MENU_STATE_IN_ITEMS);
		// TODO -- Prepare item menu for activation here.
		break;
	case BTL_MENU_OPTION_GUARD:
		// TODO -- Open confirmation window asking user if they wish to guard with this character.
		break;
	case BTL_MENU_OPTION_SWITCH:
		// TODO -- Open battle's party roster selection menu.
		break;
	case BTL_MENU_OPTION_ESCAPE:
		// TODO -- Open confimation window asking user if they wish to flee.
		break;
	}

	return true;
}

bool BattleMainMenu::StateInsideSkills() {
	if (FLAG_IS_MENU_RETURN_ACTIVE) {
		skillMenu->PrepareForDeactivation();
		selOption = 0xFFui8;
	}

	return true;
}

bool BattleMainMenu::StateInsideItems() {
	if (FLAG_IS_MENU_RETURN_ACTIVE) {
		SET_NEXT_STATE(MENU_STATE_DEFAULT);
		// TODO -- Deactivate the item menu here.
		selOption = 0xFFui8;
	}

	return true;
}