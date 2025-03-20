#ifndef MENU_MANAGER_HPP
#define MENU_MANAGER_HPP

#include "../Utils/GeneralMacros.hpp"

#include <math.h>
#include <vector>

class Menu;

class MenuManager {
	// Singleton Initialization (Creates Constructor/Destrcutor Declarations)
	INIT_SINGLETON_HPP(MenuManager)
public: // Main Engine Function Declarations
	bool OnUserDestroy();
	bool OnUserUpdate(float_t _deltaTime);
	bool OnUserRender(float_t _deltaTime);
	
	bool OnBeforeUserUpdate(float_t _deltaTime);
	bool OnAfterUserUpdate(float_t _deltaTime);

public: // Publicly Accessible Utility Funciton Declarations
	Menu* CreateMenu(Menu* _menu);
	void DestroyMenu(Menu* _menu);
	void DeactivateAllMenus();

private: // Hidden Member Variable Declarations
	std::vector<Menu*> activeMenus;
};

#endif