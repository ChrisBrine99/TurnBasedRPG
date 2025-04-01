#include "MenuManager.hpp"
INIT_SINGLETON_CPP(MenuManager)

#include "../ui/menu/Menu.hpp"

MenuManager::MenuManager() :
	activeMenus()
{ // Reserve enough room for at least 5 menus in case a single active parent menu has multiple submenus.
	activeMenus.reserve(5ui64);
}

bool MenuManager::OnUserDestroy() {
	size_t _length = activeMenus.size();
	for (size_t i = 0ui64; i < _length; i++) {
		if (activeMenus[i]) { // Only clear out any menus that weren't properly managed by the scenes/instances that utilized them.
			activeMenus[i]->OnUserDestroy();
			delete activeMenus[i], activeMenus[i] = nullptr;
		}
	}
	activeMenus.clear();
	activeMenus.shrink_to_fit();
	return true;
}

bool MenuManager::OnUserUpdate(float_t _deltaTime) {
	for (Menu* _menu : activeMenus) {
		if (!(_menu->GetFlags() & FLAG_MENU_ACTIVE_STATE))
			continue; // Skip over the inactive menu instances.

		if (!_menu->OnUserUpdate(_deltaTime))
			return false;
	}
	return true;
}

bool MenuManager::OnUserRender(EngineCore* _engine, float_t _deltaTime) {
	for (Menu* _menu : activeMenus) {
		if (!(_menu->GetFlags() & FLAG_MENU_VISIBLE))
			continue; // Skip over the inactive menu instances.

		if (!_menu->OnUserRender(_engine, _deltaTime))
			return false;
	}
	return true;
}

bool MenuManager::OnBeforeUserUpdate(float_t _deltaTime) {
	Menu* _menu		= nullptr;
	size_t _length	= activeMenus.size();
	for (size_t i = 0ui64; i < _length; i++) {
		if (!activeMenus[i]) {
			activeMenus.erase(activeMenus.begin() + i);
			i--; // Offset by one to account for the index being removed from the vector.
			continue;
		}

		_menu = activeMenus[i];
		if (!(_menu->GetFlags() & FLAG_MENU_ACTIVE_STATE))
			continue; // Skip over the inactive menu instances.

		if (!_menu->OnBeforeUserUpdate(_deltaTime))
			return false;
	}
	return true;
}

bool MenuManager::OnAfterUserUpdate(float_t _deltaTime) {
	for (Menu* _menu : activeMenus) {
		if (!(_menu->GetFlags() & FLAG_MENU_ACTIVE_STATE))
			continue; // Skip over the inactive menu instances.
		
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
	if (_menu == nullptr)
		return; // Don't bother doing anything if the pointer is nullptr.

	// Make sure the menu wasn't already deleted by another menu's OnUserDestroy function or by this manager class itself before
	// actually attempting to delete the non-existent instance here. If the menu is still active, it will be within the vector of
	// active menus and should be deleted.
	auto _begin = activeMenus.begin();
	auto _iter	= std::find(_begin, activeMenus.end(), _menu);
	if (_iter == activeMenus.end())
		return;
	activeMenus[std::distance(_begin, _iter)] = nullptr;

	_menu->OnUserDestroy();
	delete _menu, _menu = nullptr;
}

void MenuManager::DeactivateAllMenus() {
	for (Menu* _menu : activeMenus)
		_menu->PrepareForDeactivation();
}