#include "EngineCore.hpp"
INIT_SINGLETON_CPP(EngineCore)

#include "../extension/EngineCoreExt.hpp"
#include "DataManager.hpp"
#include "GameSettings.hpp"
#include "MenuManager.hpp"
#include "ObjectManager.hpp"
#include "PartyManager.hpp"
#include "SceneManager.hpp"

uint32_t	EngineCore::s_HudLayer			= 0ui32;
uint32_t	EngineCore::s_ObjectLayer		= 0ui32;
uint32_t	EngineCore::s_BackgroundLayer	= 0ui32;
float_t		EngineCore::deltaTime			= 0.0f;

EngineCore::EngineCore() :
	engineExt(new EngineCoreExt(true))
{ // Sets the name for the application.
	sAppName = "Turn Based RPG";
}

bool EngineCore::OnUserCreate() {
	CALL_SINGLETON_CREATE(DataManager, OnUserCreate);
	CALL_SINGLETON_CREATE(SceneManager, OnUserCreate);

	s_ObjectLayer		= CreateLayer();
	s_BackgroundLayer	= CreateLayer();

	EnableLayer(s_ObjectLayer, true);
	EnableLayer(s_BackgroundLayer, true);
	return true;
}

bool EngineCore::OnUserDestroy() {
	delete engineExt, engineExt = nullptr;

	CALL_SINGLETON_DESTROY(SceneManager);
	CALL_SINGLETON_DESTROY(PartyManager);
	CALL_SINGLETON_DESTROY(ObjectManager);
	CALL_SINGLETON_DESTROY(MenuManager);
	CALL_SINGLETON_DESTROY(DataManager);
	CALL_SINGLETON_DESTROY(GameSettings);
	return true;
}

bool EngineCore::OnUserUpdate(float_t _deltaTime) {
	(void)(_deltaTime);
	CALL_SINGLETON_UPDATE(ObjectManager, OnUserUpdate);
	CALL_SINGLETON_UPDATE(SceneManager, OnUserUpdate);
	CALL_SINGLETON_UPDATE(MenuManager, OnUserUpdate);
	return true;
}

bool EngineCore::OnUserRender(float_t _deltaTime) {
	(void)(_deltaTime);
	EngineCore* _engine = this; // Required for singletons to work with having to add an additional argument.

	Clear(olc::BLANK);
	SetPixelMode(olc::Pixel::ALPHA);
	CALL_SINGLETON_RENDER(MenuManager);

	SetDrawTarget(s_ObjectLayer);
	Clear(olc::BLANK);
	CALL_SINGLETON_RENDER(ObjectManager);

	SetDrawTarget(s_BackgroundLayer);
	Clear(COLOR_BLACK);
	SetPixelMode(olc::Pixel::NORMAL);
	CALL_SINGLETON_RENDER(SceneManager);

	SetDrawTarget(s_HudLayer);
	return true;
}