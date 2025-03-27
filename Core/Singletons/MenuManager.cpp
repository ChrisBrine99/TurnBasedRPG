#include "MenuManager.hpp"
INIT_SINGLETON_CPP(MenuManager)

#include "../UI/Menus/Menu.hpp"

MenuManager::MenuManager() :
	activeMenus()
{ // Reserve enough room for at least 5 menus in case a single active parent menu has multiple submenus.
	activeMenus.reserve(5ui64);
}

bool MenuManager::OnUserDestroy() {
	for (Menu* _menu : activeMenus) {
		_menu->OnUserDestroy();
		delete _menu, _menu = nullptr;
	}
	activeMenus.clear();
	activeMenus.shrink_to_fit();
	return true;
}

bool MenuManager::OnUserUpdate(float_t _deltaTime) {
	for (Menu* _menu : activeMenus) {
		if (!(_menu->GetFlags() & FLAG_MENU_ACTIVE_STATE))
			continue;

		if (!_menu->OnUserUpdate(_deltaTime))
			return false;
	}
	return true;
}

bool MenuManager::OnUserRender(EngineCore* _engine, float_t _deltaTime) {
	for (Menu* _menu : activeMenus) {
		if (!(_menu->GetFlags() & FLAG_MENU_VISIBLE))
			continue;

		if (!_menu->OnUserRender(_engine, _deltaTime))
			return false;
	}
	return true;
}

bool MenuManager::OnBeforeUserUpdate(float_t _deltaTime) {
	for (Menu* _menu : activeMenus) {
		if (!(_menu->GetFlags() & FLAG_MENU_ACTIVE_STATE))
			continue;

		if (!_menu->OnBeforeUserUpdate(_deltaTime))
			return false;
	}
	return true;
}

bool MenuManager::OnAfterUserUpdate(float_t _deltaTime) {
	for (Menu* _menu : activeMenus) {
		if (!(_menu->GetFlags() & FLAG_MENU_ACTIVE_STATE))
			continue;
		
		if (!_menu->OnAfterUserUpdate(_deltaTime))
			return false;
	}
	return true;
}

Menu* MenuManager::CreateMenu(Menu* _menu) {
	// Make sure the same menu instance isn't already a part of the vector of instances.
	if (std::find(activeMenus.begin(), activeMenus.end(), _menu) != activeMenus.end())
		return nullptr;

	if (!_menu->OnUserCreate()) {
		_menu->OnUserDestroy();
		delete _menu, _menu = nullptr;
		return nullptr;
	}

	activeMenus.push_back(_menu);
	return _menu; // Returned itself; the menu was successfully added to the manager.
}

void MenuManager::DestroyMenu(Menu* _menu) {
	if (_menu == nullptr) // The menu passed in was nullptr; don't do anything.
		return;

	// Make sure the menu is cleaned up before it's deallocated.
	_menu->OnUserDestroy();
	delete _menu; // Don't set _menu to nullptr so it can still be found within the management list.

	// Make sure the menu actually exists before the code attempts to remove it from memory.
	auto _iter = std::find(activeMenus.begin(), activeMenus.end(), _menu);
	if (_iter == activeMenus.end())
		return; // Menu instance wasn't in the manager; don't attempt to clear from vector
	activeMenus.erase(_iter);
}

void MenuManager::DeactivateAllMenus() {
	for (Menu* _menu : activeMenus)
		_menu->PrepareForDeactivation();
}