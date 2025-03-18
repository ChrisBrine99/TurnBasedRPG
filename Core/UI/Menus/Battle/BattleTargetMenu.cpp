#include "BattleTargetMenu.hpp"

#include "../../../Singletons/BattleManager.hpp"
#include "../../../Structs/Battle/Skill.hpp"
#include "BattleSkillMenu.hpp"

BattleTargetMenu::BattleTargetMenu() :
	Menu(),
	validTargets()
{ // Reserve enough memory to store the maximum number of combatants in a battle at any one time.
	validTargets.reserve(BATTLE_TOTAL_COMBATANTS);
}

bool BattleTargetMenu::OnUserCreate() {
	return true;
}

void BattleTargetMenu::PrepareForActivation(uint8_t _state, BattleSkillMenu* _bSkillMenu, Skill* _skill) {
	Menu::PrepareForActivation(_state);
	upperMenu = _bSkillMenu;

	DetermineValidTargets(_skill->targeting);
}

void BattleTargetMenu::DetermineValidTargets(uint8_t _targeting) {
	if (_targeting == TARGET_INVALID)
		return;

	BattleManager* _manager		= GET_SINGLETON(BattleManager);
	Combatant* _curCombatant	= _manager->curCombatant;
	bool _shouldSkipCaster		= (_targeting == TARGET_SINGLE_ALLY || _targeting == TARGET_ALL_ALLY);
	switch (_targeting) {
	case TARGET_ALL_ENEMY:			// Grab all enemy targets for the user to select from.
	case TARGET_ALL_ENEMY_SELF:
	case TARGET_SINGLE_ENEMY:
	case TARGET_SINGLE_ENEMY_SELF:
		for (size_t i : _manager->turnOrder) {
			if (FLAG_IS_COMBATANT_PLAYER(_manager->combatants[i]))
				continue; // Skip over all player party combatants
			validTargets.push_back(i);
		}
		return;
	case TARGET_ALL_ALLY:			// Grab all allied targets for the user to select from.
	case TARGET_ALL_ALLY_SELF:
	case TARGET_SINGLE_ALLY:		
	case TARGET_SINGLE_ALLY_SELF:
		for (size_t i : _manager->turnOrder) {
			if (!FLAG_IS_COMBATANT_PLAYER(_manager->combatants[i]) || (_curCombatant == _manager->combatants[i] && _shouldSkipCaster))
				continue; // Skip over all enemy combatants OR the caster if they can't be targeted by the skill.
			validTargets.push_back(i);
		}
		return;
	case TARGET_EVERYONE:			// Skill hits everyone; copy over the entire contents of the turn order.
	case TARGET_EVERYONE_SELF:
		for (size_t i : _manager->turnOrder) {
			if (_curCombatant == _manager->combatants[i] && _targeting == TARGET_EVERYONE)
				continue; // Skips over the caster if required by the skill.
			validTargets.push_back(i);
		}
		return;
	}
}

void BattleTargetMenu::PrepareForDeactivation() {
	Menu::PrepareForDeactivation();
	upperMenu->MenuSetNextState(MENU_STATE_DEFAULT, true);
	validTargets.clear();
}

bool BattleTargetMenu::StateProcessSelection() {
	return true;
}

bool BattleTargetMenu::StateConfirmSkillUse() {
	return true;
}