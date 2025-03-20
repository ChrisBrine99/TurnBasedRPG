#include "EngineCore.hpp"
INIT_SINGLETON_CPP(EngineCore)

#include "../Extensions/EngineCoreExt.hpp"
#include "BattleManager.hpp"
#include "DataManager.hpp"
#include "GameSettings.hpp"
#include "PartyManager.hpp"
#include "SceneManager.hpp"
#include "MenuManager.hpp"

EngineCore::EngineCore() :
	engineExt(new EngineCoreExt(true))
{ // Sets the name for the application.
	sAppName = "Turn Based RPG";
}

bool EngineCore::OnUserCreate() {
	CALL_SINGLETON_CREATE(DataManager,	OnUserCreate)
	CALL_SINGLETON_CREATE(SceneManager, OnUserCreate)
	return true;
}

bool EngineCore::OnUserDestroy() {
	delete engineExt, engineExt = nullptr;

	CALL_SINGLETON_DESTROY(BattleManager)
	CALL_SINGLETON_DESTROY(DataManager)
	CALL_SINGLETON_DESTROY(GameSettings)
	CALL_SINGLETON_DESTROY(MenuManager)
	CALL_SINGLETON_DESTROY(PartyManager)
	CALL_SINGLETON_DESTROY(SceneManager)
	return true;
}

bool EngineCore::OnUserUpdate(float_t _deltaTime) {
	CALL_SINGLETON_UPDATE(SceneManager, OnUserUpdate)
	CALL_SINGLETON_UPDATE(MenuManager, OnUserUpdate)
	return true;
}

bool EngineCore::OnUserRender(float_t _deltaTime) {
	Clear(olc::BLACK);
	CALL_SINGLETON_RENDER(SceneManager)
	CALL_SINGLETON_RENDER(MenuManager)
	return true;
}