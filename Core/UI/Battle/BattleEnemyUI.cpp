#include "BattleEnemyUI.hpp"

#include "../../Singletons/EngineCore.hpp"
#include "../../Utils/BattleMacros.hpp"

BattleEnemyUI::BattleEnemyUI() :
	uiElements()
{ // Reserve enough memory to contain eight elements since that is the max number of enemies that can participate in a battle at once.
	uiElements.reserve(BATTLE_MAX_ENEMY_SIZE);
}

bool BattleEnemyUI::OnUserCreate() {
	for (size_t i = 0ui64; i < BATTLE_MAX_PARTY_SIZE; i++)
		uiElements.push_back(EnemyUI()); // Initialize all eight instaces upon creation of the UI.
	return true;
}