#ifndef BATTLE_SKILL_MENU_HPP
#define BATTLE_SKILL_MENU_HPP

#include "../Menu.hpp"
#include "../../../Structs/Battle/Combatant.hpp"

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that explain how the number they contain is utilized by this menu's state machine. Defined here since these values aren't	//
//	used in this context anywhere outside of this menu.	The first two values are already used by inherited menu states.					//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define STATE_SKLMENU_TARGET_SELECT		2ui8

// ------------------------------------------------------------------------------------------------------------------------------------	//
//	Defines that are the index values for the options they represent within the menu's option vector. Defined here since no other		//
//	class/piece of code should ever require these defines outside of this menu.															//
// ------------------------------------------------------------------------------------------------------------------------------------	//

#define	OPTION_SKLMENU_ATTACK			0ui8
#define OPTION_SKLMENU_BACK				uint8_t(menuOptions.size() - 1ui64)

class BattleMainMenu;

class BattleSkillMenu : public Menu {
public: // Constructor/Destructor Declaration
	BattleSkillMenu();
	~BattleSkillMenu() = default;

public: // Inherited Function Declarations
	bool OnUserCreate() override;
	bool OnUserDestroy() override;
	bool OnUserUpdate(float_t _deltaTime) override;
	bool OnUserRender(float_t _deltaTime) override;

public: // Publicly Accessibly Utility Function Declarations
	void GenerateMenuOptions(Combatant* _curCombatant);
	void PrepareForActivation(uint8_t _state, BattleMainMenu* _bMainMenu);
	void PrepareForDeactivation();

private: // Hidden Utility Function Declarations

	// Returns if the combatant has enough hitpoints to cast the skill in question. The value has to be GREATER than their
	// current hitpoints since a skill cannot leave the caster with less than one hitpoints after using the skill.
	inline bool SkillHpCostCheck(Combatant* _curCombatant, uint8_t _cost) {
		return (_curCombatant->curHitpoints > _cost);
	}

	// Returns if the combatant has the same amount of or more magicpoints than the skill's required magicpoints or not.
	inline bool SkillMpCostCheck(Combatant* _curCombatant, uint8_t _cost) {
		return (_curCombatant->curMagicpoints >= _cost);
	}

private: // State Function Declarations
	bool StateProcessSelection() override;
	bool StateTargetSelect();

private: // Hidden Member Variable Declarations
	std::vector<std::string>	sSkillCost;
	std::vector<uint16_t>		skillIDs;
	Combatant*					curCombatant;
};

#endif