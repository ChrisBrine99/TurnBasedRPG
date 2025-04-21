#ifndef BATTLE_TARGET_MENU_HPP
#define BATTLE_TARGET_MENU_HPP

#include "../Menu.hpp"

class BattleScene;
class BattleSkillMenu;
struct ActiveSkill;

class BattleTargetMenu : public Menu {
public: // Constructor/Destructor Declaration
	BattleTargetMenu();
	~BattleTargetMenu() = default;

public: // Inherited Function Declarations
	bool OnUserCreate() override;
	bool OnUserDestroy() override;
	bool OnUserRender(EngineCore* _engine) override;

public: // Publicly Accessibly Utility Function Declarations
	void PrepareForActivation(uint8_t _state, BattleSkillMenu* _skillMenu, ActiveSkill* _skill);
	void PrepareForDeactivation();

private: // Hidden Utility Function Declarations
	void DetermineValidTargets(uint8_t _targeting);
	inline void UpdateDisplayedTargetName(size_t _index, std::string_view _name);

private: // Hidden (Accessible to Children Only) State Function Declarations
	bool StateDefault() override;
	bool StateProcessSelection();

private: // Hidden Member Variable Declarations
	std::vector<size_t> validTargets;
	BattleScene*		sceneRef;
	ActiveSkill*		activeSkillRef;
};

#endif