#include "BattleUI.hpp"

BattleUI::BattleUI(int32_t _xPos, int32_t _yPos) :
	uiElements(),
	xPos(0),
	yPos(0)
{ // Reserve a bit of memory for 11 structs since there will never be more than 11 combatants in a battle at once.
	uiElements.reserve(PARTY_ACTIVE_MAX_SIZE + BATTLE_MAX_ENEMY_SIZE);
}

bool BattleUI::OnUserCreate() {
	return true;
}

bool BattleUI::OnUserDestroy() {
	uiElements.clear();
	return true;
}

bool BattleUI::OnUserUpdate(float_t _deltaTime) {
	return true;
}

bool BattleUI::OnUserRender(float_t _deltaTime) {
	for (auto& _data : uiElements) {

	}
	return true;
}

void BattleUI::InitializeElement(Combatant* _combatant, int32_t _xPos, int32_t _yPos) {
	if (uiElements.size() >= PARTY_ACTIVE_MAX_SIZE + BATTLE_MAX_ENEMY_SIZE)
		return;


}