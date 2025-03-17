#include "MenuManager.hpp"
INIT_SINGLETON_CPP(MenuManager)

#include "../UI/Menus/Menu.hpp"

MenuManager::MenuManager() :
	activeMenus()
{ // Reserve enough room for at least 5 menus in case a single active parent menu has multiple submenus.
	activeMenus.reserve(5ui64);
}

void MenuManager::OnUserDestroy() {
	for (Menu* _menu : activeMenus) {
		_menu->OnUserDestroy();
		delete _menu, _menu = nullptr;
	}
	activeMenus.clear();
	activeMenus.shrink_to_fit();
}

void MenuManager::OnUserUpdate(float_t _deltaTime) {
	for (Menu* _menu : activeMenus) {
		if (!(_menu->GetFlags() & FLAG_MENU_ACTIVE_STATE))
			continue;
		_menu->OnUserUpdate(_deltaTime);
	}
}

void MenuManager::OnUserRender(float_t _deltaTime) {
	for (Menu* _menu : activeMenus) {
		if (!(_menu->GetFlags() & FLAG_MENU_VISIBLE))
			continue;
		_menu->OnUserRender(_deltaTime);
	}
}

void MenuManager::OnBeforeUserUpdate(float_t _deltaTime) {
	for (Menu* _menu : activeMenus) {
		if (!(_menu->GetFlags() & FLAG_MENU_ACTIVE_STATE))
			continue;
		_menu->OnBeforeUserUpdate(_deltaTime);
	}
}

void MenuManager::OnAfterUserUpdate(float_t _deltaTime) {
	for (Menu* _menu : activeMenus) {
		if (!(_menu->GetFlags() & FLAG_MENU_ACTIVE_STATE))
			continue;
		_menu->OnAfterUserUpdate(_deltaTime);
	}
}

Menu* MenuManager::CreateMenu(Menu* _menu) {
	// Make sure the same menu instance isn't already a part of the vector of instances.
	if (std::find(activeMenus.begin(), activeMenus.end(), _menu) != activeMenus.end())
		return nullptr;

	activeMenus.push_back(_menu);
	return _menu; // Returned itself; the menu was successfully added to the manager.
}

void MenuManager::DestroyMenu(Menu* _menu) {
	// Make sure the menu actuall exists before the code attempts to remove it from memory.
	auto _iter = std::find(activeMenus.begin(), activeMenus.end(), _menu);
	if (_iter == activeMenus.end())
		return; // Menu doesn't exist; exit now.

	_menu->OnUserDestroy(); // Make sure the menu is cleaned up before its deallocated.
	delete _menu, _menu = nullptr;

	// Finally, remove the element formally storing the now-deleted menu from the list.
	activeMenus.erase(_iter);
}