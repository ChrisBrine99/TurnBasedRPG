#ifndef BATTLE_SKILL_MENU_HPP
#define BATTLE_SKILL_MENU_HPP

#include "../Menu.hpp"

struct Combatant;
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
	void PrepareForActivation(BattleMainMenu* _bMainMenu);
	void PrepareForDeactivation();

private: // State Function Declarations
	bool StateDefault(float_t _deltaTime);
};

#endif