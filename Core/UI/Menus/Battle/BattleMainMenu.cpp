#include "BattleMainMenu.hpp"

BattleMainMenu::BattleMainMenu() :
	Menu()
{}

bool BattleMainMenu::OnUserCreate() {
	InitializeParams(INVALID_STATE, 5ui8, 5ui8, 3ui8, 1ui8, 1ui8);
	InitializeOptionParams(15u, 15u, 50u, 10u);

	for (size_t i = 0ui64; i < 22ui64; i++)
		AddOption(0u, 0u, "Test" + std::to_string(i));
	return true;
}

bool BattleMainMenu::OnUserUpdate(float_t _deltaTime) {
	if (!FLAG_IS_MENU_ACTIVE)
		return true;

	UpdateCursor(_deltaTime);
	return true;
}

bool BattleMainMenu::OnUserRender(float_t _deltaTime) {
	RenderVisibleOptions(_deltaTime);
	return true;
}