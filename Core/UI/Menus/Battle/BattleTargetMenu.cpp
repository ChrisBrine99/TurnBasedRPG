#include "BattleTargetMenu.hpp"

#include "../../../Singletons/BattleManager.hpp"
#include "../../../Structs/Battle/Skill.hpp"
#include "BattleSkillMenu.hpp"

#define FLAG_TGTMENU_ONLY_CONFIRM		0x00000100ui32

#define TGTMENU_CAN_ONLY_CONFIRM		(flags & FLAG_TGTMENU_ONLY_CONFIRM)

BattleTargetMenu::BattleTargetMenu() :
	Menu(),
	validTargets(),
	skillToUse(nullptr)
{ // Reserve enough memory to store the maximum number of combatants in a battle at any one time.
	validTargets.reserve(BATTLE_TOTAL_COMBATANTS);
}

bool BattleTargetMenu::OnUserCreate() {
	InitializeParams(STATE_INVALID, 1ui8, 8ui8, 1ui8, 0ui8, 0ui8, 0xFFui8, FLAG_MENU_BLOCK_INPUT);
	InitializeOptionParams(100, 15, 0, 10);
	return true;
}

void BattleTargetMenu::PrepareForActivation(uint8_t _state, BattleSkillMenu* _skillMenu, Skill* _skill) {
	Menu::PrepareForActivation(_state);
	upperMenu = _skillMenu;

	if (_skill != skillToUse) {
		DetermineValidTargets(_skill->targeting);
		skillToUse = _skill;
	}
}

void BattleTargetMenu::PrepareForDeactivation() {
	Menu::PrepareForDeactivation();
	upperMenu->MenuSetNextState(STATE_MENU_DEFAULT, true);
	upperMenu->SetFlags(upperMenu->GetFlags() & ~FLAG_MENU_BLOCK_INPUT);
}

void BattleTargetMenu::DetermineValidTargets(uint8_t _targeting) {
	if (_targeting == TARGET_INVALID)
		return;

	if (validTargets.size() > 0ui64)
		validTargets.clear();

	BattleManager* _manager		= GET_SINGLETON(BattleManager);
	Combatant* _curCombatant	= _manager->curCombatant;
	bool _shouldSkipCaster		= (_targeting == TARGET_SINGLE_ALLY || _targeting == TARGET_ALL_ALLY);
	switch (_targeting) {
	case TARGET_ALL_ENEMY:			// Grab all enemy targets for the user to select from.
	case TARGET_ALL_ENEMY_SELF:
		// There is no need to pick a target out of the valid targets since all will be affected by the current skill.
		flags |= FLAG_TGTMENU_ONLY_CONFIRM;
		[[fallthrough]];
	case TARGET_SINGLE_ENEMY:
	case TARGET_SINGLE_ENEMY_SELF:
		for (size_t i : _manager->turnOrder) {
			if (COMBATANT_IS_PLAYER(_manager->combatants[i]))
				continue; // Skip over all player party combatants
			validTargets.push_back(i);
		}
		break;
	case TARGET_ALL_ALLY:			// Grab all allied targets for the user to select from.
	case TARGET_ALL_ALLY_SELF:
		// There is no need to pick a target out of the valid targets since all will be affected by the current skill.
		flags |= FLAG_TGTMENU_ONLY_CONFIRM;
		[[fallthrough]];
	case TARGET_SINGLE_ALLY:
	case TARGET_SINGLE_ALLY_SELF:
		for (size_t i : _manager->turnOrder) {
			if (!COMBATANT_IS_PLAYER(_manager->combatants[i]) || (_curCombatant == _manager->combatants[i] && _shouldSkipCaster))
				continue; // Skip over all enemy combatants OR the caster if they can't be targeted by the skill.
			validTargets.push_back(i);
		}
		break;
	case TARGET_EVERYONE:			// Skill hits everyone; copy over the entire contents of the turn order.
	case TARGET_EVERYONE_SELF:
		for (size_t i : _manager->turnOrder) {
			if (_curCombatant == _manager->combatants[i] && _targeting == TARGET_EVERYONE)
				continue; // Skips over the caster if required by the skill.
			validTargets.push_back(i);
		}
		// There is no need to pick a target out of the valid targets since all will be affected by the current skill.
		flags |= FLAG_TGTMENU_ONLY_CONFIRM;
		break;
	}

	for (size_t i : validTargets) // Create options for all valid targets so their names can be listed for selection.
		AddOption(0, 0, _manager->combatants[i]->character->name);
}

bool BattleTargetMenu::StateDefault(float_t _deltaTime) {
	if (TGTMENU_CAN_ONLY_CONFIRM) {
		if (MINPUT_IS_SELECT_PRESSED)
			SET_NEXT_STATE(STATE_MENU_PROCESS_SELECTION);
		return true;
	}

	return Menu::StateDefault(_deltaTime);
}

bool BattleTargetMenu::StateProcessSelection() {
	return true;
}