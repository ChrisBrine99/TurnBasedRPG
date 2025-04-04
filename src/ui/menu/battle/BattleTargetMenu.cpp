#include "BattleTargetMenu.hpp"

#include "../../../scene/BattleScene.hpp"
#include "../../../singleton/SceneManager.hpp"
#include "../../../singleton/MenuManager.hpp"
#include "../../../struct/Battle/Skill.hpp"
#include "BattleSkillMenu.hpp"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Additional flags that will be used alongside the base menu flags that were inherited by this child class. They provide additional	//
//	toggles specifically for how the target menu functions.																				//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define FLAG_TGTMENU_ONLY_CONFIRM		0x00000100ui32
#define FLAG_TGTMENU_TARGET_SELF		0x00000200ui32

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that condense the checks required for the two additional flags that are defined above.										//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define TGTMENU_CAN_ONLY_CONFIRM		(flags & FLAG_TGTMENU_ONLY_CONFIRM)
#define TGTMENU_WILL_TARGET_SELF		(flags & FLAG_TGTMENU_TARGET_SELF)

BattleTargetMenu::BattleTargetMenu() :
	Menu(),
	validTargets(),
	skillToUse(nullptr)
{ // Reserve enough memory to store the maximum number of combatants in a battle at any one time.
	validTargets.reserve(BATTLE_TOTAL_COMBATANTS);
}

bool BattleTargetMenu::OnUserCreate() {
	InitializeParams(STATE_INVALID, 1ui8, 0ui8, 0ui8, 0ui8, 0ui8, 0xFFui8, FLAG_MENU_BLOCK_INPUT);
	InitializeOptionParams(0i32, 0i32, 0i32, 0i32);
	return true;
}

bool BattleTargetMenu::OnUserDestroy() {
	Menu::OnUserDestroy();

	validTargets.clear();
	validTargets.shrink_to_fit();

	return true;
}

bool BattleTargetMenu::OnUserRender(EngineCore* _engine) {
	std::pair<float_t, float_t> _positions;
	size_t _offset = 0ui64;

	if (TGTMENU_CAN_ONLY_CONFIRM) {
		size_t _length = validTargets.size();
		for (size_t i = 0ui64; i < _length; i++) {
			_positions	= BattleScene::positions[validTargets[i]];
			_engine->DrawRectDecal(
				{ _positions.first, _positions.second },
				{ 32.0f, 32.0f },
				COLOR_LIGHT_YELLOW
			);
		}
		return true;
	}

	_positions = BattleScene::positions[validTargets[curOption]];
	_engine->DrawRectDecal({ _positions.first, _positions.second }, { 32.0f, 32.0f }, COLOR_LIGHT_YELLOW);

	return true;
}

void BattleTargetMenu::PrepareForActivation(uint8_t _state, BattleSkillMenu* _skillMenu, Skill* _skill) {
	Menu::PrepareForActivation(_state);
	DetermineValidTargets(_skill->targeting);
	upperMenu	= _skillMenu;
	skillToUse	= _skill;
}

void BattleTargetMenu::PrepareForDeactivation() {
	Menu::PrepareForDeactivation();
	upperMenu->MenuSetNextState(STATE_MENU_DEFAULT, true);
	upperMenu->SetFlags(upperMenu->GetFlags() & ~FLAG_MENU_BLOCK_INPUT);
	flags &= ~FLAG_TGTMENU_ONLY_CONFIRM;
}

void BattleTargetMenu::DetermineValidTargets(uint8_t _targeting) {
	if (_targeting == TARGET_INVALID && validTargets.size())
		return;
	validTargets.clear();

	BattleScene* _scene			= (BattleScene*)GET_SINGLETON(SceneManager)->curScene;
	Combatant* _curCombatant	= _scene->curCombatant;
	auto& _combatants			= _scene->combatants;
	bool _shouldSkipCaster		= false;

	switch (_targeting) {
	case TARGET_ALL_ENEMY:			// Grab all enemy targets for the user to select from.
	case TARGET_ALL_ENEMY_SELF:
		// There is no need to pick a target out of the valid targets since all will be affected by the current skill.
		flags |= FLAG_TGTMENU_ONLY_CONFIRM;
		[[fallthrough]];
	case TARGET_SINGLE_ENEMY:
	case TARGET_SINGLE_ENEMY_SELF:
		for (size_t i = BATTLE_MAX_PARTY_SIZE; i < BATTLE_TOTAL_COMBATANTS; i++) {
			if (_combatants[i]->isActive)
				validTargets.push_back(i);
		}

		if ((_targeting == TARGET_SINGLE_ENEMY_SELF || _targeting == TARGET_ALL_ENEMY_SELF)) 
			flags |= FLAG_TGTMENU_TARGET_SELF;
		break;
	case TARGET_ALL_ALLY:			// Grab all allied targets for the user to select from.
	case TARGET_ALL_ALLY_SELF:
		// There is no need to pick a target out of the valid targets since all will be affected by the current skill.
		flags |= FLAG_TGTMENU_ONLY_CONFIRM;
		[[fallthrough]];
	case TARGET_SINGLE_ALLY:
	case TARGET_SINGLE_ALLY_SELF:
		_shouldSkipCaster = (_targeting == TARGET_SINGLE_ALLY || _targeting == TARGET_ALL_ALLY);
		for (size_t i = 0ui64; i < BATTLE_MAX_PARTY_SIZE; i++) {
			if (!_combatants[i]->isActive || (_combatants[i] == _curCombatant && _shouldSkipCaster))
				continue;
			validTargets.push_back(i);
		}
		break;
	case TARGET_EVERYONE:			// Skill hits everyone; copy over the entire contents of the turn order.
	case TARGET_EVERYONE_SELF:
		for (size_t i = 0ui64; i < BATTLE_TOTAL_COMBATANTS; i++) {
			if (!_combatants[i]->isActive || (_combatants[i] == _curCombatant && _targeting != TARGET_EVERYONE_SELF))
				continue;
			validTargets.push_back(i);
		}
		flags |= FLAG_TGTMENU_ONLY_CONFIRM;
		break;
	}
}

bool BattleTargetMenu::StateDefault() {
	if (TGTMENU_CAN_ONLY_CONFIRM) {
		if (MINPUT_IS_SELECT_PRESSED) {
			GET_SINGLETON(MenuManager)->DeactivateAllMenus();
			flags &= ~FLAG_TGTMENU_ONLY_CONFIRM;

			BattleScene* _scene = (BattleScene*)GET_SINGLETON(SceneManager)->curScene;
			_scene->targets.insert(_scene->targets.begin(), validTargets.begin(), validTargets.end());
			_scene->ExecuteSkill(skillToUse);
		}
		return true;
	}

	if (MINPUT_IS_SELECT_PRESSED) {
		SET_NEXT_STATE(STATE_MENU_PROCESS_SELECTION);
		selOption = curOption;
		return true;
	}

	int8_t _hMovement = int8_t(MINPUT_IS_RIGHT_PRESSED) - int8_t(MINPUT_IS_LEFT_PRESSED);
	if (_hMovement == 0i8)
		return true;

	if (_hMovement == -1i8) {
		curOption--;
		if (curOption == 0xFFui8)
			curOption = uint8_t(validTargets.size() - 1ui64);
	} else if (_hMovement == 1ui8) {
		curOption++;
		if (curOption == validTargets.size())
			curOption = 0ui8;
	}

	return true;
}

bool BattleTargetMenu::StateProcessSelection() {
	GET_SINGLETON(MenuManager)->DeactivateAllMenus();

	BattleScene* _scene = (BattleScene*)GET_SINGLETON(SceneManager)->curScene;
	if (TGTMENU_WILL_TARGET_SELF) { _scene->targets.push_back(_scene->GetCurCombatantIndex()); }
	_scene->targets.push_back(validTargets[selOption]);

	_scene->ExecuteSkill(skillToUse);
	validTargets.clear();
	return true;
}