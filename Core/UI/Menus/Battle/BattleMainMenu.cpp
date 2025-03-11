#include "BattleMainMenu.hpp"

BattleMainMenu::BattleMainMenu() :
	Menu()
{}

bool BattleMainMenu::OnUserCreate() {
	InitializeParams(INVALID_STATE, 4ui8, 4ui8, 4ui8, 1ui8, 1ui8);
	InitializeOptionParams(0u, 0u, 0u, 0u);

	for (size_t i = 0ui64; i < 47ui64; i++)
		AddOption(0u, 0u, "Test" + std::to_string(i));

	std::cout << menuOptions.size() << std::endl;
	return true;
}

bool BattleMainMenu::OnUserUpdate(float_t _deltaTime) {
	if (!FLAG_IS_MENU_ACTIVE)
		return true;

	UpdateCursor(_deltaTime);
	return true;
}

bool BattleMainMenu::OnUserRender(float_t _deltaTime) {
	return true;
}