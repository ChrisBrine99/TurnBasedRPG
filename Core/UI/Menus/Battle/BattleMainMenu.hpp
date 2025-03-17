#ifndef BATTLE_MAIN_MENU_HPP
#define BATTLE_MAIN_MENU_HPP

#include "../Menu.hpp"

struct Combatant;
class BattleSkillMenu;
class ConfirmWindow;

class BattleMainMenu : public Menu {
public: // Constructor/Destructor Declaration
	BattleMainMenu();
	~BattleMainMenu() = default;

public: // Inherited Function Declarations
	bool OnUserCreate() override;
	bool OnUserUpdate(float_t _deltaTime) override;

public: // Publicly Accessibly Utility Function Declarations
	void PrepareForActivation(uint8_t _state, Combatant* _curCombatant);

private: // State Function Declarations
	bool StateProcessSelection() override;
	bool StateInsideSkills();
	bool StateInsideItems();

private: // Hidden Member Variable Declarations
	BattleSkillMenu*	skillMenu;
	ConfirmWindow*		confirmWindow;
};

#endif