#include "BattleTargetMenu.hpp"

#include "../../../Scenes/BattleScene.hpp"
#include "../../../Singletons/SceneManager.hpp"
#include "../../../Singletons/MenuManager.hpp"
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
	InitializeOptionParams(0i32, 0i32, 0i32, 0i32);
	return true;
}

bool BattleTargetMenu::OnUserDestroy() {
	Menu::OnUserDestroy();

	validTargets.clear();
	validTargets.shrink_to_fit();

	return true;
}

void BattleTargetMenu::PrepareForActivation(uint8_t _state, BattleSkillMenu* _skillMenu, Skill* _skill) {
	Menu::PrepareForActivation(_state);
	upperMenu = _skillMenu;

	DetermineValidTargets(_skill->targeting);
	skillToUse = _skill;
}

void BattleTargetMenu::PrepareForDeactivation() {
	Menu::PrepareForDeactivation();
	upperMenu->MenuSetNextState(STATE_MENU_DEFAULT, true);
	upperMenu->SetFlags(upperMenu->GetFlags() & ~FLAG_MENU_BLOCK_INPUT);
	flags &= ~FLAG_TGTMENU_ONLY_CONFIRM;
}

void BattleTargetMenu::DetermineValidTargets(uint8_t _targeting) {
	if (_targeting == TARGET_INVALID)
		return;
	menuOptions.clear();
	validTargets.clear();

	BattleScene* _scene			= (BattleScene*)GET_SINGLETON(SceneManager)->currentScene;
	Combatant* _curCombatant	= _scene->curCombatant;
	auto& _turnOrder			= _scene->turnOrder;
	size_t _size				= _turnOrder.size();
	bool _shouldSkipCaster		= (_targeting == TARGET_SINGLE_ALLY || _targeting == TARGET_ALL_ALLY);

	switch (_targeting) {
	case TARGET_ALL_ENEMY:			// Grab all enemy targets for the user to select from.
	case TARGET_ALL_ENEMY_SELF:
		// There is no need to pick a target out of the valid targets since all will be affected by the current skill.
		flags |= FLAG_TGTMENU_ONLY_CONFIRM;
		[[fallthrough]];
	case TARGET_SINGLE_ENEMY:
	case TARGET_SINGLE_ENEMY_SELF:
		for (size_t i = 0ui64; i < _size; i++) {
			if (_turnOrder[i] >= BATTLE_MAX_PARTY_SIZE)
				validTargets.push_back(_turnOrder[i]);
		}
		break;
	case TARGET_ALL_ALLY:			// Grab all allied targets for the user to select from.
	case TARGET_ALL_ALLY_SELF:
		// There is no need to pick a target out of the valid targets since all will be affected by the current skill.
		flags |= FLAG_TGTMENU_ONLY_CONFIRM;
		[[fallthrough]];
	case TARGET_SINGLE_ALLY:
	case TARGET_SINGLE_ALLY_SELF:
		for (size_t i = 0ui64; i < BATTLE_MAX_PARTY_SIZE; i++) {
			if (_scene->combatants[i]->isActive || (_scene->combatants[i] == _curCombatant && !_shouldSkipCaster))
				validTargets.push_back(i);
		}
		break;
	case TARGET_EVERYONE:			// Skill hits everyone; copy over the entire contents of the turn order.
	case TARGET_EVERYONE_SELF:
		std::copy(_turnOrder.begin(), _turnOrder.end(), validTargets.begin());
		flags |= FLAG_TGTMENU_ONLY_CONFIRM;
		break;
	}

	auto& _positions = BattleScene::positions;
	for (size_t i : validTargets) // Create options for all valid targets so their names can be listed for selection.
		AddOption(_positions[i].first, _positions[i].second, _scene->combatants[i]->character->name);
}

bool BattleTargetMenu::StateDefault(float_t _deltaTime) {
	if (TGTMENU_CAN_ONLY_CONFIRM) {
		if (MINPUT_IS_SELECT_PRESSED) {
			GET_SINGLETON(MenuManager)->DeactivateAllMenus();
			flags &= ~FLAG_TGTMENU_ONLY_CONFIRM;

			BattleScene* _scene = (BattleScene*)GET_SINGLETON(SceneManager)->currentScene;
			_scene->targets.insert(_scene->targets.begin(), validTargets.begin(), validTargets.end());
			_scene->ExecuteSkill(skillToUse);
		}
		return true;
	}

	return Menu::StateDefault(_deltaTime);
}

bool BattleTargetMenu::StateProcessSelection() {
	GET_SINGLETON(MenuManager)->DeactivateAllMenus();

	BattleScene* _scene = (BattleScene*)GET_SINGLETON(SceneManager)->currentScene;
	_scene->targets.push_back(validTargets[selOption]);
	_scene->ExecuteSkill(skillToUse);
	return true;
}