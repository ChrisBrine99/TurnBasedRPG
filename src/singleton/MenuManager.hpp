#ifndef MENU_MANAGER_HPP
#define MENU_MANAGER_HPP

#include "../utility/GeneralMacros.hpp"

#include <vector>

class EngineCore;
class Menu;

class MenuManager {
	// Singleton Initialization (Creates Constructor/Destrcutor Declarations)
	INIT_SINGLETON_HPP(MenuManager)
public: // Main Engine Function Declarations
	bool OnUserDestroy();
	bool OnUserUpdate();
	bool OnUserRender(EngineCore* _engine);
	
	bool OnBeforeUserUpdate();
	bool OnAfterUserUpdate();

public: // Publicly Accessible Utility Funciton Declarations
	Menu* CreateMenu(Menu* _menu);
	void DestroyMenu(Menu* _menu);
	void DeactivateAllMenus();

private: // Hidden Member Variable Declarations
	std::vector<Menu*> activeMenus;
};

#endif