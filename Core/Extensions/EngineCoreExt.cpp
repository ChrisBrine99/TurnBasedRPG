#include "EngineCoreExt.hpp"

#include "../Singletons/DataManager.hpp"
#include "../Singletons/GameSettings.hpp"
#include "../Singletons/MenuManager.hpp"
#include "../Singletons/ObjectManager.hpp"
#include "../Singletons/SceneManager.hpp"

EngineCoreExt::EngineCoreExt(bool _hook) :
	PGEX(_hook)
{}

bool EngineCoreExt::OnBeforeUserCreate() {
	CALL_SINGLETON_CREATE(DataManager, OnBeforeUserCreate);
	CALL_SINGLETON_CREATE(GameSettings, OnBeforeUserCreate);
	return true;
}

bool EngineCoreExt::OnAfterUserCreate() {
	return true;
}

bool EngineCoreExt::OnBeforeUserUpdate(float_t _deltaTime) {
	CALL_SINGLETON_UPDATE(SceneManager, OnBeforeUserUpdate);
	CALL_SINGLETON_UPDATE(MenuManager, OnBeforeUserUpdate);
	return true;
}

bool EngineCoreExt::OnAfterUserUpdate(float_t _deltaTime) {
	CALL_SINGLETON_UPDATE(SceneManager, OnAfterUserUpdate);
	CALL_SINGLETON_UPDATE(MenuManager, OnAfterUserUpdate);
	return true;
}