#include "EngineCoreExt.hpp"

#include "../singleton/DataManager.hpp"
#include "../singleton/EngineCore.hpp"
#include "../singleton/GameSettings.hpp"
#include "../singleton/MenuManager.hpp"
#include "../singleton/ObjectManager.hpp"
#include "../singleton/SceneManager.hpp"

EngineCoreExt::EngineCoreExt(bool _hook) :
	PGEX(_hook)
{}

bool EngineCoreExt::OnBeforeUserCreate() {
	CALL_SINGLETON_CREATE(DataManager, OnBeforeUserCreate);
	CALL_SINGLETON_CREATE(GameSettings, OnBeforeUserCreate);
	return true;
}

bool EngineCoreExt::OnAfterUserCreate() {
	srand((uint32_t)time(NULL));
	return true;
}

bool EngineCoreExt::OnBeforeUserUpdate(float_t _deltaTime) {
	EngineCore::deltaTime = _deltaTime * TARGET_FPS;
	CALL_SINGLETON_UPDATE(ObjectManager, OnBeforeUserUpdate);
	CALL_SINGLETON_UPDATE(SceneManager, OnBeforeUserUpdate);
	CALL_SINGLETON_UPDATE(MenuManager, OnBeforeUserUpdate);
	return true;
}

bool EngineCoreExt::OnAfterUserUpdate(float_t _deltaTime) {
	(void)(_deltaTime);
	CALL_SINGLETON_UPDATE(ObjectManager, OnAfterUserUpdate);
	CALL_SINGLETON_UPDATE(SceneManager, OnAfterUserUpdate);
	CALL_SINGLETON_UPDATE(MenuManager, OnAfterUserUpdate);
	return true;
}