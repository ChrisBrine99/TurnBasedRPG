#ifndef MENU_MANAGER_HPP
#define MENU_MANAGER_HPP

#include "../Utils/GeneralMacros.hpp"

#include <math.h>
#include <vector>

class Menu;

class MenuManager {
	INIT_SINGLETON_HPP(MenuManager)
	MenuManager();
public: // Main Engine Function Declarations
	void OnUserDestroy();
	void OnUserUpdate(float_t _deltaTime);
	void OnUserRender(float_t _deltaTime);
	
	void OnBeforeUserUpdate(float_t _deltaTime);
	void OnAfterUserUpdate(float_t _deltaTime);

public: // Publicly Accessible Utility Funciton Declarations
	Menu* CreateMenu(Menu* _menu);
	void DestroyMenu(Menu* _menu);

private: // Hidden Member Variable Declarations
	std::vector<Menu*> activeMenus;
};

#endif