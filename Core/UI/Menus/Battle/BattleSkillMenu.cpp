#include "BattleSkillMenu.hpp"

#include "../../../Structs/Battle/Combatant.hpp"
#include "../../../Structs/Battle/Skill.hpp"
#include "../../../Singletons/DataManager.hpp"
#include "BattleMainMenu.hpp"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that are the index values for the options they represent within the menu's option vector. Defined here since no other		//
//	class/piece of code should ever require these defines outside of this menu.															//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define	SKL_MENU_OPTION_ATTACK			0ui8
#define SKL_MENU_OPTION_BACK			uint8_t(menuOptions.size() - 1ui64)

BattleSkillMenu::BattleSkillMenu() :
	Menu(),
	skillCost()
{ // Reserve enough memory to store the skill costs for all 6 possible active skills.
	skillCost.reserve(PLAYER_SKILL_LIMIT);
}

bool BattleSkillMenu::OnUserCreate() {
	InitializeParams(INVALID_STATE, 1ui8, 8ui8, 1ui8, 0ui8, 0ui8, 0xFFui8, FLAG_MENU_BLOCK_INPUT);
	InitializeOptionParams(80, 200, 0, 10);
	
	return true;
}

bool BattleSkillMenu::OnUserUpdate(float_t _deltaTime) {
	Menu::OnUserUpdate(_deltaTime);

	if (FLAG_IS_MENU_RETURN_ACTIVE)
		PrepareForDeactivation();
	return true;
}

bool BattleSkillMenu::OnUserRender(float_t _deltaTime) {
	RenderVisibleOptions(_deltaTime);

	// Loop through the six menu options that have costs associated with them and render their costs to the right.
	EngineCore* _core	= GET_SINGLETON(EngineCore);
	int32_t		_length	= int32_t(skillCost.size());
	int8_t		_index	= 0;
	olc::Pixel	_color	= optionColor;
	for (int32_t i = 0; i < _length; i++) {
		_index = int8_t(i + 1);

		// Much like how menu option text is rendered, the skill cost text will have its color altered depending on if 
		// the option is selected, being hovered over by the cursor, within the visible menu region OR it's inactive.
		if (!FLAG_IS_MOPTION_ACTIVE(_index)) {
			_color = optionInactiveColor;
		} else { // Select a color as normal if the option is active.
			if (selOption == _index)		{ _color = optionSelColor; }
			else if (curOption == _index)	{ _color = optionHoverColor; }
			else							{ _color = optionColor; }
		}

		_core->DrawString(optionAnchorX + 100, optionAnchorY + (optionSpacingY * _index), skillCost[i], _color);
	}

	return true;
}

void BattleSkillMenu::GenerateMenuOptions(Combatant* _curCombatant) {
	if (menuOptions.size() != 0ui64)
		menuOptions.clear(); // Make sure any previously existing options are removed before adding the combatant's skills.

	AddOption(0, 0, "Attack", "Perform a low-damage physical attack on a single target.");

	DataManager* _manager = GET_SINGLETON(DataManager);
	Skill* _skill = nullptr;
	for (uint16_t _id : _curCombatant->activeSkills) {
		_skill = _manager->GetSkill(_id);
		AddOption(0, 0, _skill->name, _skill->description);

		// Indexes 0x00 to 0x0F are all considered physical-type affinities and will display their HP costs.
		if (_skill->affinity < AFFINITY_FIRE) {
			skillCost.push_back("HP " + std::to_string(_skill->hpCost));
			if (!SkillHpCostCheck(_curCombatant, _skill->hpCost))
				menuOptions[skillCost.size()].flags &= ~FLAG_MOPTION_ACTIVE_STATE;
			continue; // Skip over the remaining check
		}

		// Indexes 0x10 to 0x2F are all considered magic-type affinities and will display their MP costs.
		if (_skill->affinity < AFFINITY_VOID) {
			skillCost.push_back("MP " + std::to_string(_skill->mpCost));
			if (!SkillMpCostCheck(_curCombatant, _skill->mpCost))
				menuOptions[skillCost.size()].flags &= ~FLAG_MOPTION_ACTIVE_STATE;
			continue;
		}

		// All indexes above and including 0x80 are considered equivalent to "void" type skills and will display both HP and MP costs.
		skillCost.push_back("HP " + std::to_string(_skill->hpCost) + " MP " + std::to_string(_skill->mpCost));
		if (!SkillHpCostCheck(_curCombatant, _skill->hpCost) || !SkillMpCostCheck(_curCombatant, _skill->mpCost))
			menuOptions[skillCost.size()].flags &= ~FLAG_MOPTION_ACTIVE_STATE;
	}

	AddOption(0, 0, "Back", "Close the skill selection menu.");
}

void BattleSkillMenu::PrepareForActivation(uint8_t _state, BattleMainMenu* _bMainMenu) {
	Menu::PrepareForActivation(_state);
	upperMenu = _bMainMenu;
}

void BattleSkillMenu::PrepareForDeactivation() {
	Menu::PrepareForDeactivation();
	upperMenu->MenuSetNextState(MENU_STATE_DEFAULT, true);
}

bool BattleSkillMenu::StateProcessSelection() {
	if (selOption == SKL_MENU_OPTION_BACK) {
		PrepareForDeactivation();
		return true;
	}

	switch (selOption) {
	case SKL_MENU_OPTION_ATTACK: // The character's basic attack will be used; open target selection interface.
		break;
	default: // A skill has been selected; open target selection interface.
		break;
	}

	return true;
}