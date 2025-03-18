#include "BattleMainMenu.hpp"

#include "../../../Structs/Battle/Combatant.hpp"
#include "../General/ConfirmWindow.hpp"
#include "BattleSkillMenu.hpp"

BattleMainMenu::BattleMainMenu() :
	Menu(),
	skillMenu(nullptr),
	confirmWindow(nullptr)
{}

bool BattleMainMenu::OnUserCreate() {
	InitializeParams(STATE_INVALID, 1ui8, 7ui8, 1ui8, 0ui8, 0ui8, 0xFFui8, FLAG_MENU_BLOCK_INPUT);
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
	case STATE_MENU_DEFAULT:			return StateDefault(_deltaTime);
	case STATE_MENU_PROCESS_SELECTION:	return StateProcessSelection();
	case STATE_BTLMENU_IN_SKILLS:		return StateInsideSkills();
	case STATE_BTLMENU_IN_ITEMS:		return StateInsideItems();
	case STATE_INVALID:					return true;
	}

	return false;
}

void BattleMainMenu::PrepareForActivation(uint8_t _state, Combatant* _curCombatant) {
	Menu::PrepareForActivation(_state);
	skillMenu->GenerateMenuOptions(_curCombatant);
}

bool BattleMainMenu::StateProcessSelection() {
	flags |= FLAG_MENU_BLOCK_INPUT;

	switch (selOption) {
	case OPTION_BTLMENU_SKILLS: // Switches primary control to the skill menu so the use can select a skill to utilize.
		SET_NEXT_STATE(STATE_BTLMENU_IN_SKILLS);
		skillMenu->PrepareForActivation(STATE_MENU_DEFAULT, this);
		break;
	case OPTION_BTLMENU_ITEMS:
		SET_NEXT_STATE(STATE_BTLMENU_IN_ITEMS);
		flags &= ~FLAG_MENU_BLOCK_INPUT;
		// TODO -- Prepare item menu for activation here.
		break;
	case OPTION_BTLMENU_GUARD:
		SET_NEXT_STATE(STATE_BTLMENU_IN_ITEMS);
		flags &= ~FLAG_MENU_BLOCK_INPUT;
		// TODO -- Open confirmation window asking user if they wish to guard with this character.
		break;
	case OPTION_BTLMENU_SWITCH:
		SET_NEXT_STATE(STATE_BTLMENU_IN_ITEMS);
		flags &= ~FLAG_MENU_BLOCK_INPUT;
		// TODO -- Open battle's party roster selection menu.
		break;
	case OPTION_BTLMENU_ESCAPE:
		SET_NEXT_STATE(STATE_BTLMENU_IN_ITEMS);
		flags &= ~FLAG_MENU_BLOCK_INPUT;
		// TODO -- Open confimation window asking user if they wish to flee.
		break;
	}

	return true;
}

bool BattleMainMenu::StateInsideSkills() {
	if (skillMenu->GetPressedInputs() & FLAG_INPUT_MENU_RETURN)
		skillMenu->PrepareForDeactivation();

	return true;
}

bool BattleMainMenu::StateInsideItems() {
	if (MINPUT_IS_RETURN_PRESSED) {
		SET_NEXT_STATE(STATE_MENU_DEFAULT);
		// TODO -- Deactivate the item menu here.
		selOption = MENU_SELECTION_INVALID;
	}

	return true;
}