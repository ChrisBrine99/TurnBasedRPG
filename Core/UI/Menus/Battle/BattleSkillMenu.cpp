#include "BattleSkillMenu.hpp"

#include "../../../Structs/Battle/Combatant.hpp"
#include "../../../Structs/Battle/Skill.hpp"
#include "../../../Singletons/DataManager.hpp"
#include "BattleMainMenu.hpp"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that explain how the number they contain is utilized by the menu's state machine. Defined here since these values aren't	//
//	used in this context anywhere outside of this menu.																					//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define SKL_MENU_STATE_DEFAULT			0ui8

BattleSkillMenu::BattleSkillMenu() :
	Menu()
{}

bool BattleSkillMenu::OnUserCreate() {
	InitializeParams(INVALID_STATE, 1ui8, 7ui8, 1ui8, 0ui8, 0ui8, 0xFFui8, FLAG_MENU_BLOCK_INPUT);
	InitializeOptionParams(80, 200, 0, 10);
	
	return true;
}

bool BattleSkillMenu::OnUserUpdate(float_t _deltaTime) {
	switch (curState) {
	case SKL_MENU_STATE_DEFAULT:		return StateDefault(_deltaTime);
	case INVALID_STATE:					return true;
	}

	return false;
}

bool BattleSkillMenu::OnUserRender(float_t _deltaTime) {
	GET_SINGLETON(EngineCore)->DrawString(15, 15, "curVisibleRowOffset = " + std::to_string(curVisibleRowOffset));
	RenderVisibleOptions(_deltaTime);
	return true;
}

void BattleSkillMenu::GenerateMenuOptions(Combatant* _curCombatant) {
	if (menuOptions.size() != 0ui64)
		menuOptions.clear(); // Make sure any previously existing options are removed before adding the combatant's skills.

	DataManager* _manager = GET_SINGLETON(DataManager);
	Skill* _skill = nullptr;
	for (uint16_t _id : _curCombatant->activeSkills) {
		_skill = _manager->GetSkill(_id);
		AddOption(0, 0, _skill->name, _skill->description);
	}

	AddOption(0, 0, "Back", "Close the skill selection menu.");
}

void BattleSkillMenu::PrepareForActivation(BattleMainMenu* _bMainMenu) {
	SET_NEXT_STATE(SKL_MENU_STATE_DEFAULT);
	upperMenu = _bMainMenu;

	curOption = 0ui8;
	selOption = 0xFFui8;

	flags &= ~FLAG_MENU_BLOCK_INPUT;
	flags |=  FLAG_MENU_VISIBLE | FLAG_MENU_ACTIVE_STATE;
}

void BattleSkillMenu::PrepareForDeactivation() {
	SET_NEXT_STATE(INVALID_STATE);

	flags |=   FLAG_MENU_BLOCK_INPUT;
	flags &= ~(FLAG_MENU_VISIBLE | FLAG_MENU_ACTIVE_STATE);
}

bool BattleSkillMenu::StateDefault(float_t _deltaTime) {
	UpdateCursor(_deltaTime);
	return true;
}