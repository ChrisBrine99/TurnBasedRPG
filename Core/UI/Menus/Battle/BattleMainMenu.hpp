#ifndef BATTLE_MAIN_MENU_HPP
#define BATTLE_MAIN_MENU_HPP

#include "../Menu.hpp"

class BattleMainMenu : public Menu {
public: // Constructor Declaration
	BattleMainMenu();
	~BattleMainMenu() = default;

public: // Inherited Function Declarations
	bool OnUserCreate() override;
	bool OnUserUpdate(float_t _deltaTime) override;
	bool OnUserRender(float_t _deltaTime) override;

public: // Publicly Accessibly Utility Function Declarations
	void PrepareForPlayerTurn();
	void PostPlayerTurn();

private: // State Function Declarations
	bool StateAcceptInput(float_t _deltaTime);
};

#endif