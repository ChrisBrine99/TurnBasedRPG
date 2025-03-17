#include "BattleMainMenu.hpp"

#include "../../../Structs/Battle/Combatant.hpp"
#include "BattleSkillMenu.hpp"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that explain how the number they contain is utilized by the menu's state machine. Defined here since these values aren't	//
//	used in this context anywhere outside of this menu.																					//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define BTL_MENU_STATE_DEFAULT			0ui8
#define BTL_MENU_PROCESS_SELECTION		1ui8
#define BTL_MENU_STATE_IN_SKILLS		2ui8

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
	skillMenu(nullptr)
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
	case BTL_MENU_STATE_DEFAULT:		return StateDefault(_deltaTime);
	case BTL_MENU_PROCESS_SELECTION:	return StateProcessSelection();
	case BTL_MENU_STATE_IN_SKILLS:		return StateInsideSkills();
	case INVALID_STATE:					return true;
	}

	return false;
}

bool BattleMainMenu::OnUserRender(float_t _deltaTime) {
	RenderVisibleOptions(_deltaTime);
	return true;
}

void BattleMainMenu::PrepareForPlayerTurn(Combatant* _curCombatant) {
	SET_NEXT_STATE(BTL_MENU_STATE_DEFAULT);
	skillMenu->GenerateMenuOptions(_curCombatant);

	curOption = 0ui8;
	selOption = 0xFFui8;

	flags &= ~FLAG_MENU_BLOCK_INPUT;
	flags |=  FLAG_MENU_VISIBLE | FLAG_MENU_ACTIVE_STATE;
}

void BattleMainMenu::PostPlayerTurn() {
	SET_NEXT_STATE(INVALID_STATE);

	flags |=   FLAG_MENU_BLOCK_INPUT;
	flags &= ~(FLAG_MENU_VISIBLE | FLAG_MENU_ACTIVE_STATE);
}

bool BattleMainMenu::StateDefault(float_t _deltaTime) {
	if (FLAG_IS_MENU_SELECT_ACTIVE) { // Capture what option was selected so it can be parsed in the next state.
		SET_NEXT_STATE(BTL_MENU_PROCESS_SELECTION);
		selOption = curOption;
		return true;
	}

	UpdateCursor(_deltaTime);
	return true;
}

bool BattleMainMenu::StateProcessSelection() {
	switch (selOption) {
	case BTL_MENU_OPTION_SKILLS: // Switches primary control to the skill menu so the use can select a skill to utilize.
		SET_NEXT_STATE(BTL_MENU_STATE_IN_SKILLS);
		skillMenu->PrepareForActivation(this);
		break;
	case BTL_MENU_OPTION_ITEMS:
		// TODO -- Move control over to the battle's item menu.
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
		SET_NEXT_STATE(BTL_MENU_STATE_DEFAULT);
		skillMenu->PrepareForDeactivation();
		selOption = 0xFFui8;
	}

	return true;
}