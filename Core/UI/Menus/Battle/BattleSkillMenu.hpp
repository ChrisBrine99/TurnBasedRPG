#ifndef BATTLE_SKILL_MENU_HPP
#define BATTLE_SKILL_MENU_HPP

#include "../Menu.hpp"
#include "../../../Structs/Battle/Combatant.hpp"
#include "../../../Structs/Battle/Skill.hpp"

class BattleMainMenu;

class BattleSkillMenu : public Menu {
public: // Constructor/Destructor Declaration
	BattleSkillMenu();
	~BattleSkillMenu() = default;

public: // Inherited Function Declarations
	bool OnUserCreate() override;
	bool OnUserUpdate(float_t _deltaTime) override;
	bool OnUserRender(float_t _deltaTime) override;

public: // Publicly Accessibly Utility Function Declarations
	void GenerateMenuOptions(Combatant* _curCombatant);
	void PrepareForActivation(uint8_t _state, BattleMainMenu* _bMainMenu);
	void PrepareForDeactivation();

private: // Hidden Utility Function Declarations
	inline bool SkillHpCostCheck(Combatant* _curCombatant, uint8_t _cost) {
		return (_curCombatant->curHitpoints > _cost);
	}

	inline bool SkillMpCostCheck(Combatant* _curCombatant, uint8_t _cost) {
		return (_curCombatant->curMagicpoints >= _cost);
	}

private: // State Function Declarations
	bool StateProcessSelection() override;

private: // Hidden Member Variable Declarations
	std::vector<std::string> skillCost;
};

#endif