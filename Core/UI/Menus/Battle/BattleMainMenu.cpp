#include "BattleMainMenu.hpp"

#define BTL_MENU_STATE_ACCEPT_INPUT		0ui8

#define BTL_MENU_OPTION_SKILLS			0ui64
#define BTL_MENU_OPTION_ITEMS			1ui64
#define BTL_MENU_OPTION_GUARD			2ui64
#define BTL_MENU_OPTION_SWITCH			3ui64
#define BTL_MENU_OPTION_ESCAPE			4ui64

BattleMainMenu::BattleMainMenu() :
	Menu()
{}

bool BattleMainMenu::OnUserCreate() {
	InitializeParams(INVALID_STATE, 1ui8, 6ui8, 1ui8);
	InitializeOptionParams(15, 200, 0, 10);
	InitializeDescriptionParams(VIEWPORT_WIDTH >> 1, VIEWPORT_HEIGHT - 15);

	AddOption(0, 0, "Skills",	"Opens the menu that lists all active skills for the current party member.");
	AddOption(0, 0, "Items",	"Choose an item to use on the enemy or your allies.");
	AddOption(0, 0, "Guard",	"Forgo your turn to reduce effects from all incoming attacks until the next turn.");
	AddOption(0, 0, "Switch",	"Switch the current party member out for another within the party roster.");
	AddOption(0, 0, "Escape",	"Attempt to run away from the current battle.");

	flags &= ~(FLAG_MENU_VISIBLE | FLAG_MENU_ACTIVE_STATE);
	flags |=   FLAG_MENU_BLOCK_INPUT;

	return true;
}

bool BattleMainMenu::OnUserUpdate(float_t _deltaTime) {
	switch (curState) {
	case BTL_MENU_STATE_ACCEPT_INPUT:	return StateAcceptInput(_deltaTime);
	case INVALID_STATE:					return true;
	}

	return false;
}

bool BattleMainMenu::OnUserRender(float_t _deltaTime) {
	RenderVisibleOptions(_deltaTime);
	return true;
}

void BattleMainMenu::PrepareForPlayerTurn() {
	SET_NEXT_STATE(BTL_MENU_STATE_ACCEPT_INPUT);

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

bool BattleMainMenu::StateAcceptInput(float_t _deltaTime) {
	UpdateCursor(_deltaTime);
	return true;
}