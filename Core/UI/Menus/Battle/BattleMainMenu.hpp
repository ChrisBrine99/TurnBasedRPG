#ifndef BATTLE_MAIN_MENU_HPP
#define BATTLE_MAIN_MENU_HPP

#include "../Menu.hpp"

struct Combatant;
class BattleSkillMenu;

class BattleMainMenu : public Menu {
public: // Constructor/Destructor Declaration
	BattleMainMenu();
	~BattleMainMenu() = default;

public: // Inherited Function Declarations
	bool OnUserCreate() override;
	bool OnUserUpdate(float_t _deltaTime) override;
	bool OnUserRender(float_t _deltaTime) override;

public: // Publicly Accessibly Utility Function Declarations
	void PrepareForPlayerTurn(Combatant* _curCombatant);
	void PostPlayerTurn();

private: // State Function Declarations
	bool StateDefault(float_t _deltaTime);
	bool StateProcessSelection();
	bool StateInsideSkills();

private: // Hidden Member Variable Declarations
	BattleSkillMenu* skillMenu;
};

#endif