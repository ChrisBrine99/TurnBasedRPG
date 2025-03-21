#ifndef BATTLE_TARGET_MENU_HPP
#define BATTLE_TARGET_MENU_HPP

#include "../Menu.hpp"

class BattleSkillMenu;
struct Skill;

class BattleTargetMenu : public Menu {
public: // Constructor/Destructor Declaration
	BattleTargetMenu();
	~BattleTargetMenu() = default;

public: // Inherited Function Declarations
	bool OnUserCreate() override;
	bool OnUserDestroy() override;

public: // Publicly Accessibly Utility Function Declarations
	void PrepareForActivation(uint8_t _state, BattleSkillMenu* _skillMenu, Skill* _skill);
	void PrepareForDeactivation();

private: // Hidden Utility Function Declarations
	void DetermineValidTargets(uint8_t _targeting);

private: // Hidden (Accessible to Children Only) State Function Declarations
	bool StateDefault(float_t _deltaTime) override;
	bool StateProcessSelection();

private: // Hidden Member Variable Declarations
	std::vector<size_t> validTargets;
	Skill* skillToUse;
};

#endif