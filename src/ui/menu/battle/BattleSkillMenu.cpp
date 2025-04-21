#include "BattleSkillMenu.hpp"

#include "../../../singleton/DataManager.hpp"
#include "../../../singleton/MenuManager.hpp"
#include "../../../struct/battle/Skill.hpp"
#include "BattleMainMenu.hpp"
#include "BattleTargetMenu.hpp"

BattleSkillMenu::BattleSkillMenu() :
	Menu(),
	sSkillCost(),
	skillIDs(),
	curCombatant(nullptr)
{ // Reserve enough memory to store the skill costs strings AND their actualy numerical values for all 6 possible active skills.
	sSkillCost.reserve(PLAYER_SKILL_LIMIT);
	skillIDs.reserve(PLAYER_SKILL_LIMIT);
}

bool BattleSkillMenu::OnUserCreate() {
	InitializeParams(STATE_INVALID, 1ui8, 8ui8, 1ui8, 0ui8, 0ui8, 0xFFui8, FLAG_MENU_BLOCK_INPUT);
	InitializeOptionParams(80i32, 260i32, 0i32, 10i32);

	subMenu = CREATE_NEW_MENU(BattleTargetMenu);
	
	return true;
}

bool BattleSkillMenu::OnUserDestroy() {
	Menu::OnUserDestroy();
	DESTROY_MENU(subMenu, BattleTargetMenu);

	sSkillCost.clear();
	sSkillCost.shrink_to_fit();

	skillIDs.clear();
	skillIDs.shrink_to_fit();

	return true;
}

bool BattleSkillMenu::OnUserUpdate() {
	switch (curState) {
	case STATE_MENU_DEFAULT:			return StateDefault();
	case STATE_MENU_PROCESS_SELECTION:	return StateProcessSelection();
	case STATE_SKLMENU_TARGET_SELECT:	return StateTargetSelect();
	case STATE_INVALID:					return true;
	}

	return false;
}

bool BattleSkillMenu::OnUserRender(EngineCore* _engine) {
	RenderVisibleOptions(_engine);

	// Loop through the six menu options that have costs associated with them and render their costs to the right.
	int32_t		_length	= int32_t(sSkillCost.size());
	int8_t		_index	= 0;
	olc::Pixel	_color	= optionColor;
	for (int32_t i = 0; i < _length; i++) {
		_index = int8_t(i + 1);

		// Much like how menu option text is rendered, the skill cost text will have its color altered depending on if 
		// the option is selected, being hovered over by the cursor, within the visible menu region OR it's inactive.
		if (!MOPTION_IS_ACTIVE(_index)) {
			_color = optionInactiveColor;
		} else { // Select a color as normal if the option is active.
			if (selOption == _index)		{ _color = optionSelColor; }
			else if (curOption == _index)	{ _color = optionHoverColor; }
			else							{ _color = optionColor; }
		}

		_engine->DrawStringDecal({ optionAnchorX + 100.0f, optionAnchorY + (optionSpacingY * float_t(_index)) }, sSkillCost[i], _color);
	}

	return true;
}

void BattleSkillMenu::GenerateMenuOptions(Combatant* _curCombatant) {
	// If menu options already existed, it can be assumed the string representation of a skill's cost and the IDs 
	// retrieved alongside said cost are also filled with data, so all three are cleared before getting new options.
	if (menuOptions.size() != 0ui64) {
		menuOptions.clear();
		sSkillCost.clear();
		skillIDs.clear();
	}
	curCombatant = _curCombatant;

	DataManager* _manager = GET_SINGLETON(DataManager);
	Skill* _skill = nullptr;

	// Add the character's basic attack as an option to use.
	AddOption(0, 0, "Attack");

	for (uint16_t _id : _curCombatant->activeSkills) {
		_skill = _manager->GetSkill(_id);
		AddOption(0, 0, _skill->name);
		skillIDs.push_back(_id);

		if (_skill->hpCost > 0ui16 && _skill->mpCost == 0ui16) {
			sSkillCost.push_back("HP " + std::to_string(_skill->hpCost));
			continue; // Skip over the remaining check
		}

		if (_skill->hpCost == 0ui16 && _skill->mpCost > 0ui16) {
			sSkillCost.push_back("MP " + std::to_string(_skill->mpCost));
			continue;
		}

		sSkillCost.push_back("HP " + std::to_string(_skill->hpCost) + " MP " + std::to_string(_skill->mpCost));
	}

	AddOption(0, 0, "Back");
}

void BattleSkillMenu::PrepareForActivation(uint8_t _state, BattleMainMenu* _bMainMenu) {
	Menu::PrepareForActivation(_state);
	upperMenu = _bMainMenu;

	DataManager* _manager = GET_SINGLETON(DataManager);
	Skill* _skill = nullptr;
	for (size_t i = 0ui64; i < skillIDs.size(); i++){
		_skill = _manager->GetSkill(skillIDs[i]);
		if (!SkillHpCostCheck(curCombatant, _skill->hpCost) || !SkillMpCostCheck(curCombatant, _skill->mpCost)) {
			menuOptions[i + 1ui64].flags &= ~FLAG_MOPTION_ACTIVE_STATE;
			continue; // Deactivate the menu option since the party member doesn't meet the requirements to cast it.
		}
		// Always flip the menu option active state bit to 1 if the above check fails.
		menuOptions[i + 1ui64].flags |= FLAG_MOPTION_ACTIVE_STATE;
	}
}

void BattleSkillMenu::PrepareForDeactivation() {
	Menu::PrepareForDeactivation();
	upperMenu->MenuSetNextState(STATE_MENU_DEFAULT, true);
	upperMenu->SetFlags(upperMenu->GetFlags() & ~FLAG_MENU_BLOCK_INPUT);
}

bool BattleSkillMenu::StateProcessSelection() {
	if (selOption == OPTION_SKLMENU_BACK) {
		PrepareForDeactivation();
		return true;
	}

	Skill* _skill = nullptr;
	if (selOption != OPTION_SKLMENU_ATTACK) { // Get the skill from the provided ID.
		_skill = GET_SINGLETON(DataManager)->GetSkill(skillIDs[size_t(selOption) - 1ui64]);
	} else { // Isn't a normal skill; copy pointer from combatant instead of checking the data manager.
		_skill = curCombatant->basicAttack;
	}

	if (_skill == nullptr) { // Atempted to use a non-existent skill; deactive menu and return false.
		PrepareForDeactivation();
		return false;
	}

	BattleTargetMenu* _tMenu = (BattleTargetMenu*)subMenu;
	if (typeid(*_tMenu).hash_code() != typeid(BattleTargetMenu).hash_code())
		return false;

	_tMenu->PrepareForActivation(STATE_MENU_DEFAULT, this, _skill);
	SET_NEXT_STATE(STATE_SKLMENU_TARGET_SELECT);
	flags |= FLAG_MENU_BLOCK_INPUT;

	return true;
}

bool BattleSkillMenu::StateTargetSelect() {
	if (subMenu->GetPressedInputs() & FLAG_INPUT_MENU_RETURN) {
		BattleTargetMenu* _tMenu = (BattleTargetMenu*)subMenu;
		if (typeid(*_tMenu).hash_code() != typeid(BattleTargetMenu).hash_code())
			return false;
		_tMenu->PrepareForDeactivation();
	}

	return true;
}